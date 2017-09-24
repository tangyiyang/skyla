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

#ifndef s2d_program_h
#define s2d_program_h

#include "s2d_common.h"
#include "s2d_gl_util.h"
#include "s2d_util.h"

NS_S2D

class program : public ref_counter {
public:
    program() : _program_handle(0) {}

    enum EMBEDED_PROGRAMS {
        EMBEDED_PROGRAM_SPRITE_DEFAULT = 0,

        EMBEDED_PROGRAM_MAX
    };

    enum UNIFORM_TYPE {
        UNIFORM_TYPE_1_F = 0,
        UNIFORM_TYPE_MATRIX_3_FV,

        UNIFORM_TYPE_MAX,
    };

public:
    static program* load_default_program(EMBEDED_PROGRAMS type);
    static GLuint load_shader(GLenum shader_type, const char* shader_data);
    static GLuint load_program(GLuint vs, GLuint fs);

public:
    void init(GLuint vs, GLuint fs);
    void shutdown();
    
    GLuint enable_attribute(const char* attr_name);
    void set_uniform(const char* name, UNIFORM_TYPE type, float* value, bool transpose = false);

    inline void use()
    {
        S2D_ASSERT(_program_handle > 0);
        glUseProgram(_program_handle);
    }

    inline void unuse()
    {
        glUseProgram(0);
    }

    inline GLuint get_handle()
    {
        return _program_handle;
    }

    inline GLuint get_vertex_attr_location(const char* name)
    {
        return _map_vertex_attribute_location[name];
    }

private:
    GLuint _program_handle;
    std::map<std::string, GLuint> _map_vertex_attribute_location;
    std::map<std::string, GLint> _map_uniform_location;
};

NS_S2D_END

#endif /* s2d_program_h */
