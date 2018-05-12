/*
 * Ref counter, this is used to manage the Resources(Texture, Program ...)
 * Notice we may be able to cache the file if we need.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef ref_counter_h
#define ref_counter_h

#include "skyla_macro.h"

NS_SKYLA

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

NS_SKYLA_END

#endif
