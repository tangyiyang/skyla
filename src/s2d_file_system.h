/*
 * Filesystem implemention for all platforms.
 * Notice we may be able to cache the file if we need.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_file_system_h
#define s2d_file_system_h

#include "s2d_platform.h"
#include "s2d_common.h"

NS_S2D

class file_entry : public ref_counter {
public:
    file_entry();
    virtual ~file_entry();

public:
    uint32_t    _id;
    uint8_t*    _buffer;
    size_t      _size;
    std::string _path;
};

class file_system {
public:
    file_system();

    inline const std::string& get_writable_path()
    {
        return _writable_path;
    }

    void add_search_path(const char* relative_path);
    void insert_full_search_path(const char* full_path);
    file_entry* read(const char* path, bool cache);

public:
    void init();
    bool exist(const char* path);
    void shutdown();

private:
    uint32_t _file_counter;
    bool _prefer_external;
    std::string _writable_path;
    std::string _sandbox_path;
    std::vector<std::string> _search_path;
    
    std::map<std::string, file_entry*> _file_cache;
};

NS_S2D_END

#endif
