/*
 * Scissors clipping.
 * Author: yiyang
 * Copyright: see copyright at s2d.h
 */

#ifndef s2d_panel_h
#define s2d_panel_h

#include "s2d_common.h"
#include "s2d_node.h"

NS_S2D

class panel : public node {
public:
    void init(const rect& r);
    void update(float dt) override;

private:
    rect _rect;
};

NS_S2D_END

#endif /* s2d_clipnode_h */
