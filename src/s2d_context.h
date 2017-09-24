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
    node*            _root;
    camera*          _camera;

    affine_transform _world_view_affine_transform;
    
public:
    static context* _global_context;
};

NS_S2D_END

#endif /* s2d_context_h */
