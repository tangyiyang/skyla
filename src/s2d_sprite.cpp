/****************************************************************************
Copyright (c) Yiyang Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "s2d_sprite.h"
#include "s2d_context.h"

#define STRINGFY(x) #x

#define CHECK_GL_ERROR check_gl_error(__FILE__, __LINE__);


void check_gl_error(const char* file, int line)
{
    GLenum err = GL_NO_ERROR;
    if((err = glGetError()) != GL_NO_ERROR) {
        printf("gl error: %04x. file = %s, line = %d\n", err, file, line);
    }
}

static const char* vs_primitive = STRINGFY(\n
                                           precision lowp float;\n
                                           attribute mediump vec2 vertex_pos;\n

                                           void main() {\n
                                               gl_Position = vec4(vertex_pos.x, vertex_pos.y, 0.0, 1.0); \n
                                           }\n
                                           );

static const char* fs_primitive = STRINGFY(\n
                                           precision lowp float;\n
                                           
                                           void main() {\n
                                               gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
                                           }\n
                                           );


static GLuint create_program(GLuint vs, GLuint fs)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar strInfoLog[4096] = "";
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        printf("Linker failure: %s\n", strInfoLog);
    }
    glDetachShader(program, vs);
    glDetachShader(program, fs);

    CHECK_GL_ERROR;
    return program;
}

static GLuint create_shader(GLenum shader_type, const char* shader_data)
{
    GLuint shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, &shader_data, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
        {
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

        printf("Compile failure in %s shader:\n%s\n shader_src = %s",
             strShaderType, strInfoLog, shader_data);
        }
    
    return shader;
}

NS_S2D

void sprite::init()
{
    _local_transform = affine_transform::mk_identity();
    _pos = {0, 0};

    _scale = {1.0, 2.0};
    _rotaion = {0, 0};
    _anchor = {0, 0};
    _size = {200, 100};

    _vertex[0].x = 0;
    _vertex[0].y = 0;
    _vertex[1].x = 100;
    _vertex[1].y = 100;
    _vertex[2].x = 200;
    _vertex[2].y = 0;

    int index = 0;
    const char* shaders[] = {
        vs_primitive,
        fs_primitive,
    };


    GLuint vs = create_shader(GL_VERTEX_SHADER, shaders[index]);
    GLuint fs = create_shader(GL_FRAGMENT_SHADER, shaders[index+1]);

    _program = create_program(vs, fs);

    printf("sprite.init vs, fs, program = %d, %d, %d\n", vs, fs, _program);

    _vbo = 0;
    _vao = 0;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindAttribLocation(_program, 0, "vertex_pos");
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(float)*2, 0);
    glBindVertexArray(0);

    CHECK_GL_ERROR;
}

void sprite::update_srt()
{
    _local_transform = affine_transform::mk_identity();

    float anchor_x = _size.width * _anchor.x;
    float anchor_y = _size.height * _anchor.y;

    affine_transform anchor_to = affine_transform::mk_translate(-anchor_x, -anchor_y);
    affine_transform scale = affine_transform::mk_scale(_scale.x, _scale.y);
    affine_transform rotation = affine_transform::mk_rotation(_rotaion.x, _rotaion.y);
    affine_transform translate = affine_transform::mk_translate(_pos.x, _pos.y);

    affine_transform::inplace_concat(_local_transform, anchor_to);
    affine_transform::inplace_concat(_local_transform, scale);
    affine_transform::inplace_concat(_local_transform, rotation);
    affine_transform::inplace_concat(_local_transform, translate);
}

void sprite::update()
{
    this->update_srt();

    context* ctx = context::_global_context;

    vec2 tmp[3];
    tmp[0] = affine_transform::apply_transform(_local_transform, _vertex[0].x, _vertex[0].y);
    tmp[1] = affine_transform::apply_transform(_local_transform, _vertex[1].x, _vertex[1].y);
    tmp[2] = affine_transform::apply_transform(_local_transform, _vertex[2].x, _vertex[2].y);

    const affine_transform& mv = ctx->_model_view_affine_transform;

    tmp[0] = affine_transform::apply_transform(mv, tmp[0].x, tmp[0].y);
    tmp[1] = affine_transform::apply_transform(mv, tmp[1].x, tmp[1].y);
    tmp[2] = affine_transform::apply_transform(mv, tmp[2].x, tmp[2].y);

    camera* c = ctx->_camera;
    matrix4* cm = &c->_m;
    _buffer[0] = matrix4::multiply_vec2(cm, tmp[0]);
    _buffer[1] = matrix4::multiply_vec2(cm, tmp[1]);
    _buffer[2] = matrix4::multiply_vec2(cm, tmp[2]);
}

void sprite::draw()
{
    printf("%.2f, %.2f\n", _buffer[0].x, _buffer[0].y);
    printf("%.2f, %.2f\n", _buffer[1].x, _buffer[1].y);
    printf("%.2f, %.2f\n", _buffer[2].x, _buffer[2].y);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6, _buffer, GL_DYNAMIC_DRAW);

    glUseProgram(_program);

    glBindVertexArray(_vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    CHECK_GL_ERROR;
}

NS_S2D_END
