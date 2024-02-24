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

#include <common/common.h>
#include <platform/TellusimContext.h>
#include <platform/TellusimDevice.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	// create app
	App app(argc, argv);
	if(!app.create()) return 1;
	
	// create primary context
	Context primary_context(app.getPlatform(), app.getDevice());
	if(!primary_context || !primary_context.create()) return 1;
	
	// create device
	Device device(primary_context);
	if(!device) return 1;
	
	// allocate 1GB buffer
	Buffer buffer = device.createBuffer(Buffer::FlagStorage, 1024 * 1024 * 1024);
	if(!buffer) return 1;
	buffer.clearPtr();
	
	// print device info
	TS_LOGF(Message, "%s %s %s\n", device.getName().get(), device.getVendor().get(), device.getVersion().get());
	
	// create secondary context
	for(uint32_t i = 0; i < 128; i++) {
		
		Context secondary_context;
		
		if(primary_context.getPlatform() == PlatformD3D12) {
			D3D12Context d3d12_context = D3D12Context(primary_context);
			if(!d3d12_context) break;
			
			D3D12Context context;
			if(!context.create(d3d12_context.getDevice(), d3d12_context.getQueue())) break;
			secondary_context = context;
		}
		else if(primary_context.getPlatform() == PlatformD3D11) {
			D3D11Context d3d11_context = D3D11Context(primary_context);
			if(!d3d11_context) break;
			
			D3D11Context context;
			if(!context.create(d3d11_context.getDevice())) break;
			secondary_context = context;
		}
		else if(primary_context.getPlatform() == PlatformMTL) {
			MTLContext mtl_context = MTLContext(primary_context);
			if(!mtl_context) break;
			
			MTLContext context;
			if(!context.create(mtl_context.getDevice(), mtl_context.getQueue())) break;
			secondary_context = context;
		}
		else if(primary_context.getPlatform() == PlatformVK) {
			VKContext vk_context = VKContext(primary_context);
			if(!vk_context) break;
			
			VKContext context;
			if(!context.create(vk_context.getInstance(), vk_context.getInstanceProcAddress(), vk_context.getAdapter(), vk_context.getDevice(), vk_context.getFamily(), 0)) break;
			secondary_context = context;
		}
		else if(primary_context.getPlatform() == PlatformGL) {
			GLContext gl_context = GLContext(primary_context);
			if(!gl_context) break;
			
			GLContext context;
			if(!context.create(gl_context.getGLContext())) break;
			secondary_context = context;
		}
		else if(primary_context.getPlatform() == PlatformGLES) {
			GLESContext gles_context = GLESContext(primary_context);
			if(!gles_context) break;
			
			GLESContext context;
			if(!context.create(gles_context.getGLESContext())) break;
			secondary_context = context;
		}
		else {
			TS_LOGF(Error, "unknown platform %s\n", primary_context.getPlatformName());
			break;
		}
		
		// create device
		Device device(secondary_context);
		if(!device) break;
		
		// allocate 1GB buffer
		Buffer buffer = device.createBuffer(Buffer::FlagStorage, 1024 * 1024 * 1024);
		if(!buffer) break;
		buffer.clearPtr();
		
		// print device info
		TS_LOGF(Message, "%s %s %s %u\n", device.getName().get(), device.getVendor().get(), device.getVersion().get(), device.getIndex());
		
		// flip device
		for(uint32_t j = 0; j < 16; j++) {
			device.flip();
		}
		
		// finish context
		secondary_context.finish();
	}
	
	// finish primary context
	primary_context.finish();
	
	return 0;
}
