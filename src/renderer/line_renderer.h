#ifndef skyla_line_renderer_h
#define skyla_line_renderer_h

#include "renderer.h"
#include "program.h"

NS_SKYLA

class line_renderer : public renderer {
public:
    line_renderer();
    virtual ~line_renderer() {};

public:
    void init() override;
    void shutdown() override;
    void flush() override;

public:
    void draw_line(const affine_transform& world_transform, const vec2& begin, const vec2& end, color_t color);

private:
    color_t           _cur_color;
    pos_color_vertex* _vertex_buffer;
    int32_t           _num_vertices;
    int32_t           _max_vertices;
    program*          _program;
    GLuint            _vbo;
    GLuint            _vao;
};

NS_SKYLA_END

#endif
