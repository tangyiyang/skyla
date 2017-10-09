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

#ifndef s2d_touch_handler_h
#define s2d_touch_handler_h

#include "s2d_common.h"
#include "s2d_util.h"

NS_S2D

class node;
struct touch_event {
    enum TOUCH_EVENT_PHASE {
        TOUCH_BEGIN = 0,
        TOUCH_MOVED,
        TOUCH_ENDED,
        TOUCH_CANCL,
    };

public:
    uint64_t          _id;
    vec2              _pos;
    TOUCH_EVENT_PHASE _phase;
};

class touch_handler {
public:
    static uint64_t touch_id()
    {
        return _touch_id_counter++;
    }

public:
    void init();

    void handle_touch_event(touch_event* event);

    inline void add_touch_node(node* n)
    {
        _touchable_nodes.push_back(n);
    }

private:
    void send_touch_events(touch_event* event);

private:
    std::vector<node*> _touchable_nodes;

private:
    static uint64_t _touch_id_counter;
};

NS_S2D_END

#endif
