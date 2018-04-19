#include "s2d_spine.h"
#include "s2d_util.h"

NS_S2D

extern "C" {

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path)
{
    texture* tex = util::load_texture(path);
    self->rendererObject = tex;

    // TODO: build the spine texture into sprite frame, this would benifit memory.
}

void _spAtlasPage_disposeTexture (spAtlasPage* self)
{
    texture* tex = (texture*)self->rendererObject;
    tex->release();
}

char* _spUtil_readFile (const char* path, int* length)
{
    file_entry* f = util::load_file(path, false);
    *length = (int)f->_size;
    char* ret = (char*)f->take();
    f->release();
    return ret;
}

}

spine_anim::spine_anim()
{
    _skeleton = nullptr;
    _attachment_loader = nullptr;
    _state = nullptr;
}

spine_anim::~spine_anim()
{
    if (_skeleton) {
        spSkeleton_dispose(_skeleton);
    }
    if (_state) {
        spAnimationState_dispose(_state);
    }
}

void spine_anim::sp_anim_cb(spAnimationState* state, spEventType type, spTrackEntry* entry, spEvent* event)
{
    spine_anim* self = (spine_anim*)state->rendererObject;
    self->on_amim_event(type, entry, event);
}

void spine_anim::on_amim_event(spEventType type, spTrackEntry* entry, spEvent* event)
{

}

void spine_anim::set_anim(const char* name, int track, int loop)
{
    spAnimation* animation = spSkeletonData_findAnimation(_skeleton->data, name);
    if (!animation) {
        LOGE("spine_anim::set_anim, not found animation for name = %s", name);
        return;
    }
    spAnimationState_setAnimation(_state, track, animation, loop);
}

void spine_anim::init(const char* skeleton_file, const char* atlas_file)
{
    node::init();

    spAtlas* atlas = spAtlas_createFromFile(atlas_file, NULL);
    spAtlasAttachmentLoader* attachment_loader =  spAtlasAttachmentLoader_create(atlas);
    spSkeletonJson* json = spSkeletonJson_createWithLoader((spAttachmentLoader*)attachment_loader);
    spSkeletonData* data = spSkeletonJson_readSkeletonDataFile(json, skeleton_file);

    _skeleton = spSkeleton_create(data);
    if (!_skeleton) {
        LOGE("error create skeleton: %s, %s", skeleton_file, atlas_file);
        return;
    }
    _state = spAnimationState_create(spAnimationStateData_create(_skeleton->data));
    _state->rendererObject = this;
    _state->listener = spine_anim::sp_anim_cb;

    spAttachmentLoader_dispose((spAttachmentLoader*)attachment_loader);
    spSkeletonJson_dispose(json);
}

bool spine_anim::update(float dt)
{
    if (!_skeleton) {
        return false;
    }

    if (_state) {
        spSkeleton_update(_skeleton, dt);

        
        spAnimationState_update(_state, dt);
        spAnimationState_apply(_state, _skeleton);
        spSkeleton_updateWorldTransform(_skeleton);
    }

    return node::update(dt);
}

void spine_anim::draw(render_state* rs)
{
    float buffer[8] = {0};
    pos_tex_color_vertex quad[4];
    float r, g, b, a;
    float* uv = nullptr;
    float multiplier;
    spAtlasRegion* region;

    for (int i = 0, n = _skeleton->slotsCount; i < n; ++i) {
        spSlot* slot = _skeleton->drawOrder[i];
        if (!slot->attachment) {
            continue;
        }

        blend_mode mode = BLEND_MODE_NONE;
        switch (slot->data->blendMode) {
            case SP_BLEND_MODE_NORMAL:
                mode = BLEND_MODE_NORMAL;
                break;
            case SP_BLEND_MODE_ADDITIVE:
                mode = BLEND_MODE_ADDTIVE;
                break;
            case SP_BLEND_MODE_MULTIPLY:
                mode = BLEND_MODE_MUTIPLY;
                break;
            case SP_BLEND_MODE_SCREEN:
                mode = BLEND_MODE_SCREEN;
                break;
            default:
                break;
        }

        switch (slot->attachment->type) {
            case SP_ATTACHMENT_REGION: {
                spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
                spRegionAttachment_computeWorldVertices(attachment, slot->bone, buffer, 0, 2);

                region = (spAtlasRegion*)attachment->rendererObject;
                a = attachment->color.a * _skeleton->color.a * slot->color.a;
                multiplier = ((texture*)region->page->rendererObject)->_premultiply_alpha ? a : 255;
                r = attachment->color.r * _skeleton->color.r * slot->color.r * multiplier;
                g = attachment->color.g * _skeleton->color.g * slot->color.g * multiplier;
                b = attachment->color.b * _skeleton->color.b * slot->color.b * multiplier;

                uv = attachment->uvs;
                region = (spAtlasRegion*)attachment->rendererObject;

                /*Coord order of spine: BR, BL, UL, UR*/
                /*Coord order of ours:  UL, BL, UR, BR*/
                quad[0].pos.x = buffer[2*2+0];
                quad[0].pos.y = buffer[2*2+1];
                quad[0].color = COLOR4F_TO_UINT32(r, g, b, a);
                quad[0].uv.u = uv[2*2+0] * ((1<<16)-1);
                quad[0].uv.v = uv[2*2+1] * ((1<<16)-1);

                quad[1].pos.x = buffer[1*2+0];
                quad[1].pos.y = buffer[1*2+1];
                quad[1].color = COLOR4F_TO_UINT32(r, g, b, a);
                quad[1].uv.u = uv[1*2+0] * ((1<<16)-1);
                quad[1].uv.v = uv[1*2+1] * ((1<<16)-1);

                quad[2].pos.x = buffer[3*2+0];
                quad[2].pos.y = buffer[3*2+1];
                quad[2].color = COLOR4F_TO_UINT32(r, g, b, a);
                quad[2].uv.u = uv[3*2+0] * ((1<<16)-1);
                quad[2].uv.v = uv[3*2+1] * ((1<<16)-1);

                quad[3].pos.x = buffer[0*2+0];
                quad[3].pos.y = buffer[0*2+1];
                quad[3].color = COLOR4F_TO_UINT32(r, g, b, a);
                quad[3].uv.u = uv[0*2+0] * ((1<<16)-1);
                quad[3].uv.v = uv[0*2+1] * ((1<<16)-1);

                rs->draw_quad(_local_transform, (texture*)region->page->rendererObject, mode, quad);

                break;
            }
            case SP_ATTACHMENT_MESH: {
                // TODO: support later
                LOGE("mesh not supported for spine yet.");
                break;
            }
            default:
                continue;
        }
    }
    
    node::draw(rs);
}

void spine_anim::hit_test(touch_handler* handler, touch_event* event)
{

}

void spine_anim::on_touch(touch_event* event)
{

}

NS_S2D_END
