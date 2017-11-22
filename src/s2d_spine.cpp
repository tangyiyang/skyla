#include "s2d_spine.h"


NS_S2D

extern "C" {
void _spAtlasPage_createTexture (spAtlasPage* self, const char* path)
{
    texture* tex = util::load_texture(path);
    self->rendererObject = tex;
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
    return (char*)f->_buffer;
}

}

spine_anim::spine_anim()
{
    _skeleton = nullptr;
    _attachment_loader = nullptr;
}
spine_anim::~spine_anim()
{
    if (_skeleton) {
        spSkeleton_dispose(_skeleton);
    }
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
}

bool spine_anim::update(float dt)
{
    if (!_skeleton) {
        return false;
    }

    spSkeleton_update(_skeleton, dt);

    return node::update(dt);
}

void spine_anim::draw(render_state* rs)
{
    float buffer[8] = {0};
    pos_tex_color_vertex quad[4];

    for (int i = 0, n = _skeleton->slotsCount; i < n; ++i) {
        spSlot* slot = _skeleton->drawOrder[i];
        if (!slot->attachment) continue;

        switch (slot->attachment->type) {
            case SP_ATTACHMENT_REGION: {
                spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
                spRegionAttachment_computeWorldVertices(attachment, slot->bone, buffer, 0, 0);
                break;
            }
            case SP_ATTACHMENT_MESH: {
                // TODO:
                break;
            }
            default:
                continue;
        }

        rs->draw_sprite(<#s2d::sprite *s#>)


    }
}

void spine_anim::hit_test(touch_handler* handler, touch_event* event)
{

}

void spine_anim::on_touch(touch_event* event)
{

}

rect spine_anim::bounds_in(node* space)
{
    return node::bounds_in(this);
}

NS_S2D_END
