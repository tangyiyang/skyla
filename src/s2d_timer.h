#ifndef s2d_timer_h
#define s2d_timer_h

#include "s2d_common.h"

NS_S2D

typedef std::function<void(void*)> timer_callback_t;
typedef uint32_t    timer_id_t;

class timer {
public:
    void init(float interval, timer_callback_t callback, void* userdata);
    void start();
    void stop();

public:
    timer_id_t       _id;
    float            _remain;
    float            _interval;
    timer_callback_t _callback;
    void*            _user_data;
};

struct timer_compare {
    bool operator() (timer* const a, timer* const b)
    {
        return a->_remain > b->_remain;
    }
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
    std::priority_queue<timer*, std::vector<timer*>, timer_compare> _timers;
    float       _time_elapsed;
};


NS_S2D_END

#endif /* s2d_timer_h */
