//
//  Created by yiyang.tang on 2017/9/23.
//

#include "s2d_node_test_case.h"

using namespace s2d;
void node_test::setup()
{
    _ctx = new context();
    _ctx->init(1136, 640);
}

void node_test::tear_down()
{
    _ctx->shutdown();
    delete _ctx;
}

void node_test::test_node_addchild_with_zorder()
{
    
}


