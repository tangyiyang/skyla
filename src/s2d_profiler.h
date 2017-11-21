/*
 * Profiler for seal2d, taken from cocos2d-x
 * Rewriter: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_profiler_h
#define s2d_profiler_h

#include "s2d_common.h"

NS_S2D

class profiler_entry {
public:
    profiler_entry()
    {
        this->reset();
    }

    void reset()
    {
        _average_time_1 = 0;
        _average_time_2 = 0;
        _min_time = LONG_MAX;
        _max_time = 0;
        _total_time = 0;
        _number_of_calls = 0;
    }

    std::string get_stats()
    {
        static char stats[256] = "";
        snprintf(stats, 256, "%s ::\tavg1: %ldu,\tavg2: %ldu,""\tmin: %ldu,\tmax: %ldu,\ttotal: %.2lds,\tnr calls: %ld",
                 this->_name.c_str(), 
                 this->_average_time_1, this->_average_time_2,
                 this->_min_time, this->_max_time,
                 this->_total_time, this->_number_of_calls
                 );
        return std::string(stats);
    }

    std::string _name;
    std::chrono::high_resolution_clock::time_point _start_time;
    long _average_time_1;
    long _average_time_2;
    long _min_time;
    long _max_time;
    long _total_time;
    long _number_of_calls;
};

class profiler {
public:
    void begin(const char* name);
    void end(const char* name);
    void reset(const char* name);
    std::string get_stats(const char* name);
    void log_all_stats();

public:
    profiler_entry* obtain(const char* profiler_name);
    void            unload(const char* profiler_name);
    void            shutdown();

private:
    std::map<std::string, profiler_entry*> _profilers;
};


NS_S2D_END

#endif /* s2d_profiler_h */
