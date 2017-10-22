/****************************************************************************
 * Copyright (c) Yiyang Tang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

#include "s2d_program.h"

static const char* vs_sprite = ""
#if S2D_IS_DESKTOP
"#version 100\n"
#endif
""
" attribute mediump vec2 pos;                               \n"
" attribute highp   vec2 tex_coord;                         \n"
" attribute lowp    vec4 color;                             \n"
" varying highp vec2 v_tex_coord;                           \n"
" varying lowp vec4 v_color;                                \n"

" uniform mat3 u_projection;                                \n"
" void main() {                                             \n"
     "vec3 tmp = u_projection * vec3(pos.x, pos.y, 0.0);    \n"
     "gl_Position = vec4(tmp.x, tmp.y, 0.0, 1.0);           \n"
     "v_tex_coord = tex_coord;                              \n"
     "v_color = color;                                      \n"
 "}                                                         \n";

static const char* fs_sprite = ""
#if S2D_IS_DESKTOP
"#version 100                                               \n"
#endif
" varying highp vec2 v_tex_coord;                           \n"
" varying lowp vec4 v_color;                                \n"
" uniform sampler2D texture0;                               \n"
" void main() {\n"
"     gl_FragColor = v_color * texture2D(texture0, v_tex_coord); \n"
" }                                                         \n";


static const char* vs_render_texture = ""
#if S2D_IS_DESKTOP
"#version 100\n"
#endif
""
" attribute mediump vec2 pos;                       \n"
" attribute highp vec2 tex_coord;                   \n"
" varying   highp vec2 v_tex_coord;                 \n"
" void main() {                                     \n"
"     gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);   \n"
"     v_tex_coord = tex_coord;                      \n"
"}                                                  \n";

static const char* fs_render_Texture = ""
#if S2D_IS_DESKTOP
"#version 100\n"
#endif
""
" varying highp vec2 v_tex_coord;                 \n"
" varying lowp vec4 color;                        \n"
" uniform sampler2D texture0;                    \n"
" void main() {                                  \n"
"   color = texture2D(texture0, v_tex_coord);    \n"
" }"
"";

NS_S2D

program* program::load_default_program(EMBEDED_PROGRAMS type)
{
    static const char* shaders[EMBEDED_PROGRAM_MAX*2] =
    {
        vs_sprite,
        fs_sprite,
        vs_render_texture,
        fs_render_Texture,
    };
    int vs_index = type*2;
    int fs_index = type*2 + 1;
    GLuint vs = load_shader(GL_VERTEX_SHADER, shaders[vs_index]);
    CHECK_GL_ERROR
    GLuint fs = load_shader(GL_FRAGMENT_SHADER, shaders[fs_index]);
    CHECK_GL_ERROR
    program* p = new program();
    p->init(vs, fs);
    return p;
}

GLuint program::load_shader(GLenum shader_type, const char* shader_data)
{
    GLuint shader = glCreateShader(shader_type);
    CHECK_GL_ERROR
    glShaderSource(shader, 1, &shader_data, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar strInfoLog[4096] = "";
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = NULL;
        switch(shader_type) {
            case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
            case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
            default: strShaderType = "unkown"; break;
        }

        LOGE("Compile failure in %s shader:\n%s\n shader_src = %s",
               strShaderType, strInfoLog, shader_data);
    }

    return shader;
}

static const char* vertex_attr_names[program::VERTEX_ATTR_COUNT] = {
    "pos",
    "text_coord",
    "color",
};

void program::bind_vertex_locations(GLuint program)
{
    for (int i = 0; i < VERTEX_ATTR_COUNT; ++i) {
        glBindAttribLocation(program, i, vertex_attr_names[i]);
        CHECK_GL_ERROR
    }
}

GLuint program::load_program(GLuint vs, GLuint fs)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    bind_vertex_locations(program);
    
    glLinkProgram(program);

    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar strInfoLog[4096] = "";
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        LOGE("Linker failure: %s\n", strInfoLog);
    }
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    CHECK_GL_ERROR
    return program;
}

void program::init(GLuint vs, GLuint fs)
{
    _program_handle = load_program(vs, fs);
}

void program::shutdown()
{
    glDeleteProgram(_program_handle);
}

void program::set_uniform(const char* name, UNIFORM_TYPE type, float* value, bool transpose /*= false*/)
{
    std::map<std::string, GLint>::iterator it = _map_uniform_location.find(name);
    GLint location = -1;
    if (it == _map_uniform_location.end()) {
        location = glGetUniformLocation(_program_handle, name);
        _map_uniform_location[name] = location;
    } else {
        location = _map_uniform_location[name];
    }

    switch (type) {
        case UNIFORM_TYPE_1_F:
            glUniform1f(location, *value);
            break;
        case UNIFORM_TYPE_MATRIX_3_FV:
            glUniformMatrix3fv(location, 1, transpose, value);
            break;

        default:
            LOGD("unknown uniform name = %s, type = %d", name, type);
            break;
    }
}

NS_S2D_END
