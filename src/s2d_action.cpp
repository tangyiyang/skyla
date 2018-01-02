#include "s2d_context.h"
#include "s2d_action.h"

NS_S2D

void action_mgr::init()
{
    _actions.clear();
    _target_action_map.clear();
}

void action_mgr::add(action* act)
{
    _actions.push_back(act);
    
    _target_action_map[act->_target].push_back(act);
}

void action_mgr::update(float dt)
{
    std::vector<action*>::iterator it = _actions.begin();
    for (; it != _actions.end(); ) {
        bool running = (*it)->update(dt);
        if (!running) {
            delete (*it);
            it = _actions.erase(it);
        } else {
            ++it;
        }
    }
}

void action_mgr::remove(action* act)
{
    std::vector<action*>::iterator end = std::remove(_actions.begin(), _actions.end(), act);
    _actions.erase(end, _actions.end());
    delete act;
}

void action_mgr::remove(node* target)
{
    std::map<node*, std::vector<action*> >::iterator it = _target_action_map.find(target);
    if (it != _target_action_map.end()) {
        std::vector<action*>& actions = _target_action_map[target];
        for (std::vector<action*>::iterator it = _actions.begin(); it != _actions.end(); ++it){
            this->remove(*it);
        }
        actions.clear();
        _target_action_map.erase(target);
    }
}

void action_mgr::shutdown()
{
    std::vector<action*>::iterator it = _actions.begin();
    for (; it != _actions.end(); ++it) {
        delete (*it);
    }
    
    _actions.clear();
}

void action::init(float duration)
{
    _duration = duration;
    _now = 0.0f;
    _target = nullptr;
    _running = false;
    _step = 0.0f;
}

void action::start(node* target)
{
    _target = target;
    _running = true;
    context::C()->_action_mgr->add(this);
}

void action::update_step(float dt)
{
    _now += dt;
    if (_now >= _duration) {
        _step = 1.0f;
        _running = false;
    } else {
        _step = _now / _duration;
    }
}

void action::stop()
{
    _running = false;
}

void action_move::init(float duration, float x, float y)
{
    action::init(duration);
    _end_pos.x = x;
    _end_pos.y = y;
}

void action_move::start(s2d::node *target)
{
    action::start(target);

    _start_pos = target->get_pos();
    _delta.x = _end_pos.x - target->get_pos().x;
    _delta.y = _end_pos.y - target->get_pos().y;
}

bool action_move::update(float dt)
{
    action::update_step(dt);
    
    _target->set_pos(_start_pos.x + _delta.x * _step, _start_pos.y + _delta.y * _step);
    return _running;
}

void action_scale_to::init(float duration, float scale_x, float scale_y)
{
    action::init(duration);

    _end_scale.x = scale_x;
    _end_scale.y = scale_y;
}

void action_scale_to::start(node* target)
{
    action::start(target);
    
    _start_scale = target->get_scale();
    _delta.x = _end_scale.x - target->get_scale().x;
    _delta.y = _end_scale.y - target->get_scale().y;
}

bool action_scale_to::update(float dt)
{
    action::update_step(dt);
    
    _target->set_scale(_start_scale.x + _delta.x * _step, _start_scale.y + _delta.y * _step);
    return _running;
}

NS_S2D_END
