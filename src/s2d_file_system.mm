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
        buffer = (unsigned char*)malloc(file_size);
    }
    memset(buffer, 0, file_size);
    size_t result = fread(buffer, 1, file_size, fp);

    if(result != file_size) {
        free(buffer);
        fclose(fp);
        fprintf(stderr, "s_read, file reading error.\n");
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

file_entry::file_entry() {
    _id = -1;
    _buffer = nullptr;
    _size = 0;
}

file_entry::~file_entry()
{
    if (_buffer) {
        free(_buffer);
    }
}

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

void file_system::add_search_path(const char* relative_path)
{
    if (std::find(_search_path.begin(), _search_path.end(), relative_path) != _search_path.end()) {
        return;
    }
    _search_path.push_back(relative_path);
}

bool file_system::exist(const char* path)
{
    return sys_exist(path);
}

file_entry* file_system::read(const char* path, bool cache)
{
    std::map<std::string, file_entry*>::iterator found = _file_cache.find(path);
    if (found != _file_cache.end()) {
        return found->second;
    }
    
    std::vector<std::string>::iterator it = _search_path.begin();
    for (; it != _search_path.end(); ++it) {
        std::string full_path = *it + path;
        if (this->exist(full_path.c_str())) {
            size_t size;
            uint8_t* buffer = sys_read(full_path.c_str(), nullptr, &size);
            file_entry* f = new file_entry();
            f->_id = _file_counter++;
            f->_buffer = buffer;
            f->_path = full_path;
            f->_size = size;
            if (cache) {
                f->retain();
                _file_cache[path] = f;
            }
            return f;
        }
    }
    
    LOGE("file could not be found at path: %s", path);
    S2D_ASSERT(path);
    return nullptr;
}

void file_system::shutdown()
{
    std::map<std::string, file_entry*>::iterator it = _file_cache.begin();
    for (; it != _file_cache.end(); ++it) {
        it->second->release();
    }
    _file_cache.clear();
}

NS_S2D_END
