#include "s2d_particle_renderer.h"
#include "s2d_util.h"

NS_S2D

/* transform-feedback shader is special, it's initialization progcess is different, the code were here.*/
static const char* vs_feed_back = ""
#if S2D_IS_DESKTOP
"#version 100\n"
#endif
""
" attribute mediump float in_value;                         \n"
" varying mediump float out_value;                          \n"
" void main() {                                             \n"
"       out_value = sqrt(in_value);                         \n"
" }                                                         \n";


particle_renderer::particle_renderer()
{

}

void particle_renderer::init()
{
#if S2D_IS_DESKTOP
    /* create the program. */
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &vs_feed_back, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint info_log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

        GLchar log_info[4096] = "";
        glGetShaderInfoLog(shader, info_log_length, NULL, log_info);
        LOGE("Compile failure in vertex shader:\n%s\n shader_src = %s", log_info, vs_feed_back);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, shader);

    /* generate the VAO. */
    const GLchar* feedbackVaryings[] = { "out_value" };
    glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(program);
    glUseProgram(program);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* specify the vertex attribute and submit the data. */
    GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    GLint inputAttrib = glGetAttribLocation(program, "in_value");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

    /* Back buffer to store the data. */
    GLuint tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_READ);

    /* make the shader compute the data */
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, 5);
    glEndTransformFeedback();
    glFlush();

    GLfloat feedback[5];
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

    for (int i = 0; i < 5; ++i) {
        LOGD("i = %.2f", feedback[i]);
    }
    glDisable(GL_RASTERIZER_DISCARD);

    CHECK_GL_ERROR;
#endif
}

void particle_renderer::shutdown()
{

}

void particle_renderer::flush()
{

}

NS_S2D_END
