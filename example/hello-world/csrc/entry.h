#ifndef entry_h
#define entry_h

#include "skyla.h"

using namespace skyla;
class entry : public app_protocol {
public:
    void on_init(context* ctx) override;
    void on_pause() override;
    void on_resume() override;
    void on_destroy() override;
    void on_resize(context* ctx) override;
    void on_begin_update(float dt) override;
    void on_after_update(float dt) override;

private:
    float _time_elapsed;
};

#endif /* entry_h */
