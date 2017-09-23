//
//  Created by yiyang.tang on 2017/9/11.
//

#ifndef s2d_context_h
#define s2d_context_h

#include "s2d_common.h"
#include "s2d_sprite.h"
#include "s2d_camera.h"
#include "s2d_file_system.h"
#include "s2d_sprite_renderer.h"

NS_S2D
class context {
public:
    void init(float width, float height);
    void loop(float dt);
    void shutdown();

public:
    file_system*     _file_system;
    sprite_renderer* _sprite_renderer;
    sprite*          _root;
    camera*          _camera;

    affine_transform _world_view_affine_transform;
    
public:
    static context* _global_context;
};

NS_S2D_END

#endif /* s2d_context_h */
