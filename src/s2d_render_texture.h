/*
 * Render Texture for editor, the mobile implemention is on it's way.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_render_texture_h
#define s2d_render_texture_h

#include "s2d_node.h"
#include "s2d_program.h"
#include "s2d_texture.h"

NS_S2D

class render_texture : public texture
{
public:
    render_texture();
    virtual ~render_texture();

    bool init(float width, float height);
    void draw(node* tree);

private:
    GLuint _fbo;
};

NS_S2D_END

#endif
