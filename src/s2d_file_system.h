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
    file_entry(uint32_t id = -1) : _id(id) {}
    
    void release()
    {
        --_rc;
        if (_rc == 0) {
            free(_buffer);
            delete this;
        }
    }
public:
    uint32_t _id;
    std::string _path;
    uint8_t* _buffer;
    size_t _size;
};

class file_system {
public:
    file_system();
    inline const std::string& get_writable_path() { return _writable_path; }
    
public:
    void init();
    bool exist(const char* path);
    file_entry* read(const char* path);
    
private:
    uint32_t _file_counter;
    bool _prefer_external;
    std::string _writable_path;
    std::string _sandbox_path;
    std::vector<std::string> _search_path;
    
    std::map<std::string, file_entry*> _file_cache;
};

NS_S2D_END


#endif /* s2d_file_system_h */