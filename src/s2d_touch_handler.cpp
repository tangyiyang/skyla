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

#include "s2d_touch_handler.h"
#include "s2d_context.h"

NS_S2D

uint64_t touch_handler::_touch_id_counter = 0;

void touch_handler::init()
{
    _touchable_nodes.reserve(S2D_INITIAL_TOUCHABLE_NODES);
}

void touch_handler::handle_touch_event(touch_event *event)
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
    std::vector<node*>::iterator it = _touchable_nodes.begin();
    for (; it != _touchable_nodes.end(); ++it) {
        (*it)->on_touch(event);
    }
}

NS_S2D_END
