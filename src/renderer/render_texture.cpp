#include "render_texture.h"

#include "core/util.h"
#include "skyla_context.h"

NS_SKYLA

render_texture::render_texture()
{
    _fbo = 0;
    _upside_down = false;
}

render_texture::~render_texture()
{
    if (_fbo) {
        glDeleteFramebuffers(1, &_fbo);
    }
}

bool render_texture::init(float width, float height)
{
    _size = {width, height};

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_name);
    glBindTexture(GL_TEXTURE_2D, _name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _name, 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _name, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    CHECK_GL_ERROR;

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("incomplete framebuffer stat.");
        return false;
    }

    return true;
}

void render_texture::draw(node* tree)
{
    context* C = context::C();

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _size.width, _size.height);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    // TODO: we should have an visit function to display different types.
    tree->update(0);
    tree->draw(C->_render_state);
    C->_render_state->flush();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

NS_SKYLA_END
