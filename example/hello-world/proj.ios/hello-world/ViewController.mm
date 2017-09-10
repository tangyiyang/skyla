//
//  ViewController.m
//  hello-world
//
//  Created by Hong Shan on 2017/9/10.
//  Copyright © 2017年 levelmax. All rights reserved.
//


#import "ViewController.h"
#import <OpenGLES/ES2/glext.h>

#include "s2d_sprite.h"

@interface ViewController () {

}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

static s2d::sprite* _spr;

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
        return;
    }

    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    [self setupGL];

    _spr = new s2d::sprite();
    _spr->init();
}

- (void)viewDidLayoutSubviews
{
    GLKView* view = (GLKView*)self.view;
    [view bindDrawable];
    
    self.preferredFramesPerSecond = 60.0f;

    CGSize viewSize = [view bounds].size;
    CGFloat scaleFactor = [view contentScaleFactor];
}

- (void)dealloc
{
    [self tearDownGL];

    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;

        [self tearDownGL];

        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

        // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];

}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{


}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    
    CGSize viewSize = [view bounds].size;
    CGFloat scaleFactor = [view contentScaleFactor];


    glViewport(0, 0, viewSize.width * scaleFactor , viewSize.height * scaleFactor);
    _spr->update();
    _spr->draw();
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{

}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{

}


-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

@end

