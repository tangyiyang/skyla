#include "cJSON.h"
#include "s2d_util.h"
#include "s2d_particle.h"

NS_S2D

/* color setting macros*/
#define SET_COLOR(c, b, v)\
for (int i = start; i < _num_particles; ++i)\
{\
c[i] = math::clampf( b + v * util::normalized_random(), 0.0f, 1.0f);\
}

#define SET_DELTA_COLOR(c, dc)\
for (int i = start; i < _num_particles; ++i)\
{\
dc[i] = (dc[i] - c[i]) / _emitter_data.ttl[i];\
}


void emmiter_property::init(EMMITER_TYPE mode, int total_particles)
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

    ttl = (float*)calloc(total_particles, sizeof(float));

    _mode_info._mode = mode;
    if (mode == GRAVITY) {
        _mode_info._data._gravity.dir_x = (float*)malloc(total_particles * sizeof(float));
        _mode_info._data._gravity.dir_y = (float*)malloc(total_particles * sizeof(float));
        _mode_info._data._gravity.radial_accel = (float*)malloc(total_particles * sizeof(float));
        _mode_info._data._gravity.tangential_accel = (float*)malloc(total_particles * sizeof(float));
    } else if (mode == RADIDUS) {
        _mode_info._data._radius.angle = (float*)malloc(total_particles * sizeof(float));
        _mode_info._data._radius.degrees_per_second = (float*)malloc(total_particles * sizeof(float));
        _mode_info._data._radius.radius = (float*)malloc(total_particles * sizeof(float));
        _mode_info._data._radius.delta_radius = (float*)malloc(total_particles * sizeof(float));
    } else {
        LOGE("invalid particle mode");
        S2D_ASSERT(false);
    }
}

void emmiter_property::shutdown()
{
    free(x);
    free(y);
    free(start_x);
    free(start_y);

    free(r);
    free(g);
    free(b);
    free(a);

    free(delta_r);
    free(delta_g);
    free(delta_b);
    free(delta_a);

    free(size);
    free(delta_size);
    free(rotation);
    free(delta_rotation);
    free(ttl);

    if (_mode_info._mode == GRAVITY) {
        free(_mode_info._data._gravity.dir_x);
        free(_mode_info._data._gravity.dir_y);
        free(_mode_info._data._gravity.radial_accel);
        free(_mode_info._data._gravity.tangential_accel);
    } else if (_mode_info._mode == RADIDUS) {
        free(_mode_info._data._radius.angle);
        free(_mode_info._data._radius.degrees_per_second);
        free(_mode_info._data._radius.radius);
        free(_mode_info._data._radius.delta_radius);
    } else {
        LOGE("invalid particle mode");
        S2D_ASSERT(false);
    }
}

particle::particle()
{
    _initialized = false;
    _texture = nullptr;
    _vertices = nullptr;
    _num_particles = 0;
}

particle::~particle()
{
    if (_initialized) {
        _emitter_data.shutdown();
        _texture->release();
        free(_vertices);
    }
}

void particle::init(const char* file_path)
{
    if (_initialized) {
        return;
    }
    _initialized = true;

    node::init();

    this->load_particle_settings(file_path);
    this->init_vertices();
}

void particle::init_vertices()
{
    /* temporary solution */
    _num_vertices = 0;
    _vertices = (pos_tex_color_vertex*)malloc(4 * _max_particle * sizeof(pos_tex_color_vertex));
}

void particle::load_particle_settings(const char *file_path)
{
    file_entry* file = util::load_file(file_path, true);
    cJSON* root = cJSON_Parse((const char*)file->_buffer);
    S2D_ASSERT(root);

    /* load the emmiter settings */
    _max_particle = cJSON_GetObjectItemCaseSensitive(root, "maxParticles")->valueint;
    _source_pos_var.x = cJSON_GetObjectItemCaseSensitive(root, "sourcePositionVariancex")->valuedouble;
    _source_pos_var.y = cJSON_GetObjectItemCaseSensitive(root, "sourcePositionVariancey")->valuedouble;
    _angle = cJSON_GetObjectItemCaseSensitive(root, "angle")->valuedouble;
    _angle_var = cJSON_GetObjectItemCaseSensitive(root, "angleVariance")->valuedouble;
    _speed = cJSON_GetObjectItemCaseSensitive(root, "speed")->valuedouble;
    _speed_var = cJSON_GetObjectItemCaseSensitive(root, "speedVariance")->valuedouble;
    _absolute_position = cJSON_GetObjectItemCaseSensitive(root, "absolutePosition")->valueint;
    _duration = cJSON_GetObjectItemCaseSensitive(root, "duration")->valuedouble;
    _y_coord_flip = cJSON_GetObjectItemCaseSensitive(root, "yCoordFlipped")->valueint;

    /* load the particle settings */
    _life_span = cJSON_GetObjectItemCaseSensitive(root, "particleLifespan")->valuedouble;
    _life_span_var = cJSON_GetObjectItemCaseSensitive(root, "particleLifespanVariance")->valuedouble;;
    _start_size = cJSON_GetObjectItemCaseSensitive(root, "startParticleSize")->valuedouble;
    _start_size_var = cJSON_GetObjectItemCaseSensitive(root, "startParticleSizeVariance")->valuedouble;
    _end_size = cJSON_GetObjectItemCaseSensitive(root, "finishParticleSize")->valuedouble;
    _end_size_var = cJSON_GetObjectItemCaseSensitive(root, "finishParticleSizeVariance")->valuedouble;

    _start_color.r = cJSON_GetObjectItemCaseSensitive(root, "startColorRed")->valuedouble;
    _start_color.g = cJSON_GetObjectItemCaseSensitive(root, "startColorGreen")->valuedouble;
    _start_color.b = cJSON_GetObjectItemCaseSensitive(root, "startColorBlue")->valuedouble;
    _start_color.a = cJSON_GetObjectItemCaseSensitive(root, "startColorAlpha")->valuedouble;

    _start_color_var.r = cJSON_GetObjectItemCaseSensitive(root, "startColorVarianceRed")->valuedouble;
    _start_color_var.g = cJSON_GetObjectItemCaseSensitive(root, "startColorVarianceGreen")->valuedouble;
    _start_color_var.b = cJSON_GetObjectItemCaseSensitive(root, "startColorVarianceBlue")->valuedouble;
    _start_color_var.a = cJSON_GetObjectItemCaseSensitive(root, "startColorVarianceAlpha")->valuedouble;

    _end_color.r = cJSON_GetObjectItemCaseSensitive(root, "finishColorRed")->valuedouble;
    _end_color.g = cJSON_GetObjectItemCaseSensitive(root, "finishColorGreen")->valuedouble;
    _end_color.b = cJSON_GetObjectItemCaseSensitive(root, "finishColorBlue")->valuedouble;
    _end_color.a = cJSON_GetObjectItemCaseSensitive(root, "finishColorAlpha")->valuedouble;

    _end_color_var.r = cJSON_GetObjectItemCaseSensitive(root, "finishColorVarianceRed")->valuedouble;
    _end_color_var.g = cJSON_GetObjectItemCaseSensitive(root, "finishColorVarianceGreen")->valuedouble;
    _end_color_var.b = cJSON_GetObjectItemCaseSensitive(root, "finishColorVarianceBlue")->valuedouble;
    _end_color_var.a = cJSON_GetObjectItemCaseSensitive(root, "finishColorVarianceAlpha")->valuedouble;

    // TODO: correctly parse these parameters
    _blend.src = cJSON_GetObjectItemCaseSensitive(root, "blendFuncSource")->valueint;
    _blend.dst = cJSON_GetObjectItemCaseSensitive(root, "blendFuncDestination")->valueint;

    _start_rotation = cJSON_GetObjectItemCaseSensitive(root, "rotationStart")->valuedouble;
    _start_rotation_var = cJSON_GetObjectItemCaseSensitive(root, "rotationStartVariance")->valuedouble;
    _end_rotation = cJSON_GetObjectItemCaseSensitive(root, "rotationEnd")->valuedouble;
    _end_rotation_var = cJSON_GetObjectItemCaseSensitive(root, "rotationEndVariance")->valuedouble;

    _emmiter_type = cJSON_GetObjectItemCaseSensitive(root, "emitterType")->valueint;
    if (_emmiter_type == emmiter_property::GRAVITY) {
        /* load the gravity mode settings */
        _gravity.x = cJSON_GetObjectItemCaseSensitive(root, "gravityx")->valuedouble;
        _gravity.y = cJSON_GetObjectItemCaseSensitive(root, "gravityy")->valuedouble;
        _radial_accel = cJSON_GetObjectItemCaseSensitive(root, "radialAcceleration")->valuedouble;
        _radial_accel_var = cJSON_GetObjectItemCaseSensitive(root, "radialAccelVariance")->valuedouble;
        _tangential_accel = cJSON_GetObjectItemCaseSensitive(root, "tangentialAcceleration")->valuedouble;
        _tangential_accel_var = cJSON_GetObjectItemCaseSensitive(root, "tangentialAccelVariance")->valuedouble;

        if (cJSON_HasObjectItem(root, "rotationIsDir")) {
            _rotation_is_dir = cJSON_GetObjectItem(root, "rotationIsDir")->valueint;
        } else {
            _rotation_is_dir = false;
        }
    } else if (_emmiter_type == emmiter_property::RADIDUS) {
        /* load the radial mode settings */
        _rotate_per_second = cJSON_GetObjectItemCaseSensitive(root, "rotatePerSecondVariance")->valuedouble;
        _rotate_per_second_var = cJSON_GetObjectItemCaseSensitive(root, "rotatePerSecondVariance")->valuedouble;
        _max_radius = cJSON_GetObjectItemCaseSensitive(root, "maxRadius")->valuedouble;
        _max_radius_var = cJSON_GetObjectItemCaseSensitive(root, "maxRadiusVariance")->valuedouble;
        _min_radius = cJSON_GetObjectItemCaseSensitive(root, "minRadius")->valuedouble;

        /* this looks ugly due to the format doesn't export the minRadiusVariance.
         * TODO: the particle design doesn't seems so cool, we should build one ourselves.
         * It's not hard to add an particle editor in our game tool.
         * refactor this some day, you would do it, will you.? :)
         */
        if (cJSON_HasObjectItem(root, "minRadiusVariance")) {
            _min_radius_var = cJSON_GetObjectItemCaseSensitive(root, "minRadiusVariance")->valuedouble;
        } else {
            _min_radius_var = 0.0f;
        }
    } else {
        LOGE("invalid emmiter type: %d", _emmiter_type);
        S2D_ASSERT(false);
    }

    /*
     * load the texture data
     * we don't support the compressed image right now.
     */

    if (cJSON_HasObjectItem(root, "textureFileName")) {
        util::insert_search_path(file->_path.c_str());

        const char* name = cJSON_GetObjectItemCaseSensitive(root, "textureFileName")->valuestring;
        texture* tex = new texture();
        tex->init(name);
        _texture = tex;
    }

    cJSON_Delete(root);

    this->_emitter_data.init((emmiter_property::EMMITER_TYPE)_emmiter_type, _max_particle);

    _active = true;
    _time_elapsed = 0.0f;
    _num_particles = 0;
    _emmit_counter = 0.0f;
    _emission_rate = _max_particle / _life_span; /* particle emmit per second. */

    this->reset();
}

void particle::start()
{
    this->reset();
}

void particle::reset()
{
    _active = true;
    _time_elapsed = 0.0f;
    for (int i = 0; i < _max_particle; ++i) {
        _emitter_data.ttl[i] = 0.0f;
    }
}

void particle::stop()
{
    _active = false;
}

bool particle::update(float dt)
{
    node::update(dt);
    _time_elapsed += dt;

    /* first we test if the particle is dead.*/
    if ( (!FLT_EQUAL(_duration, -1.0f)) && _time_elapsed > _duration) {
        this->stop();
        return false;
    }

    if (_active) {
        _emmit_counter += dt;

        /* emit serveral particles */
        int n = std::min(_max_particle - _num_particles, (int)(_emmit_counter * _emission_rate));
        if (n > 0) {
            this->emit(n);
            _emmit_counter -= ((float)n) / _emission_rate;
        }

//        LOGD("Emit: "
//             "dt = %.2f, _emission_rate = %.2f, "
//             "_max_particle = %d, n = %d, "
//             "_num_particles = %d, _emmit_counter = %.2f, "
//             " lose = %.2f",
//             dt,
//             _emission_rate,
//             _max_particle,
//             n,
//             _num_particles,
//             _emmit_counter,
//             ((float)n) / _emission_rate);
    }

    /* check their lives */
    this->update_life(dt);

    /* if there were someby alive, we update these lucky girls.*/
    if (_emmiter_type == emmiter_property::GRAVITY)
    {
        for (int i = 0 ; i < _num_particles; ++i)
        {
            vec2 tmp, radial, tangential;
            float radial_accel = _emitter_data._mode_info._data._gravity.radial_accel[i];
            float tangential_accel = _emitter_data._mode_info._data._gravity.tangential_accel[i];

            vec2::normalize(_emitter_data.x[i], _emitter_data.y[i], radial);

            radial.x *= radial_accel;
            radial.y *= radial_accel;

            tangential = radial;
            std::swap(tangential.x, tangential.y);
            tangential.x *= -tangential_accel;
            tangential.y *= tangential_accel;

            // (gravity + radial + tangential) * dt
            tmp.x = radial.x + tangential.x + _gravity.x;
            tmp.y = radial.y + tangential.y + _gravity.y;

            tmp.x *= dt;
            tmp.y *= dt;

            _emitter_data._mode_info._data._gravity.dir_x[i] += tmp.x;
            _emitter_data._mode_info._data._gravity.dir_y[i] += tmp.y;

            tmp.x = _emitter_data._mode_info._data._gravity.dir_x[i] * dt * _y_coord_flip;
            tmp.y = _emitter_data._mode_info._data._gravity.dir_y[i] * dt * _y_coord_flip;

            _emitter_data.x[i] += tmp.x;
            _emitter_data.y[i] += tmp.y;
        }
    } else if (_emmiter_type == emmiter_property::RADIDUS) {

    }

    //color r,g,b,a
    for (int i = 0 ; i < _num_particles; ++i) {
        _emitter_data.r[i] += _emitter_data.delta_r[i] * dt;
    }

    for (int i = 0 ; i < _num_particles; ++i) {
        _emitter_data.g[i] += _emitter_data.delta_g[i] * dt;
    }

    for (int i = 0 ; i < _num_particles; ++i) {
        _emitter_data.b[i] += _emitter_data.delta_b[i] * dt;
    }

    for (int i = 0 ; i < _num_particles; ++i) {
        _emitter_data.a[i] += _emitter_data.a[i] * dt;
    }

    for (int i = 0 ; i < _num_particles; ++i) {
        _emitter_data.size[i] += (_emitter_data.delta_size[i] * dt);
        _emitter_data.size[i] = std::max(0.0f, _emitter_data.size[i]);
    }

    for (int i = 0 ; i < _num_particles; ++i) {
        _emitter_data.rotation[i] += _emitter_data.delta_rotation[i] * dt;
    }

    return false;
}

void particle::draw(render_state* rs)
{
    _model_view = transform_to(this->get_root());

    for (int i = 0, j = 0; i < _num_particles; ++i, j += 4) {
        float x = _emitter_data.x[i];
        float y = _emitter_data.y[i];
        float rotation = _emitter_data.rotation[i];
        float size = _emitter_data.size[i];
        float red = _emitter_data.r[i];
        float green = _emitter_data.g[i];
        float blue = _emitter_data.b[i];
        float alpha = _emitter_data.a[i];
        uint32_t color = (((int)(red*255))<<24) + (((int)(green*255))<<16) + (((int)(blue*255))<<8) + ((int)alpha);

        pos_tex_color_vertex* v = _vertices + j;

        GLfloat size_2 = size/2;
        GLfloat x1 = -size_2;
        GLfloat y1 = -size_2;

        GLfloat x2 = size_2;
        GLfloat y2 = size_2;

        GLfloat r = -(rotation*PI/180.0f);
        GLfloat cr = cosf(r);
        GLfloat sr = sinf(r);
        GLfloat ax = x1 * cr - y1 * sr + x;
        GLfloat ay = x1 * sr + y1 * cr + y;
        GLfloat bx = x2 * cr - y1 * sr + x;
        GLfloat by = x2 * sr + y1 * cr + y;
        GLfloat cx = x2 * cr - y2 * sr + x;
        GLfloat cy = x2 * sr + y2 * cr + y;
        GLfloat dx = x1 * cr - y2 * sr + x;
        GLfloat dy = x1 * sr + y2 * cr + y;

        v[0].pos.x = ax;
        v[0].pos.y = ay;
        v[0].uv.u = 0;
        v[0].uv.v = S2D_TEX_COORD_MAX;
        v[0].color = color;

        v[1].pos.x = bx;
        v[1].pos.y = by;
        v[1].uv.u = S2D_TEX_COORD_MAX;
        v[1].uv.v = S2D_TEX_COORD_MAX;
        v[1].color = color;

        v[2].pos.x = dx;
        v[2].pos.y = dy;
        v[2].uv.u = 0;
        v[2].uv.v = 0;
        v[2].color = color;

        v[3].pos.x = cx;
        v[3].pos.y = cy;
        v[3].uv.u = S2D_TEX_COORD_MAX;
        v[3].uv.v = 0;
        v[3].color = color;

        LOGD("Particle: x = %.2f, y = %.2f", x, y);
        LOGD("Particle: v[%d] = %.2f, %.2f, y /x = %.2f", 0, v[0].pos.x, v[0].pos.y);
    }

    _num_vertices = _num_particles * 4;

    LOGD("_num_vertices = %d", _num_vertices);
    rs->draw_particle(this);

    /* particle shouldn't have any child for most cases.*/
    node::draw(rs);
}


bool particle::update_life(float dt)
{
    for (int i = 0; i < _num_particles; ++i) {
        _emitter_data.ttl[i] -= dt;
    }

    for (int i = 0; i < _num_particles; ++i) {
        if (_emitter_data.ttl[i] <= 0.0f) {
            int j = _num_particles - 1;
            while (j > 0 && _emitter_data.ttl[j] <= 0) {
                --_num_particles;
                --j;
            }
            _emitter_data.copy(i, _num_particles - 1);

            --_num_particles;

            if( _num_particles == 0) {
                this->remove_from_parent();
                return true;
            }
        }
    }

    return false;
}

void particle::emit(int n)
{
    int start = _num_particles;
    _num_particles += n;

    /* life */
    for (int i = start; i < _num_particles; ++i) {
        float life = _life_span + _life_span_var * util::normalized_random();
        _emitter_data.ttl[i] = std::max(life, _life_span);
    }

    /* positions, ralative mode only, we could easily support absolute mode by add node to root. */
    for (int i = start; i < _num_particles; ++i) {
        _emitter_data.x[i] = _source_pos_var.x * util::normalized_random();
    }

    for (int i = start; i < _num_particles; ++i) {
        _emitter_data.y[i] = _source_pos_var.y * util::normalized_random();
    }

    for (int i = start; i < _num_particles; ++i)
    {
        _emitter_data.start_x[i] = _pos.x;
    }
    for (int i = start; i < _num_particles; ++i)
    {
        _emitter_data.start_y[i] = _pos.y;
    }

    /* color */
    SET_COLOR(_emitter_data.r, _start_color.r, _start_color_var.r);
    SET_COLOR(_emitter_data.g, _start_color.g, _start_color_var.g);
    SET_COLOR(_emitter_data.b, _start_color.b, _start_color_var.b);
    SET_COLOR(_emitter_data.a, _start_color.a, _start_color_var.a);

    LOGD("Particle: emit, color.r = %.2f, start_color.r = %.2f, start_color_var = %.2f", _emitter_data.r[0], _start_color.r, _start_color_var.r);

    /*
     * TODO: actually, there were two ways to set the delta color, instead using
     * this SOA mode settings, could we have better performance directly caculate
     * the performance by (deleta = (end - start)/life) ?
     */
    SET_COLOR(_emitter_data.delta_r, _end_color.r, _end_color_var.r);
    SET_COLOR(_emitter_data.delta_g, _end_color.g, _end_color_var.g);
    SET_COLOR(_emitter_data.delta_b, _end_color.b, _end_color_var.b);
    SET_COLOR(_emitter_data.delta_a, _end_color.a, _end_color_var.a);

    LOGD("Particle: emit, _emitter_data.delta_r = %.2f, _end_color = %.2f, _end_color_var = %.2f",
         _emitter_data.delta_r[0], _end_color.r, _end_color_var.r);

    SET_DELTA_COLOR(_emitter_data.r, _emitter_data.delta_r);
    SET_DELTA_COLOR(_emitter_data.g, _emitter_data.delta_g);
    SET_DELTA_COLOR(_emitter_data.b, _emitter_data.delta_b);
    SET_DELTA_COLOR(_emitter_data.a, _emitter_data.delta_a);

    LOGD("Particle: emit, _emitter_data.r = %.2f, _emitter_data.delta_r = %.2f", _emitter_data.r[0], _emitter_data.delta_r[0]);

    /* size */
    for (int i = start; i < _num_particles; ++i) {
        _emitter_data.size[i] = _start_size + _start_size_var * util::normalized_random();
        _emitter_data.size[i] = std::max(0.0f, _emitter_data.size[i]);
    }

    if (!FLT_EQUAL(_start_size, _end_size)) {
        for (int i = start; i < _num_particles; ++i) {
            float end_size = _end_size + _end_size_var * util::normalized_random();
            end_size = std::max(0.0f, end_size);
            _emitter_data.delta_size[i] = (end_size - _emitter_data.size[i]) / _emitter_data.ttl[i];
        }
    } else {
        for (int i = start; i < _num_particles; ++i) {
            _emitter_data.delta_size[i] = 0.0f;
        }
    }

    /* rotation */
    for (int i = start; i < _num_particles; ++i) {
        _emitter_data.rotation[i] = _start_rotation + _start_rotation_var * util::normalized_random();
    }
    for (int i = start; i < _num_particles; ++i) {
        float end_rotation = _end_rotation + _end_rotation * util::normalized_random();
        _emitter_data.delta_rotation[i] = (end_rotation - _emitter_data.rotation[i]) / _emitter_data.ttl[i];
    }

    /* position */
    for (int i = start; i < _num_particles; ++i) {
        _emitter_data.start_x[i] = _pos.x;
    }
    for (int i = start; i < _num_particles; ++i) {
        _emitter_data.start_y[i] = _pos.y;
    }

    if (_emitter_data._mode_info._mode == emmiter_property::GRAVITY) {
        float* rc = _emitter_data._mode_info._data._gravity.radial_accel;
        for (int i = start; i < _num_particles; ++i) {
            rc[i] = _radial_accel + _radial_accel_var * util::normalized_random();
        }

        // tangential accel
        float* tc = _emitter_data._mode_info._data._gravity.tangential_accel;
        for (int i = start; i < _num_particles; ++i) {
            tc[i] = _tangential_accel + _tangential_accel_var * util::normalized_random();
        }

        // rotation is dir
        if(_rotation_is_dir) {
            for (int i = start; i < _num_particles; ++i) {
                float a = (_angle + _angle_var * util::normalized_random())*PI/180.0f;

                float cos_a = cosf(a);
                float sin_a = sinf(a);
                float s = _speed + _speed_var * util::normalized_random();

                float dir_x = s * cos_a;
                float dir_y = s * sin_a;

                _emitter_data._mode_info._data._gravity.dir_x[i] = dir_x;
                _emitter_data._mode_info._data._gravity.dir_y[i] = dir_y;

                _emitter_data.rotation[i] = -(atan2(dir_y, dir_x) * 180.0f / PI);
            }
        }
        else
        {
            for (int i = start; i < _num_particles; ++i) {
                float angle = _angle + _angle_var * util::normalized_random();
                float a = (_angle + _angle_var * util::normalized_random()) * PI / 180.0f;

                float cos_a = cosf(a);
                float sin_a = sinf(a);
                float s = _speed + _speed_var * util::normalized_random();

                float dir_x = s * cos_a;
                float dir_y = s * sin_a;

                _emitter_data._mode_info._data._gravity.dir_x[i] = dir_x;
                _emitter_data._mode_info._data._gravity.dir_y[i] = dir_y;

                LOGD("Particle: a = %.2f, angle_in_degree = %.2f, cos_a = %.2f, sin_a = %.2f, s = %.2f, dir_x = %.2f, dir_y = %.2f",
                     a, angle, cos_a, sin_a, s, dir_x, dir_y);
            }
        }
    } else {

    }
}

NS_S2D_END
