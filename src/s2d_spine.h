/*
 * Spine is the right choice for skeleton animation. :)
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_spine_h
#define s2d_spine_h

#ifdef S2D_ENABLE_SPINE

#include "spine/AtlasAttachmentLoader.h"
#include "spine/extension.h"
#include "spine/spine.h"

#include "s2d_common.h"
#include "s2d_node.h"
#include "s2d_quad_renderer.h"

NS_S2D

typedef std::function<void(spEventType type, spTrackEntry* entry, spEvent* event)> anim_callback_t;

class spine_anim : public node {
public:
    spine_anim();
    virtual ~spine_anim();

private:
    static void sp_anim_cb(spAnimationState* state, spEventType type, spTrackEntry* entry, spEvent* event);
    void on_amim_event(spEventType type, spTrackEntry* entry, spEvent* event);

public:
    inline void set_callback(anim_callback_t callback)
    {
        _callback = callback;
    }

    void set_anim(const char* name, int track, int loop);

    void init(const char* skeleton_file, const char* atlas_file);
    bool update(float dt) override;
    void draw(render_state* rs) override;
    void hit_test(touch_handler* handler, touch_event* event) override;
    void on_touch(touch_event* event) override;
    rect bounds_in(node* space) override;

private:
    spAnimationState*    _state;
    spSkeleton*          _skeleton;
    spAttachmentLoader*  _attachment_loader;
    anim_callback_t      _callback;
};

NS_S2D_END

#endif /* S2D_ENABLE_SPINE */

#endif
