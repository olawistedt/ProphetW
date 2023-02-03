
#include <TargetConditionals.h>
#if TARGET_OS_IOS == 1
#import <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif

#define IPLUG_AUVIEWCONTROLLER IPlugAUViewController_vProphetW
#define IPLUG_AUAUDIOUNIT IPlugAUAudioUnit_vProphetW
#import <ProphetWAU/IPlugAUViewController.h>
#import <ProphetWAU/IPlugAUAudioUnit.h>

//! Project version number for ProphetWAU.
FOUNDATION_EXPORT double ProphetWAUVersionNumber;

//! Project version string for ProphetWAU.
FOUNDATION_EXPORT const unsigned char ProphetWAUVersionString[];

@class IPlugAUViewController_vProphetW;
