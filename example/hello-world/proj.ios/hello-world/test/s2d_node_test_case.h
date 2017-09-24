//
//  Created by yiyang.tang on 2017/9/23.
//

#ifndef s2d_node_test_case_h
#define s2d_node_test_case_h

#include "s2d.h"

using namespace s2d;

class node_test {
public:
    static void setup();
    static void test_node_addchild_with_zorder();
    static void tear_down();

    static context* _ctx;
};


#endif /* s2d_node_test_case_h */
