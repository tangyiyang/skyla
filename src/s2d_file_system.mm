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

#include "s2d_file_system.h"
#include "s2d_util.h"

#ifdef S2D_IS_APPLE
#import <Foundation/Foundation.h>
#endif

NS_S2D

#if S2D_IS_APPLE

static const char* sys_writable_path()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    return [documentsDirectory UTF8String];
}

static const char* sys_sandbox_path()
{
    return [[[NSBundle mainBundle] resourcePath] UTF8String];
}

uint8_t* sys_read(const char* path, uint8_t* buffer, size_t* size) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        LOGE("s_read, can't open file path = %s.\n", path);
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    size_t file_size = ftell(fp);
    rewind(fp);

    // always allocated 1 extra byte, this would make deal with C string easier.
    if (!buffer) {
        buffer = (unsigned char*)malloc(file_size+1);
    }
    memset(buffer, 0, file_size+1);
    size_t result = fread(buffer, 1, file_size, fp);

    if(result != file_size) {
        free(buffer);
        fclose(fp);
        fprintf(stderr, "s_read, file reading error, size not match?.\n");
        return NULL;
    }
    
    if (size) {
        *size = result;
    }
    fclose(fp);
    return buffer;
}

bool sys_exist(const char* path)
{
    return fopen(path, "r") != NULL;
}

#endif

file_system::file_system() :
_file_counter(0),
_prefer_external(true)
{
    
}

void file_system::init()
{
    _writable_path = sys_writable_path();
    _sandbox_path = sys_sandbox_path();
    
    _writable_path += "/";
    _sandbox_path += "/";
    
    if (_prefer_external) {
        _search_path.push_back(_writable_path);
        _search_path.push_back(_sandbox_path);
    } else {
        _search_path.push_back(_sandbox_path);
        _search_path.push_back(_writable_path);
    }
}

bool file_system::exist(const char* path)
{
    return sys_exist(path);
}

file_entry* file_system::read(const char* path)
{
    file_entry* e = _file_cache[path];
    if (e) {
        return e;
    }
    std::vector<std::string>::iterator it = _search_path.begin();
    for (; it != _search_path.end(); ++it) {
        std::string full_path = *it + path;
        if (this->exist(full_path.c_str())) {
            size_t size;
            uint8_t* buffer = sys_read(full_path.c_str(), nullptr, &size);
            file_entry* f = new file_entry(_file_counter++);
            f->_buffer = buffer;
            f->_path = full_path;
            f->_size = size;
            
            _file_cache[path] = f;
            return f;
        }
    }
    
    LOGE("file could not be found at path: %s", path);
    S2D_ASSERT(path);
    return nullptr;
}

NS_S2D_END
