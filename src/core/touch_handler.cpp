#include "touch_handler.h"

#include "skyla_context.h"

NS_SKYLA

uint64_t touch_handler::_touch_id_counter = 0;

void touch_handler::init()
{
    _touchable_nodes.reserve(SKYLA_INITIAL_TOUCHABLE_NODES);
}

const std::vector<node*> touch_handler::touch_event_test(touch_event* event)
{
    _touchable_nodes.clear();

    node* root = context::C()->get_root();
    root->hit_test(this, event);

    std::vector<node*> copy(_touchable_nodes);
    _touchable_nodes.clear();
    return copy;
}

void touch_handler::handle_touch_event(touch_event* event)
{
    node* root = context::C()->get_root();
    switch (event->_phase) {
        case touch_event::TOUCH_BEGIN: {
            // TOOD: Do this way have performance issue?
            // test all the children in the scene.
            root->hit_test(this, event);
            this->send_touch_events(event);
            break;
        }

        case touch_event::TOUCH_MOVED: {
            this->send_touch_events(event);
            break;
        }
        case touch_event::TOUCH_ENDED:
        case touch_event::TOUCH_CANCL:
        {
            this->send_touch_events(event);
            _touchable_nodes.clear();
            break;
        }
    }
}

void touch_handler::send_touch_events(touch_event* event)
{
    std::vector<node*>::reverse_iterator it = _touchable_nodes.rbegin();
    for (; it != _touchable_nodes.rend(); ++it) {
        (*it)->on_touch(event);
    }
}

NS_SKYLA_END
