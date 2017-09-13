//
//  Created by yiyang.tang on 2017/9/11.
//

#ifndef s2d_context_h
#define s2d_context_h

#include "s2d_common.h"
#include "s2d_sprite.h"
#include "s2d_camera.h"

NS_S2D
class context {
public:
    void init(float width, float height);
    void update();
    void draw();
    void shutdown();

public:
    sprite* _root;
    camera* _camera;

    matrix4 _model_view_matrix;
public:
    static context* _global_context;
};

NS_S2D_END

#endif /* s2d_context_h */
