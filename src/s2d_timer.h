#ifndef s2d_timer_h
#define s2d_timer_h

#include "s2d_common.h"

NS_S2D

typedef std::function<void(void*)> timer_callback_t;
typedef uint32_t    timer_id_t;
class timer {
public:
    void init(float interval, timer_callback_t callback, int loop, void* userdata);
    void start();
    void stop();

    inline bool operator< (timer* a)
    {
        return !(this->_remain < a->_remain);
    }

public:
    timer_id_t       _id;
    float            _remain;
    float            _interval;
    bool             _valid;
    timer_callback_t _callback;
    int              _loop;
    void*            _user_data;

};

class timer_mgr {
public:
    void init();
    void shutdown();
    void update(float dt);

    inline int next_timer_counter()
    {
        return _timer_id_counter++;
    }
public:
    void add(timer* t);
    
private:
    uint32_t    _timer_id_counter;
    std::priority_queue<timer*> _timers;
};


NS_S2D_END

#endif /* s2d_timer_h */
