#include "timer.h"

#include "skyla_context.h"

NS_SKYLA

void timer::init(float interval, timer_callback_t callback, void* userdata)
{
    _id = context::C()->_timer_mgr->next_timer_counter();
    _remain = interval;
    _interval = interval;
    _callback = callback;
}

void timer::start()
{
    context::C()->_timer_mgr->add(this);
}

void timer::stop()
{

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
    if (_timers.empty()) {
        return;
    }

    timer* t = _timers.top();
    t->_remain -= dt;
    if (t->_remain <= 0) {
        _timers.pop();
        if (!_timers.empty()) {
            /* recursivly process the next timer */
            timer* next = _timers.top();
            next->_remain -= (t->_interval - t->_remain); /*time compensation*/
            this->update(0);
        }
        t->_callback(t->_user_data);
        delete t;
    }
}

void timer_mgr::add(timer* t)
{
    _timers.push(t);
}

NS_SKYLA_END
