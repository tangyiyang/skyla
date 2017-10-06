#ifndef entry_h
#define entry_h

#include "s2d.h"

using namespace s2d;
class entry : public app_protocol {
public:
    void on_init(context* ctx) override;
    void on_pause() override;
    void on_resume() override;
    void on_destroy() override;
    void on_resize(context* ctx) override;
};

#endif /* entry_h */
