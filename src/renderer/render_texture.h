/*
 * Render Texture for editor, the mobile implemention is on it's way.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_render_texture_h
#define skyla_render_texture_h

#include "program.h"
#include "texture.h"

#include "game_objects/node.h"

NS_SKYLA

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

NS_SKYLA_END

#endif
