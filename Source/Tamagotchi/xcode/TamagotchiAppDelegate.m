//
//  AppDelegate.m
//  Tamagotchi
//
//  Created by st on 11/06/2013.
//  Copyright (c) 2013 Asido. All rights reserved.
//

#import "TamagotchiAppDelegate.h"
#import "EAGLView.h"

@implementation TamagotchiAppDelegate

@synthesize window;
@synthesize glView;

- (void)applicationDidFinishLaunching:(UIApplication*)application
{
    glView.renderInterval = 1.0f / 60.0f;
    [glView startRendering];
    
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    glView.renderInterval = 1.0f / 5.0f;
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    glView.renderInterval = 1.0f / 60.0f;
}

- (void)dealloc
{
    [window release];
    [glView release];
    [super dealloc];
}

@end
