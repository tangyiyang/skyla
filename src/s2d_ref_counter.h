/*
 * Ref counter, this is used to manage the Resources(Texture, Program ...)
 * Notice we may be able to cache the file if we need.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_ref_counter_h
#define s2d_ref_counter_h

#include "s2d_macro.h"

NS_S2D

class ref_counter {
public:
    ref_counter() : _rc(1) {}
    virtual ~ref_counter() {}
    
    inline void retain()
    {
        ++_rc;
    }
    
    inline void release()
    {
        --_rc;
        if (_rc == 0) {
            delete this;
        }
    }
    
protected:
    int32_t _rc;
};

NS_S2D_END

#endif
