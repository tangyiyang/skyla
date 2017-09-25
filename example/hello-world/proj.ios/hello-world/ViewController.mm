//
//  ViewController.m
//  hello-world
//
//  Created by Hong Shan on 2017/9/10.
//  Copyright © 2017年 levelmax. All rights reserved.
//


#import "ViewController.h"
#include "entry.h"

@interface ViewController () {

}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

@property int opengl_es_version;
@property entry* entry;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    _opengl_es_version = 3;
    if (context == nil) {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        _opengl_es_version = 2;
    }
    self.context = context;

    if (!self.context) {
        NSLog(@"Failed to create ES context");
        return;
    }

    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    [EAGLContext setCurrentContext:self.context];

    /* TODO: looks ungly here, move to Appdelegate someday. */
    _entry = new entry();
    self.game_context = new s2d::context(_entry);
}

- (void)viewDidLayoutSubviews
{
    GLKView* view = (GLKView*)self.view;
    [view bindDrawable];
    self.preferredFramesPerSecond = 60.0f;
    CGSize viewSize = [view bounds].size;
    CGFloat scaleFactor = [view contentScaleFactor];


    self.game_context->init(_opengl_es_version,
                            viewSize.width * scaleFactor,
                            viewSize.height * scaleFactor);
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
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];

    self.game_context->shutdown();
    delete _entry;
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{

}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    CGSize viewSize = [view bounds].size;
    CGFloat scaleFactor = [view contentScaleFactor];
    glClearColor(0.5, 0.5, 0.5, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, viewSize.width * scaleFactor , viewSize.height * scaleFactor);
    NSTimeInterval interval = [self timeSinceLastDraw];
    
    self.game_context->loop(interval);
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

