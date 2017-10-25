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
