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
#include "s2d_util.h"


#define STRINGFY(x) #x

#define CHECK_GL_ERROR check_gl_error(__FILE__, __LINE__);

void check_gl_error(const char* file, int line)
{
    GLenum err = GL_NO_ERROR;
    if((err = glGetError()) != GL_NO_ERROR) {
        LOGE("gl error: %04x. file = %s, line = %d\n", err, file, line);
    }
}

#ifdef USE_SPRITE_VERTEX
static const char* vs_primitive = STRINGFY(\n
                                           attribute mediump vec2 pos;      \n
                                           attribute highp   vec2 tex_coord;\n
                                           attribute lowp    vec4 color; \n

                                           varying highp vec2 v_tex_coord; \n
                                           varying lowp vec4 v_color; \n

                                           uniform mat3 u_projection; \n
                                           void main() {              \n
                                               vec3 tmp = u_projection * vec3(pos.x, pos.y, 0.0); \n
                                               gl_Position = vec4(tmp.x, tmp.y, 0.0, 1.0); \n

                                               v_tex_coord = tex_coord;
                                               v_color = color;
                                           }\n
                                           );

static const char* fs_primitive = STRINGFY(\n
                                           varying highp vec2 v_tex_coord; \n
                                           varying lowp vec4 v_color; \n

                                           uniform sampler2D texture0;
                                           void main() {\n
                                               gl_FragColor = texture2D(texture0, v_tex_coord) * v_color;
                                           }\n
                                           );
#else
static const char* vs_primitive = STRINGFY(\n
                                           precision lowp float;\n
                                           attribute mediump vec2 vertex_pos;\n
                                           uniform mat3 u_projection; \n
                                           void main() {\n
                                               vec3 pos = u_projection * vec3(vertex_pos.x, vertex_pos.y, 0.0); \n
                                               gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); \n
                                           }\n
                                           );

static const char* fs_primitive = STRINGFY(\n
                                           precision lowp float;\n

                                           void main() {\n
                                               gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
                                           }\n
                                           );
#endif

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
    _pos = {300, 300};

    _scale = {1.0, 1.0};
    _rotation = 30;
    _anchor = {0, 0};
    _size = {200, 100};

#ifdef USE_SPRITE_VERTEX
    _quad[0].pos.x = 0;
    _quad[0].pos.y = 0;
    _quad[0].uv.u = 0;
    _quad[0].uv.v = 0;
    _quad[0].color = 0xffffff;

    _quad[1].pos.x = 0;
    _quad[1].pos.y = 100;
    _quad[1].uv.u = 0;
    _quad[1].uv.v = (1 << 15);
    _quad[1].color = 0xffffff;

    _quad[2].pos.x = 100;
    _quad[2].pos.y = 0;
    _quad[2].uv.u = (1<<15);
    _quad[2].uv.v = 0;
    _quad[2].color = 0xffffff;

    _quad[3].pos.x = 100;
    _quad[3].pos.y = 100;
    _quad[3].uv.u = (1<<15);
    _quad[3].uv.v = (1<<15);
    _quad[3].color = 0xffffff;
#else
    _vertex[0].x = 0;
    _vertex[0].y = 0;
    _vertex[1].x = 100;
    _vertex[1].y = 400;
    _vertex[2].x = 200;
    _vertex[2].y = 0;
#endif
    int index = 0;
    const char* shaders[] = {
        vs_primitive,
        fs_primitive,
    };


    GLuint vs = create_shader(GL_VERTEX_SHADER, shaders[index]);
    GLuint fs = create_shader(GL_FRAGMENT_SHADER, shaders[index+1]);

    _program = create_program(vs, fs);
    CHECK_GL_ERROR;

    LOGD("sprite.init vs, fs, program = %d, %d, %d\n", vs, fs, _program);


    _vbo = 0;
    _vao = 0;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
#ifdef USE_SPRITE_VERTEX
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindAttribLocation(_program, 0, "pos");
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(float)*2, 0);
    glBindAttribLocation(_program, 0, "tex_coord");
    glVertexAttribPointer(0, 2, GL_UNSIGNED_SHORT, GL_TRUE, sizeof(float)*2, 0);
    glBindAttribLocation(_program, 0, "color");
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(float)*2, 0);

    glBindVertexArray(0);
#else
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindAttribLocation(_program, 0, "vertex_pos");
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
    glBindVertexArray(0);
#endif




    CHECK_GL_ERROR;
    
    _texture = new texture();
    _texture->init("res/seal2d-opacity.png");
    
}

void sprite::update_srt()
{
    _local_transform = affine_transform::mk_identity();

    float anchor_x = _size.width * _anchor.x;
    float anchor_y = _size.height * _anchor.y;

//    affine_transform anchor_to = affine_transform::mk_translate(-anchor_x, -anchor_y);
    affine_transform scale = affine_transform::mk_scale(_scale.x, _scale.y);
    affine_transform rotation = affine_transform::mk_rotation(_rotation);
    affine_transform translate = affine_transform::mk_translate(_pos.x, _pos.y);

//    affine_transform::inplace_concat(_local_transform, anchor_to);
    affine_transform::inplace_concat(_local_transform, scale);
    affine_transform::inplace_concat(_local_transform, rotation);
    affine_transform::inplace_concat(_local_transform, translate);
}

void sprite::update()
{
    this->update_srt();

    context* ctx = context::_global_context;

#ifdef USE_SPRITE_VERTEX
    vec2 tmp[3];
    tmp[0] = affine_transform::apply_transform(_local_transform, _quad[0].pos.x, _quad[0].pos.y);
    tmp[1] = affine_transform::apply_transform(_local_transform, _quad[1].pos.x, _quad[1].pos.y);
    tmp[2] = affine_transform::apply_transform(_local_transform, _quad[2].pos.x, _quad[2].pos.y);
#else
    const affine_transform& mv = ctx->_world_view_affine_transform;
    
//    affine_transform t = affine_transform::concat(mv, _local_transform);
//    
//    _buffer[0] = affine_transform::apply_transform(t, _vertex[0].x, _vertex[0].y);
//    _buffer[1] = affine_transform::apply_transform(t, _vertex[1].x, _vertex[1].y);
//    _buffer[2] = affine_transform::apply_transform(t, _vertex[2].x, _vertex[2].y);
    
    vec2 tmp[3];
    tmp[0] = affine_transform::apply_transform(_local_transform, _vertex[0].x, _vertex[0].y);
    tmp[1] = affine_transform::apply_transform(_local_transform, _vertex[1].x, _vertex[1].y);
    tmp[2] = affine_transform::apply_transform(_local_transform, _vertex[2].x, _vertex[2].y);
//
//    
//
    _buffer[0] = affine_transform::apply_transform(mv, tmp[0].x, tmp[0].y);
    _buffer[1] = affine_transform::apply_transform(mv, tmp[1].x, tmp[1].y);
    _buffer[2] = affine_transform::apply_transform(mv, tmp[2].x, tmp[2].y);
#endif
}

void sprite::draw()
{
    glEnable(GL_DEPTH_TEST);

#ifdef USE_SPRITE_VERTEX
#else
    printf("%.2f, %.2f\n", _buffer[0].x, _buffer[0].y);
    printf("%.2f, %.2f\n", _buffer[1].x, _buffer[1].y);
    printf("%.2f, %.2f\n", _buffer[2].x, _buffer[2].y);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6, _buffer, GL_DYNAMIC_DRAW);
#endif


    glUseProgram(_program);
    CHECK_GL_ERROR;

    _u_projection = glGetUniformLocation(_program, "u_projection");
    context* ctx = context::_global_context;
    camera* c = ctx->_camera;
    glUniformMatrix3fv(_u_projection, 1, GL_FALSE, c->_matrix.m);

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    CHECK_GL_ERROR;
}

NS_S2D_END
