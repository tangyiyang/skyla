#include "s2d_line_renderer.h"
#include "s2d_context.h"

NS_S2D

line_renderer::line_renderer()
{
    _cur_color = 0;
    _vertex_buffer = nullptr;
    _num_vertices = -1;
    _max_vertices = 0;
    _program = nullptr;
    _vbo = 0;
    _vao = 0;
}

void line_renderer::init()
{
    _num_vertices = 0;
    _max_vertices = S2D_MAX_LINE_VERTEX_BUFFER_SIZE;
    _vertex_buffer = (pos_color_vertex*)malloc(sizeof(pos_color_vertex) * _max_vertices);

    _program = program::load_default_program(program::EMBEDED_PROGRAM_LINE_DEFAULT);
    glGenBuffers(1, &_vbo); S2D_ASSERT(_vbo > 0);
    glGenVertexArrays(1, &_vao);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_color_vertex) * _max_vertices, nullptr, GL_STATIC_DRAW);

    glVertexAttribPointer(program::VERTEX_ATTR_POS,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(pos_color_vertex),
                          (void*)offsetof(pos_color_vertex, pos));
    glEnableVertexAttribArray(program::VERTEX_ATTR_POS);
    glVertexAttribPointer(program::VERTEX_ATTR_COLOR,
                          4,
                          GL_UNSIGNED_BYTE,
                          GL_TRUE,
                          sizeof(pos_color_vertex),
                          (void*)offsetof(pos_color_vertex, color));
    glEnableVertexAttribArray(program::VERTEX_ATTR_COLOR);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERROR;
}

void line_renderer::shutdown()
{
    _program->shutdown();

    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
    free(_vertex_buffer);
}

void line_renderer::flush()
{
    if (!_program) {
        return;
    }

    _program->use();
    CHECK_GL_ERROR;
    _program->set_uniform("u_projection",
                          program::UNIFORM_TYPE_MATRIX_3_FV,
                          context::C()->_camera->_matrix.m);


    glLineWidth(1.0);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _num_vertices * sizeof(pos_color_vertex), _vertex_buffer);


    glDrawArrays(GL_LINES, 0, _num_vertices);

    _program->unuse();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _num_vertices = 0;
}

void line_renderer::draw_line(const affine_transform& world_transform, const vec2& begin, const vec2& end, color_t color)
{
    if (_num_vertices + 2 > this->_max_vertices) {
        this->flush();
    }

    context* ctx = context::C();

    const affine_transform& mv = ctx->_world_view_affine_transform;
    affine_transform t = affine_transform::concat(world_transform, mv);

    pos_color_vertex* vertex = _vertex_buffer + _num_vertices;

    vertex[0].pos = affine_transform::apply_transform(t, begin.x, begin.y);
    vertex[0].color = color;
    vertex[1].pos = affine_transform::apply_transform(t, end.x, end.y);
    vertex[1].color = color;

    _num_vertices += 2;
}

NS_S2D_END

