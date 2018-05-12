/*
 * Quad renderer, draw quads or triangles with this.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_quad_renderer_h
#define skyla_quad_renderer_h

#include "skyla_common.h"
#include "program.h"
#include "renderer/texture.h"
#include "renderer.h"

NS_SKYLA

class quad_renderer : public renderer {
public:
    quad_renderer();
    virtual ~quad_renderer() {};
public:
    void init();
    void shutdown();
    void flush();

    void draw(const affine_transform& world_transform,
              texture* tex,
              blend_mode mode,
              pos_tex_color_vertex* vertices,
              int n);
private:
    void update_indexes();

private:
    blend_mode            _blend_mode;
    blend_func            _blend_func;
    program*              _program;
    texture*              _texture;
    index_t*              _index_buffer;
    pos_tex_color_vertex* _vertex_buffer;
    int32_t               _num_indexes;
    int32_t               _num_vertices;
    int32_t               _max_vertices;
    int32_t               _max_indexes;
    GLuint                _ibo;
    GLuint                _vbo;
    GLuint                _vao;
};

NS_SKYLA_END

#endif
