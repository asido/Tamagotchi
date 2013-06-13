//
//  ViewController.m
//  TamagotchiEngineTest_iOS
//
//  Created by st on 13/06/2013.
//  Copyright (c) 2013 Asido. All rights reserved.
//

#import "ViewController.h"
#import "TestMain.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UITextView *outputTextArea;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    const std::string output = TestMain();
    [self.outputTextArea setText:[NSString stringWithCString:output.c_str() encoding:NSASCIIStringEncoding]];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
