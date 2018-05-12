#include "core/util.h"

#include "profiler.h"

NS_SKYLA

void profiler::begin(const char* name)
{
    profiler_entry* p = this->obtain(name);
    p->_number_of_calls++;
    p->_start_time = std::chrono::high_resolution_clock::now();
}

void profiler::end(const char* name)
{
    auto now = std::chrono::high_resolution_clock::now();
    profiler_entry* p = this->obtain(name);
    long duration = (long)(std::chrono::duration_cast<std::chrono::microseconds>(now - p->_start_time).count());

    p->_total_time += duration;
    p->_average_time_1 = (p->_average_time_1 + duration) / 2.0f;
    p->_average_time_2 = p->_total_time / p->_number_of_calls;
    p->_max_time = std::max(p->_max_time, duration);
    p->_min_time = std::min(p->_min_time, duration);
}

void profiler::reset(const char* name)
{
    this->obtain(name)->reset();
}

void profiler::log_all_stats()
{
    std::map<std::string, profiler_entry*>::iterator it = _profilers.begin();
    for (; it != _profilers.end(); ++it) {
        LOGD("%s", it->second->get_stats().c_str());
    }
}

profiler_entry* profiler::obtain(const char *profiler_name)
{
    std::map<std::string, profiler_entry*>::iterator found = _profilers.find(profiler_name);
    if (found != _profilers.end()) {
        return found->second;
    } else {
        profiler_entry* p = new profiler_entry();
        p->_name = profiler_name;
        _profilers[profiler_name] = p;
        return p;
    }
}

void profiler::unload(const char* profiler_name)
{
    std::map<std::string, profiler_entry*>::iterator found = _profilers.find(profiler_name);
    if (found != _profilers.end()) {
        delete found->second;
        _profilers.erase(found);
    }
}

void profiler::shutdown()
{
    std::map<std::string, profiler_entry*>::iterator it = _profilers.begin();
    for (; it != _profilers.end(); ++it) {
        delete it->second;
    }
    _profilers.clear();
}


NS_SKYLA_END
