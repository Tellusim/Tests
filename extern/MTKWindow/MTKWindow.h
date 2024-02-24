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

#ifndef __MTK_WINDOW_H__
#define __MTK_WINDOW_H__

#include <Cocoa/Cocoa.h>
#include <MetalKit/MetalKit.h>

#include <platform/TellusimContext.h>
#include <platform/TellusimSurface.h>
#include <platform/TellusimPipeline.h>
#include <platform/TellusimDevice.h>

/*
 */
@interface MTKWindow : NSWindow<MTKViewDelegate> {
		
		MTKView *mtl_view;
		id<MTLDevice> mtl_device;
		id<MTLCommandQueue> mtl_queue;
		
		Tellusim::MTLContext context;
		
		Tellusim::MTLSurface surface;
		
		Tellusim::Device device;
		Tellusim::Pipeline pipeline;
		Tellusim::Buffer vertex_buffer;
		Tellusim::Buffer index_buffer;
		
		bool initialized;
	}
	
	-(id)initWithRect: (NSRect)rect;
	
	-(void)mtkView: (MTKView*)view drawableSizeWillChange: (CGSize)size;
	
	-(void)drawInMTKView: (MTKView*)view;
	
@end

#endif /* __MTK_WINDOW_H__ */
