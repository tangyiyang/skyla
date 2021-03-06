/*
 * Filesystem implemention for all platforms.
 * Notice we may be able to cache the file if we need.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_file_system_h
#define skyla_file_system_h

#include "platform.h"
#include "skyla_common.h"

NS_SKYLA

class file_entry : public ref_counter {
public:
    file_entry();
    virtual ~file_entry();

    inline uint8_t* take()
    {
        uint8_t* buffer = _buffer;
        _buffer = 0;
        _size = 0;
        return buffer;
    }
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

    inline const std::string& get_sandbox_path()
    {
        return _sandbox_path;
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

NS_SKYLA_END

#endif
