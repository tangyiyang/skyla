//
//  s2d_particle.cpp
//  seal2d
//
//  Created by yiyang.tang on 2017/10/30.
//

#include "s2d_particle.h"

NS_S2D

void emmiter_property::init(EMMITER_MODE mode, int total_particles)
{
    S2D_ASSERT(total_particles >= 0);

    x = (float*)malloc(total_particles * sizeof(float));
    y = (float*)malloc(total_particles * sizeof(float));
    start_x = (float*)malloc(total_particles * sizeof(float));
    start_y = (float*)malloc(total_particles * sizeof(float));

    r = (float*)malloc(total_particles * sizeof(float));
    g = (float*)malloc(total_particles * sizeof(float));
    b = (float*)malloc(total_particles * sizeof(float));
    a = (float*)malloc(total_particles * sizeof(float));
    
    delta_r = (float*)malloc(total_particles * sizeof(float));
    delta_g = (float*)malloc(total_particles * sizeof(float));
    delta_b = (float*)malloc(total_particles * sizeof(float));
    delta_a = (float*)malloc(total_particles * sizeof(float));

    size = (float*)malloc(total_particles * sizeof(float));
    delta_size = (float*)malloc(total_particles * sizeof(float));
    rotation = (float*)malloc(total_particles * sizeof(float));
    delta_rotation = (float*)malloc(total_particles * sizeof(float));

    ttl = (float*)malloc(total_particles * sizeof(float));
}

void emmiter_property::shutdown()
{

}

particle::particle()
{
    _initialized = false;
}

particle::~particle()
{
    if (_initialized) {
        _emitter_data.shutdown();
    }
}

void particle::init(const char* file_path)
{
    if (_initialized) {
        return;
    }
    _initialized = true;
}

void particle::update(float dt)
{

}

NS_S2D_END
