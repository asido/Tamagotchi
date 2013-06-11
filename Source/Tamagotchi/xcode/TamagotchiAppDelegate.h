//
//  AppDelegate.h
//  Tamagotchi
//
//  Created by st on 11/06/2013.
//  Copyright (c) 2013 Asido. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;

@interface TamagotchiAppDelegate : NSObject <UIApplicationDelegate>
{
    UIWindow *window;
    EAGLView *glView;
}

@property (strong, nonatomic) IBOutlet UIWindow *window;
@property (strong, nonatomic) IBOutlet EAGLView *glView;

@end
