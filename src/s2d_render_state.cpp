#include "s2d_context.h"
#include "s2d_render_state.h"
#include "s2d_quad_renderer.h"
#include "s2d_line_renderer.h"
#include "s2d_sprite.h"
#include "s2d_particle.h"
#include "s2d_primitive_node.h"

NS_S2D

void render_state::init()
{
    _cur_renderer = nullptr;
    _cur_renderer_type = MAX_RENDERER_TYPE;
    _renderers[RENDERER_TYPE_QUAD] = new quad_renderer();
    _renderers[RENDERER_TYPE_QUAD]->init();

    _renderers[RENDERER_TYPE_LINE] = new line_renderer();
    _renderers[RENDERER_TYPE_LINE]->init();
}

void render_state::shutdown()
{
    for (int i = 0; i < MAX_RENDERER_TYPE; ++i) {
        _renderers[i]->shutdown();
        delete _renderers[i];
    }
    _cur_renderer = nullptr;
    _cur_renderer_type = MAX_RENDERER_TYPE;
}

void render_state::clear()
{
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

renderer* render_state::switch_renderer(render_type type)
{
    if (_cur_renderer_type == MAX_RENDERER_TYPE) {
        /* lazy initialize */
        _cur_renderer_type = type;
    } else {
        if (_cur_renderer_type != type) {
            _cur_renderer->flush();
            _cur_renderer_type = type;
        }
    }
    _cur_renderer = _renderers[type];
    return _cur_renderer;
}

void render_state::draw_quad(const affine_transform& t,
                             texture* tex,
                             blend_mode b,
                             pos_tex_color_vertex* p)
{
    switch_renderer(RENDERER_TYPE_QUAD);
    quad_renderer* r = dynamic_cast<quad_renderer*>(_cur_renderer);
    CHECK_GL_ERROR;
    r->draw(t, tex, b, p, 4);
}

void render_state::draw_sprite(s2d::sprite *s)
{
    switch_renderer(RENDERER_TYPE_QUAD);
    quad_renderer* r = dynamic_cast<quad_renderer*>(_cur_renderer);

    r->draw(s->world_transform(), s->_texture, s->_blend_mode, s->_quad, 4);
}

void render_state::draw_particle(particle* p)
{
    /* TODO: using particle renderer later on */
    switch_renderer(RENDERER_TYPE_QUAD);
    quad_renderer* r = dynamic_cast<quad_renderer*>(_cur_renderer);

    r->draw(p->world_transform(), p->_texture, p->_blend_mode, p->_vertices, p->_num_vertices);
}

void render_state::draw_primitive(primitive_node* p)
{
    switch_renderer(RENDERER_TYPE_LINE);
    line_renderer* r = dynamic_cast<line_renderer*>(_cur_renderer);

    std::vector<line>::iterator it = p->_lines.begin();
    for (; it != p->_lines.end(); ++it) {
        r->draw_line(p->world_transform(), it->begin, it->end, it->color);
    }
}

void render_state::push_scissors(const rect& r)
{
    float content_scale_factor = context::C()->get_content_scale_factor();
    _scissors_stack.push(r);
    glEnable(GL_SCISSOR_TEST);
    glScissor(r.origin.x * content_scale_factor,
              r.origin.y * content_scale_factor,
              r.size.width * content_scale_factor,
              r.size.height * content_scale_factor);
}

void render_state::pop_scissors()
{
    _scissors_stack.pop();
    if (_scissors_stack.empty()) {
        glDisable(GL_SCISSOR_TEST);
    }
}

void render_state::flush()
{
    if (_cur_renderer) {
        _cur_renderer->flush();
    }
}

NS_S2D_END
