/*
 * Particle emmiter system, referenced from cocos2d-x.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_particle_h
#define skyla_particle_h

#include "node.h"

#include "renderer/quad_renderer.h"

NS_SKYLA

struct emmiter_property {
    enum EMMITER_TYPE {
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

    void copy(int to, int from)
    {
        x[to] = x[from];
        y[to] = y[from];
        start_x[to] = start_x[from];
        start_y[to] = start_y[from];
        r[to] = r[from];
        g[to] = g[from];
        b[to] = b[from];
        a[to] = a[from];
        delta_r[to] = delta_r[from];
        delta_g[to] = delta_g[from];
        delta_b[to] = delta_b[from];
        delta_a[to] = delta_a[from];
        size[to] = size[from];
        delta_size[to] = delta_size[from];
        rotation[to] = rotation[from];
        delta_rotation[to] = delta_rotation[from];
        ttl[to] = ttl[from];

        if (_mode_info._mode == GRAVITY) {
            _mode_info._data._gravity.dir_x[to] = _mode_info._data._gravity.dir_x[from];
            _mode_info._data._gravity.dir_y[to] = _mode_info._data._gravity.dir_y[from];
            _mode_info._data._gravity.radial_accel[to] = _mode_info._data._gravity.radial_accel[from];
            _mode_info._data._gravity.tangential_accel[to] = _mode_info._data._gravity.tangential_accel[from];
        } else if (_mode_info._mode == RADIDUS) {
            _mode_info._data._radius.angle[to] = _mode_info._data._radius.angle[from];
            _mode_info._data._radius.degrees_per_second[to] = _mode_info._data._radius.degrees_per_second[from];
            _mode_info._data._radius.radius[to] = _mode_info._data._radius.radius[from];
            _mode_info._data._radius.delta_radius[to] = _mode_info._data._radius.delta_radius[from];
        } else {
            LOGE("invalid particle mode");
            SKYLA_ASSERT(false);
        }
    }


public:
    void init(EMMITER_TYPE mode, int total_particles);
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
    bool update(float dt) override;
    void draw(render_state* rs) override;
    void start();
    void stop();
    void reset();

private:
    void load_particle_settings(const char* file_path);
    void init_vertices();
    void emit(int n);
    bool update_life(float dt);

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
    float _duration;
    int   _emmiter_type;
    int   _y_coord_flip;

    /* srt settings */
    float _rotate_per_second;
    float _rotate_per_second_var;
    float _max_radius;
    float _max_radius_var;
    float _min_radius;
    float _min_radius_var;
    bool _absolute_position;

    /* gravity mode settings */
    vec2  _gravity;
    float _radial_accel;
    float _radial_accel_var;
    float _tangential_accel;
    float _tangential_accel_var;
    bool  _rotation_is_dir;

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
    blend_func  _blend;

    /* emmiter paramters, those vars may change duraring the update process. */
    bool _active;
    int _num_particles;
    float _time_elapsed;
    float _emission_rate;
    float _emmit_counter;

public:
    /* renderering stuff */
    /* TODO: we shoud implement this with `particle renderer`.*/
    pos_tex_color_vertex*   _vertices;
    int                     _num_vertices;
    texture*                _texture;
    blend_mode              _blend_mode;
};

NS_SKYLA_END

#endif
