#ifndef s2d_macro_h
#define s2d_macro_h

#include <assert.h>

#define STRINGIFY(s) #s

#define NS_S2D namespace s2d {
#define NS_S2D_END }

#define SAFE_RELEASE(obj) do { if(obj) { obj->release(); }}while(0);

#endif 
