/*
 * Spine is the right choice for skeleton animation. :)
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_spine_h
#define skyla_spine_h

#ifdef SKYLA_ENABLE_SPINE

#include "spine/AtlasAttachmentLoader.h"
#include "spine/extension.h"
#include "spine/spine.h"

#include "skyla_common.h"
#include "game_objects/node.h"
#include "renderer/quad_renderer.h"

NS_SKYLA

typedef std::function<void(spEventType type, spTrackEntry* entry, spEvent* event)> anim_callback_t;

class spine_anim : public node {
public:
    spine_anim();
    virtual ~spine_anim();

private:
    static void sp_anim_cb(spAnimationState* state, spEventType type, spTrackEntry* entry, spEvent* event);
    void on_amim_event(spEventType type, spTrackEntry* entry, spEvent* event);

public:

    inline spSkeleton* get_skeleton()
    {
        return _skeleton;
    }

    inline void set_callback(anim_callback_t callback)
    {
        _callback = callback;
    }

    void set_anim(const char* name, int track, int loop);


    void init(const char* skeleton_file, const char* atlas_file);
    bool update(float dt) override;
    void draw(render_state* rs) override;
    void hit_test(touch_handler* handler, touch_event* event) override;

private:
    spAnimationState*    _state;
    spSkeleton*          _skeleton;
    spAttachmentLoader*  _attachment_loader;
    anim_callback_t      _callback;
};

NS_SKYLA_END

#endif /* SKYLA_ENABLE_SPINE */

#endif /* spine_h */
