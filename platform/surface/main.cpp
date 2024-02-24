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
#include <core/TellusimString.h>
#include <platform/TellusimDevice.h>
#include <platform/TellusimTarget.h>
#include <platform/TellusimSurface.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	DECLARE_WINDOW
	if(!window) return 1;
	
	// create window
	String title = String::format("%s Tellusim::Surface", window.getPlatformName());
	if(!window.create(title) || !window.setHidden(false)) return 1;
	
	// create target
	Device device = Device(window);
	Target target = device.createTarget(window);
	
	// window surface
	Surface surface = window.getSurface();
	TS_LOGF(Message, "%s\n", surface.getPlatformName());
	TS_LOGF(Message, "Size: %ux%u\n", surface.getWidth(), surface.getHeight());
	
	// main loop
	DECLARE_GLOBAL
	window.run([&]() -> bool {
		DECLARE_COMMON
		
		using Tellusim::sin;
		using Tellusim::cos;
		
		Window::update();
		
		if(!window.render()) return false;
		
		// window surface
		if(frame_counter < 4) {
			
			// frame number
			TS_LOGF(Message, "Frame: %u\n", frame_counter);
			
			// Direct3D12 surface
			if(surface.getPlatform() == PlatformD3D12) {
				D3D12Surface d3d12_surface = D3D12Surface(surface);
				TS_LOGF(Message, "RenderTargetView: %" TS_FORMAT_64 "u\n", (uint64_t)d3d12_surface.getRenderTargetView());
				TS_LOGF(Message, "DepthStencilView: %" TS_FORMAT_64 "u\n", (uint64_t)d3d12_surface.getDepthStencilView());
			}
			
			// Direct3D11 surface
			if(surface.getPlatform() == PlatformD3D11) {
				D3D11Surface d3d11_surface = D3D11Surface(surface);
				TS_LOGF(Message, "RenderTargetView: %p\n", d3d11_surface.getRenderTargetView());
				TS_LOGF(Message, "DepthStencilView: %p\n", d3d11_surface.getDepthStencilView());
			}
			
			// Metal surface
			if(surface.getPlatform() == PlatformMTL) {
				MTLSurface mtl_surface = MTLSurface(surface);
				TS_LOGF(Message, "Descriptor: %p\n", mtl_surface.getDescriptor());
			}
			
			// Vulkan surface
			if(surface.getPlatform() == PlatformVK) {
				VKSurface vk_surface = VKSurface(surface);
				TS_LOGF(Message, "RenderPass: %p\n", vk_surface.getRenderPass());
				TS_LOGF(Message, "Framebuffer: %p\n", vk_surface.getFramebuffer());
			}
			
			// OpenGL surface
			if(surface.getPlatform() == PlatformGL) {
				GLSurface gl_surface = GLSurface(surface);
				TS_LOGF(Message, "FramebufferID: %u\n", gl_surface.getFramebufferID());
			}
		}
		
		// window target
		target.setClearColor(Color(sin(time), cos(time), sin(time) * cos(time)) * 0.5f + 0.5f);
		target.begin();
		target.end();
		
		if(!window.present()) return false;
		
		return true;
	});
	
	// finish context
	window.finish();
	
	return 0;
}
