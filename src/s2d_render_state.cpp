#include "s2d_render_state.h"
#include "s2d_quad_renderer.h"
#include "s2d_particle_renderer.h"
#include "s2d_sprite.h"
#include "s2d_particle.h"

NS_S2D

void render_state::init()
{
    _cur_renderer = nullptr;
    _cur_renderer_type = MAX_RENDERER_TYPE;
    _renderers[RENDERER_TYPE_QUAD] = new quad_renderer();
    _renderers[RENDERER_TYPE_QUAD]->init();

    _renderers[RENDERER_TYPE_PARTICLE] = new particle_renderer();
    _renderers[RENDERER_TYPE_PARTICLE]->init();
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

renderer* render_state::switch_renderer(RENDERER_TYPE type)
{
    if (_cur_renderer_type == MAX_RENDERER_TYPE) {
        /* lazy initialize */
        _cur_renderer_type = type;
        _cur_renderer = _renderers[type];
    } else {
        if (_cur_renderer_type != type) {
            _cur_renderer->flush();
            _cur_renderer = _renderers[type];
        }
    }
    return _cur_renderer;
}

void render_state::draw_sprite(s2d::sprite *s)
{
    switch_renderer(RENDERER_TYPE_QUAD);
    quad_renderer* r = dynamic_cast<quad_renderer*>(_cur_renderer);

    r->draw(s->_model_view, s->_texture, s->_quad, 4);
}

void render_state::draw_particle(particle* p)
{
    /* TODO: using particle renderer later on */
    switch_renderer(RENDERER_TYPE_QUAD);
    quad_renderer* r = dynamic_cast<quad_renderer*>(_cur_renderer);

    r->draw(p->_model_view, p->_texture, p->_vertices, p->_num_vertices);
}

void render_state::push_scissors(const rect& r)
{
    _scissors_stack.push(r);
    glEnable(GL_SCISSOR_TEST);
    glScissor(r.origin.x, r.origin.y, r.size.width, r.size.height);
}

void render_state::pop_scissors()
{
    _scissors_stack.pop();
    if (_scissors_stack.size() == 0) {
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
