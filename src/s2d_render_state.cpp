#include "s2d_render_state.h"
#include "s2d_quad_renderer.h"
#include "s2d_sprite.h"

NS_S2D

void render_state::init()
{
    _cur_renderer = nullptr;
    _renderers[RENDERER_TYPE_QUAD] = new quad_renderer();
    _renderers[RENDERER_TYPE_QUAD]->init();

}

void render_state::shutdown()
{
    for (int i = 0; i < MAX_RENDERER_TYPE; ++i) {
        _renderers[i]->shutdown();
        delete _renderers[i];
    }
    _cur_renderer = nullptr;
}

void render_state::clear()
{
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render_state::draw_sprite(s2d::sprite *s)
{
    if (!_cur_renderer) {
        // lazy init, for the sake of renderer not been used.
        _cur_renderer = _renderers[RENDERER_TYPE_QUAD];
    } else {
        // switch from other type of renderer
        if (_cur_renderer != _renderers[RENDERER_TYPE_QUAD]) {
            _cur_renderer->flush();
            _cur_renderer = _renderers[RENDERER_TYPE_QUAD];
        }
    }

    quad_renderer* r = dynamic_cast<quad_renderer*>(_cur_renderer);
    r->draw(s->_model_view, s->_texture, s->_quad, 4);
}

void render_state::flush()
{
    S2D_ASSERT(_cur_renderer);
    _cur_renderer->flush();
}

NS_S2D_END
