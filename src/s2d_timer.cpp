#include "s2d_timer.h"
#include "s2d_context.h"

NS_S2D

void timer::init(float interval, timer_callback_t callback, int loop, void* userdata)
{
    _id = context::C()->_timer_mgr->next_timer_counter();
    _valid = true;
    _remain = interval;
    _interval = interval;
    _callback = callback;
    _loop = loop;
}

void timer::start()
{
    context::C()->_timer_mgr->add(this);
}

void timer::stop()
{
    _valid = false;
}

void timer_mgr::init()
{
    _timer_id_counter = 0;
}

void timer_mgr::shutdown()
{
    while(!_timers.empty()) {
        timer* t = _timers.top();
        LOGD("delete timer %d", t->_id);
        delete t;
        _timers.pop();
    }
}

void timer_mgr::update(float dt)
{
    timer* t = _timers.top();
    t->_remain -= dt;
    if (t->_remain <= 0) {

        LOGD("update timer %d", t->_id);
        /* process one timer each update */
        if (t->_valid && t->_loop > 0) {
            t->_loop--;
            t->_callback(t->_user_data);

            if (t->_loop < 0) {
                t->_valid = false;
                _timers.pop();
            } else {
                t->_remain = (t->_interval + t->_remain);
            }

            if (!_timers.empty()) {
                /* get the next timer */
                timer* next = _timers.top();
                LOGD("update next timer %d", next->_id);
                next->_remain -= (t->_interval - t->_remain); /*time compensation*/
            }

            /* clean up*/
            if (!t->_valid) {
                delete t;
            }

            /* recursivly process the next timer */
            this->update(0);
        }
    }
}

void timer_mgr::add(timer* t)
{
    _timers.push(t);
}

NS_S2D_END
