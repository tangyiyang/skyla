//
//  ViewController.h
//  hello-world
//
//  Created by Hong Shan on 2017/9/10.
//  Copyright © 2017年 levelmax. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include "s2d.h"
#include "entry.h"

@interface ViewController : GLKViewController

@property s2d::context* game_context;

@end

