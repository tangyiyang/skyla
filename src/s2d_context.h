//
//  Created by yiyang.tang on 2017/9/11.
//

#ifndef s2d_context_h
#define s2d_context_h

#include "s2d_common.h"
#include "s2d_sprite.h"

NS_S2D
class s2d_context {
public:
    void init();
    void update();
    void draw();
    void shutdown();

private:
    sprite* _root;
};

NS_S2D_END

#endif /* s2d_context_h */
