/*
 * Particle Renderer, implemented OpenGLES 3.0 feature transform feedback.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_particle_renderer_h
#define s2d_particle_renderer_h

#include "s2d_common.h"
#include "s2d_renderer.h"

NS_S2D

class particle_renderer : public renderer {
public:
    particle_renderer();

    void init() override;
    void shutdown() override;
    void flush() override;
};

NS_S2D_END

#endif
