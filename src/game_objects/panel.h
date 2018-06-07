/*
 * Scissors clipping.
 * Author: yiyang
 * Copyright: see copyright at skyla.h
 */

#ifndef skyla_panel_h
#define skyla_panel_h

#include "skyla_common.h"

#include "node.h"

NS_SKYLA

class panel : public node {
public:
    void init(const rect& r);
    void init(float x, float y, float w, float h);
    bool update(float dt) override;
    void draw(render_state* rs) override;

private:
    rect _rect;
};

NS_SKYLA_END

#endif
