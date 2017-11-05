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

    struct mode_gravity {
        float* dir_x;
        float* dir_y;
        float* radial_accel;
        float* tangential_accel;
    };
    struct mode_raidus {
        float* angle;
        float* degrees_per_second;
        float* radius;
        float* delta_radius;
    };

    struct mode_info {
        int _mode;
        union {
            mode_gravity _gravity;
            mode_raidus _radius;
        } _data;
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

    mode_info _mode_info;

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
    void load_particle_settings(const char* file_path);

private:
    bool _initialized;
    emmiter_property _emitter_data;

    /* emmiter settings */
    int   _max_particle;
    vec2  _source_pos_var;
    float _angle;
    float _angle_var;
    float _speed;
    float _speed_var;

    /* gravity mode settings */
    vec2  _gravity;
    float _radial_accel;
    float _radial_accel_var;
    float _tangential_accel;
    float _tangential_accel_var;

    /* particle settings */
    float _life_span;
    float _life_span_var;
    float _start_size;
    float _start_size_var;
    float _end_size;
    float _end_size_var;
    float _start_rotation;
    float _start_rotation_var;
    float _end_rotation;
    float _end_rotation_var;

    /* color settings */
    color4f _start_color;
    color4f _start_color_var;
    color4f _end_color;
    color4f _end_color_var;
    blend_func _blend;

    /* texture settings */
    texture* _texture;
};

NS_S2D_END

#endif
