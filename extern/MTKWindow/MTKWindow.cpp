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

#include <core/TellusimLog.h>
#include <core/TellusimTime.h>
#include <math/TellusimMath.h>
#include <platform/TellusimCommand.h>

/*
 */
#define EXTERNAL_DEVICE		1

/*
 */
using namespace Tellusim;

/*
 */
@implementation MTKWindow
	
	/*
	 */
	-(id)initWithRect: (NSRect)rect {
		
		initialized = false;
		
		#if EXTERNAL_DEVICE
			
			// create Metal device
			mtl_device = MTLCreateSystemDefaultDevice();
			if(mtl_device == nullptr) {
				TS_LOG(Error, "MTKWindow::initWithRect(): can't get Metal device\n");
				return self;
			}
			
			// create command queue
			mtl_queue = [mtl_device newCommandQueue];
			
			// create external context
			if(!context.create((__bridge void*)mtl_device, (__bridge void*)mtl_queue)) {
				TS_LOG(Error, "MTKWidget::create_context(): can't create context\n");
				return self;
			}
			
			// create surface
			surface = MTLSurface(context);
			if(!surface) {
				TS_LOG(Error, "MTKWindow::initWithRect(): can't create surface\n");
				return self;
			}
			
		#else
			
			// create internal context
			if(!context.create()) {
				TS_LOG(Error, "MTKWidget::create_context(): can't create context\n");
				return self;
			}
			
			// create surface
			surface = MTLSurface(context);
			if(!surface) {
				TS_LOG(Error, "MTKWindow::initWithRect(): can't create surface\n");
				return self;
			}
			
			// internal device
			mtl_device = (__bridge id<MTLDevice>)surface.getDevice();
			mtl_queue = (__bridge id<MTLCommandQueue>)surface.getQueue();
			
		#endif
		
		// create window
		NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable;
		self = [super initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:YES];
		self.releasedWhenClosed = YES;
		
		// create Metal view
		mtl_view = [[MTKView alloc] initWithFrame:rect device:mtl_device];
		mtl_view.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
		if(mtl_device.isDepth24Stencil8PixelFormatSupported) mtl_view.depthStencilPixelFormat = MTLPixelFormatDepth24Unorm_Stencil8;
		else mtl_view.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
		mtl_view.sampleCount = 4;
		mtl_view.delegate = self;
		
		// window context view
		self.contentView = mtl_view;
		
		// create surface
		surface.setSize((uint32_t)rect.size.width, (uint32_t)rect.size.height);
		surface.setColorFormat(FormatBGRAu8n);
		if(mtl_device.isDepth24Stencil8PixelFormatSupported) surface.setDepthFormat(FormatDu24Su8);
		else surface.setDepthFormat(FormatDf32Su8);
		surface.setMultisample(mtl_view.sampleCount);
		
		// create device
		device = Device(surface);
		if(!device) return self;
		
		// create pipeline
		pipeline = device.createPipeline();
		pipeline.setUniformMask(0, Shader::MaskVertex);
		pipeline.addAttribute(Pipeline::AttributePosition, FormatRGBf32, 0, sizeof(float32_t) * 0, sizeof(float32_t) * 6);
		pipeline.addAttribute(Pipeline::AttributeNormal, FormatRGBf32, 0, sizeof(float32_t) * 3, sizeof(float32_t) * 6);
		pipeline.setDepthFunc(Pipeline::DepthFuncLessEqual);
		pipeline.setColorFormat(surface.getColorFormat());
		pipeline.setDepthFormat(surface.getDepthFormat());
		pipeline.setMultisample(surface.getMultisample());
		if(!pipeline.loadShaderGLSL(Shader::TypeVertex, "main.shader", "VERTEX_SHADER=1")) return self;
		if(!pipeline.loadShaderGLSL(Shader::TypeFragment, "main.shader", "FRAGMENT_SHADER=1")) return self;
		if(!pipeline.create()) return self;
		
		// create mesh geometry
		#include "main_mesh.h"
		vertex_buffer = device.createBuffer(Buffer::FlagVertex, mesh_vertices, sizeof(float32_t) * num_mesh_vertices);
		index_buffer = device.createBuffer(Buffer::FlagIndex, mesh_indices, sizeof(uint32_t) * num_mesh_indices);
		if(!vertex_buffer || !index_buffer) return self;
		
		initialized = true;
		
		return self;
	}
	
	/*
	 */
	-(void)mtkView: (MTKView*)view drawableSizeWillChange: (CGSize)size {
		
		// check status
		if(!initialized) return;
		
		// surface size
		surface.setSize((uint32_t)size.width, (uint32_t)size.height);
	}
	
	/*
	 */
	-(void)drawInMTKView: (MTKView*)view {
		
		// check status
		if(!initialized) return;
		
		@autoreleasepool {
			
			// structures
			struct CommonParameters {
				Matrix4x4f projection;
				Matrix4x4f modelview;
				Matrix4x4f transform;
				Vector4f camera;
			};
			
			// render pass descriptor
			MTLRenderPassDescriptor *descriptor = view.currentRenderPassDescriptor;
			surface.setDescriptor((__bridge void*)descriptor);
			
			// widget target
			Target target = device.createTarget(surface);
			target.setClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			target.begin();
			{
				// current time
				float32_t time = Time::seconds();
				
				// common parameters
				CommonParameters common_parameters;
				common_parameters.camera = Vector4f(2.0f, 2.0f, 1.0f, 0.0f);
				common_parameters.projection = Matrix4x4f::perspective(60.0f, (float32_t)surface.getWidth() / surface.getHeight(), 0.1f, 1000.0f);
				if(target.isFlipped()) common_parameters.projection = Matrix4x4f::scale(1.0f, -1.0f, 1.0f) * common_parameters.projection;
				common_parameters.modelview = Matrix4x4f::lookAt(Vector3f(common_parameters.camera), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));
				common_parameters.transform = Matrix4x4f::rotateZ(time * 32.0f) * Matrix4x4f::rotateY(60.0f + time * 8.0f);
				
				// create command list
				Command command = device.createCommand(target);
				
				// draw mesh
				command.setPipeline(pipeline);
				command.setUniform(0, common_parameters);
				command.setVertexBuffer(0, vertex_buffer);
				command.setIndexBuffer(FormatRu32, index_buffer);
				command.drawElements(index_buffer.getSize() / 4);
			}
			target.end();
			
			// flush context
			device.flush();
			
			// present drawable
			id<MTLCommandBuffer> command = (__bridge id<MTLCommandBuffer>)surface.getCommand();
			[command presentDrawable:view.currentDrawable];
			
			// flip context
			device.flip();
		}
	}
	
@end