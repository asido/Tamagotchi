//
//  EAGLView.m
//  Tamagotchi
//
//  Created by st on 11/06/2013.
//  Copyright (c) 2013 Asido. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "EAGLView.h"

#include "../Engine/EngineMain.h"

@interface EAGLView()
@property (strong, nonatomic) EAGLContext   *context;
@property (strong, nonatomic) NSTimer       *renderTimer;

- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;
@end

@implementation EAGLView
{
    TamagotchiEngine *tamagotchiEngine;
}

@synthesize renderInterval;

@synthesize context;
@synthesize renderTimer;

#pragma mark - override

// NOTE: This override is *very* important.
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if (self = [super initWithCoder:aDecoder])
    {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer*)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                                                                  kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!context || ![EAGLContext setCurrentContext:context])
        {
            NSLog(@"%s: error initializing EAGLContext.", __PRETTY_FUNCTION__);
            [self release];
            return nil;
        }
        
        renderInterval = 1.0f / 60.0f;

        tamagotchiEngine = new TamagotchiEngine();
    }

    return self;
}

- (void)layoutSubviews
{
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self createFramebuffer];
    tamagotchiEngine->Init(backingWidth, backingHeight);
    [self renderView];
}

- (void)dealloc
{
    [self stopRendering];
    
    if ([EAGLContext currentContext] == context)
    {
        [EAGLContext setCurrentContext:nil];
    }
    
    [context release];
    [super dealloc];
}

#pragma mark - public

- (void)startRendering
{
    if (!self.renderTimer)
    {
        self.renderTimer = [NSTimer scheduledTimerWithTimeInterval:renderInterval
                                                            target:self
                                                          selector:@selector(renderView)
                                                          userInfo:nil
                                                           repeats:YES];
    }
}

- (void)stopRendering
{
    [self.renderTimer invalidate];
    self.renderTimer = nil;
}

- (void)renderView
{
    [EAGLContext setCurrentContext:context];
    
    tamagotchiEngine->OnRender();
    
    glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

#pragma mark - private

- (BOOL)createFramebuffer
{
    glGenFramebuffers(1, &viewFramebuffer);
    glGenRenderbuffers(1, &viewRenderbuffer);
    
    glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, viewRenderbuffer);
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"%s: framebuffer creation has failed.", __PRETTY_FUNCTION__);
        return NO;
    }
    
    return YES;
}

- (void)destroyFramebuffer
{
    glDeleteFramebuffers(1, &viewFramebuffer);
    viewFramebuffer = 0;
    glDeleteRenderbuffers(1, &viewRenderbuffer);
    viewRenderbuffer = 0;
    glDeleteRenderbuffers(1, &depthRenderbuffer);
    depthRenderbuffer = 0;
}

@end
