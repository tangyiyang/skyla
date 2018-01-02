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
            it = _actions.erase(it);
            delete (*it);
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
    _start_pos = target->get_pos();
    _delta.x = _end_pos.x - target->get_pos().x;
    _delta.y = _end_pos.y - target->get_pos().y;
    
    _running = true;
    _target = target;
    
    context::C()->_action_mgr->add(this);
}

bool action_move::update(float dt)
{
    float step = 0.0f;
    
    _now += dt;
    if (_now >= _duration) {
        step = 1.0f;
        _running = false;
    } else {
        step = _now / _duration;
    }
    _target->set_pos(_start_pos.x + _delta.x * step, _start_pos.y + _delta.y * step);
    return _running;
}

NS_S2D_END
