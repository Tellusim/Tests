// MIT License
// 
// Copyright (C) 2018-2024, Tellusim Technologies Inc. https://tellusim.com/
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "MTKWindow.h"

/*
 */
@interface MTKDelegate : NSObject<NSApplicationDelegate> {
		
		MTKWindow *window;
	}
	
@end

/*
 */
@implementation MTKDelegate
	
	// application finish launching
	-(void)applicationDidFinishLaunching: (NSNotification*)notification {
		
		// window size
		CGFloat width = 1280.0f;
		CGFloat height = 720.0f;
		NSRect screen = NSScreen.mainScreen.frame;
		CGFloat x = (screen.size.width - width) / 2.0f;
		CGFloat y = (screen.size.height - height) / 2.0f;
		
		// create window
		window = [[MTKWindow alloc] initWithRect:NSMakeRect(x, y, width, height)];
		[window makeKeyAndOrderFront:nullptr];
		window.title = @"Tellusim::MTKWindow";
	}
	
	// exit on last window close
	-(BOOL)applicationShouldTerminateAfterLastWindowClosed: (NSApplication*)app {
		return YES;
	}
	
@end

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	// initialize application
	[NSApplication sharedApplication];
	NSApp.activationPolicy = NSApplicationActivationPolicyRegular;
	NSApp.presentationOptions = NSApplicationPresentationDefault;
	
	// application delegate
	MTKDelegate *delegate = [[MTKDelegate alloc] init];
	NSApp.delegate = delegate;
	
	// run application
	[NSApp run];
	
	return 0;
}
