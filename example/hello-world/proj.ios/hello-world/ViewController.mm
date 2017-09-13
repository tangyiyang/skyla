//
//  ViewController.m
//  hello-world
//
//  Created by Hong Shan on 2017/9/10.
//  Copyright © 2017年 levelmax. All rights reserved.
//


#import "ViewController.h"
#import <OpenGLES/ES2/glext.h>

#include "s2d.h"

@interface ViewController () {

}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

@property s2d::s2d_context *game_context;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController



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

    self.game_context = new s2d::s2d_context();
}

- (void)viewDidLayoutSubviews
{
    GLKView* view = (GLKView*)self.view;
    [view bindDrawable];
    
    self.preferredFramesPerSecond = 60.0f;

    CGSize viewSize = [view bounds].size;
    CGFloat scaleFactor = [view contentScaleFactor];

    self.game_context->init(viewSize.width * scaleFactor, viewSize.height * scaleFactor);

}

- (void)dealloc
{
    [self tearDownGL];

    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }

    self.game_context->shutdown();
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
    self.game_context->update();

}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    
    CGSize viewSize = [view bounds].size;
    CGFloat scaleFactor = [view contentScaleFactor];
    glViewport(0, 0, viewSize.width * scaleFactor , viewSize.height * scaleFactor);
    
    self.game_context->draw();
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

