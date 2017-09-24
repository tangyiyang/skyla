//
//  s2d_node_test.m
//  hello-world
//
//  Created by yiyang.tang on 2017/9/23.
//  Copyright © 2017年 levelmax. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "s2d_node_test_case.h"

@interface s2d_node_test : XCTestCase

@end

@implementation s2d_node_test



- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    node_test::setup();
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
    node_test::tear_down();
}

- (void)testExample {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
    node_test::test_node_addchild_with_zorder();
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
