/*
 * Particle emmiter system, referenced from cocos2d-x, enhanced with transform feedback implementation.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_particle_h
#define s2d_particle_h

#include "s2d_node.h"

NS_S2D

struct emmiter_property {
    enum EMMITER_MODE {
        GRAVITY = 0,
        RADIDUS,
    };
    
    float* x;
    float* y;
    float* start_x;
    float* start_y;

    float* r;
    float* g;
    float* b;
    float* a;

    float* delta_r;
    float* delta_g;
    float* delta_b;
    float* delta_a;

    float* size;
    float* delta_size;
    float* rotation;
    float* delta_rotation;
    float* ttl;

    union mode {
        struct {
            float* dir_x;
            float* dir_y;
            float* radial_accel;
            float* tangential_accel;
        } modeA;

        struct {
            float* angle;
            float* degrees_per_second;
            float* radius;
            float* delta_radius;
        } modeB;
    } mode;

    int _mode;

public:
    void init(EMMITER_MODE mode, int total_particles);
    void shutdown();

};

class particle : public node {
public:
    enum ParticleType {
        PARTICLE_POINT = 0,
        PARTICLE_QUAD
    };

    particle();
    ~particle();

public:
    void init(const char* file_path);
    void update(float dt) override;

private:
    bool _initialized;
    emmiter_property _emitter_data;
};

NS_S2D_END

#endif
