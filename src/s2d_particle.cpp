#include "cJSON.h"
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

    node::init();

    this->load_particle_settings(file_path);
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

    /* load the gravity mode settings */
    _gravity.x = cJSON_GetObjectItemCaseSensitive(root, "gravityx")->valuedouble;
    _gravity.y = cJSON_GetObjectItemCaseSensitive(root, "gravityy")->valuedouble;
    _radial_accel = cJSON_GetObjectItemCaseSensitive(root, "radialAcceleration")->valuedouble;
    _radial_accel_var = cJSON_GetObjectItemCaseSensitive(root, "radialAccelVariance")->valuedouble;
    _tangential_accel = cJSON_GetObjectItemCaseSensitive(root, "tangentialAcceleration")->valuedouble;
    _tangential_accel_var = cJSON_GetObjectItemCaseSensitive(root, "tangentialAccelVariance")->valuedouble;

    /* load the particle settings */
    _life_span = cJSON_GetObjectItemCaseSensitive(root, "particleLifespan")->valuedouble;
    _life_span_var = cJSON_GetObjectItemCaseSensitive(root, "particleLifespanVariance")->valuedouble;;
    _start_size = cJSON_GetObjectItemCaseSensitive(root, "startParticleSize")->valuedouble;
    _start_size_var = cJSON_GetObjectItemCaseSensitive(root, "startParticleSizeVariance")->valuedouble;
    _end_size = cJSON_GetObjectItemCaseSensitive(root, "finishParticleSize")->valuedouble;
    _end_size_var = cJSON_GetObjectItemCaseSensitive(root, "finishParticleSizeVariance")->valuedouble;

    _start_rotation = cJSON_GetObjectItemCaseSensitive(root, "rotationStart")->valuedouble;
    _start_rotation_var = cJSON_GetObjectItemCaseSensitive(root, "rotationStartVariance")->valuedouble;
    _end_rotation = cJSON_GetObjectItemCaseSensitive(root, "rotationEnd")->valuedouble;
    _end_rotation_var = cJSON_GetObjectItemCaseSensitive(root, "rotationEndVariance")->valuedouble;

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

    _blend.src = cJSON_GetObjectItemCaseSensitive(root, "blendFuncSource")->valueint;

    cJSON_Delete(root);
}

void particle::update(float dt)
{

}

NS_S2D_END
