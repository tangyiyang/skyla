#include "quad_renderer.h"
#include "skyla_context.h"

NS_SKYLA

quad_renderer::quad_renderer()
{
    _blend_mode = BLEND_MODE_NONE;
    _program = nullptr;
    _texture = nullptr;
    _vertex_buffer = nullptr;
    _index_buffer = nullptr;
    _vbo = 0;
    _ibo = 0;
    _vao = 0;
    _num_indexes = 0;
    _max_indexes = 0;
    _num_vertices = 0;
    _max_vertices = 0;
}

void quad_renderer::init()
{
    _num_vertices = 0;
    _max_vertices = SKYLA_MAX_VERTEX_PER_DRAW_CALL;
    _num_indexes = 0;
    _max_indexes = _max_vertices/4*6;
    _vertex_buffer = (pos_tex_color_vertex*)malloc(sizeof(pos_tex_color_vertex) * _max_vertices);
    _index_buffer = (index_t*)malloc(sizeof(index_t) * _max_indexes);

    _program = program::load_default_program(program::EMBEDED_PROGRAM_SPRITE_DEFAULT);
    glGenBuffers(1, &_vbo); SKYLA_ASSERT(_vbo > 0);
    glGenBuffers(1, &_ibo);
    glGenVertexArrays(1, &_vao);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_tex_color_vertex) * _max_vertices, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_t) * _max_indexes, nullptr, GL_STATIC_DRAW);

    glVertexAttribPointer(program::VERTEX_ATTR_POS,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(pos_tex_color_vertex),
                          (void*)offsetof(pos_tex_color_vertex, pos));
    glEnableVertexAttribArray(program::VERTEX_ATTR_POS);
    glVertexAttribPointer(program::VERTEX_ATTR_TEX_COORD,
                          2,
                          GL_UNSIGNED_SHORT,
                          GL_TRUE,
                          sizeof(pos_tex_color_vertex),
                          (void*)offsetof(pos_tex_color_vertex, uv));
    glEnableVertexAttribArray(program::VERTEX_ATTR_TEX_COORD);
    glVertexAttribPointer(program::VERTEX_ATTR_COLOR,
                          4,
                          GL_UNSIGNED_BYTE,
                          GL_TRUE,
                          sizeof(pos_tex_color_vertex),
                          (void*)offsetof(pos_tex_color_vertex, color));
    glEnableVertexAttribArray(program::VERTEX_ATTR_COLOR);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void quad_renderer::shutdown()
{
    _program->shutdown();

    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ibo);
    glDeleteVertexArrays(1, &_vao);
    free(_vertex_buffer);
    free(_index_buffer);
}

void quad_renderer::draw(const affine_transform& world_transform,
                         texture* tex,
                         blend_mode mode,
                         pos_tex_color_vertex* quad,
                         int n)
{
    SKYLA_ASSERT(n % 4 == 0);
    SKYLA_ASSERT(tex != nullptr);
    if (_texture == nullptr) {
        _texture = tex;
    } else {
        if (_texture != tex) {
            this->flush();
            _texture = tex;
        }
    }

    if (_blend_mode == BLEND_MODE_NONE) {
        _blend_mode = mode;
        _blend_func = renderer::blend_mode_to_func(_blend_mode);
    } else {
        if (_blend_mode != mode) {
            this->flush();
            _blend_mode = mode;
            _blend_func = renderer::blend_mode_to_func(_blend_mode);
        }
    }

    if (_num_vertices + n > _max_vertices) {
        this->flush();
    }

    context* ctx = context::C();

    // TODO: can this be put in shader?
    const affine_transform& mv = ctx->_world_view_affine_transform;
    affine_transform t = affine_transform::concat(world_transform, mv);

    pos_tex_color_vertex* p = _vertex_buffer + _num_vertices;

    for (int i = 0; i < n; ++i) {
        (p+i)->pos = affine_transform::apply_transform(t, quad[i].pos.x, quad[i].pos.y);
        (p+i)->color = quad[i].color;
        (p+i)->uv = quad[i].uv;
    }

    _num_vertices += n;
}

void quad_renderer::flush()
{
    this->update_indexes();
    if (!(_num_indexes>0) || !_program || !_texture) {
        return;
    }

    _program->use();
    CHECK_GL_ERROR;
    _program->set_uniform("u_projection",
                          program::UNIFORM_TYPE_MATRIX_3_FV,
                          context::C()->_camera->_matrix.m);
    CHECK_GL_ERROR;
    _texture->bind();
    CHECK_GL_ERROR;

    glEnable(GL_BLEND);
    glBlendFunc(_blend_func.src, _blend_func.dst);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * sizeof(pos_tex_color_vertex), _vertex_buffer, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _num_indexes * sizeof(index_t), _index_buffer, GL_DYNAMIC_DRAW);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _num_indexes, GL_UNSIGNED_SHORT, 0);

    _program->unuse();
    _texture->unbind();
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _num_indexes = 0;
    _num_vertices = 0;
}

void quad_renderer::update_indexes()
{
    for (int i = 0, j = 0; i < _num_vertices; i += 4, j += 6) {
        _index_buffer[j+0] = i+0;
        _index_buffer[j+1] = i+1;
        _index_buffer[j+2] = i+2;
        _index_buffer[j+3] = i+3;
        _index_buffer[j+4] = i+2;
        _index_buffer[j+5] = i+1;
    }
    _num_indexes = _num_vertices/4*6;
}

NS_SKYLA_END
