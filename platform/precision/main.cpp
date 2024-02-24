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
#include <math/TellusimMath.h>
#include <platform/TellusimDevice.h>
#include <platform/TellusimPipeline.h>
#include <platform/TellusimCommand.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	DECLARE_WINDOW
	
	// create window
	String title = String::format("%s Tellusim::Precision", window.getPlatformName());
	if(!window.create(title) || !window.setHidden(false)) return 1;
	
	// structures
	struct CommonParameters {
		float32_t aspect;
		float32_t time;
	};
	
	// create device
	Device device(window);
	if(!device) return 1;
	
	// create f32 pipeline
	Pipeline pipeline_f32 = device.createPipeline();
	pipeline_f32.setUniformMask(0, Shader::MaskVertex);
	pipeline_f32.setColorFormat(window.getColorFormat());
	pipeline_f32.setDepthFormat(window.getDepthFormat());
	if(!pipeline_f32.loadShaderGLSL(Shader::TypeVertex, "main.shader", "VERTEX_SHADER=1")) return 1;
	if(!pipeline_f32.loadShaderGLSL(Shader::TypeFragment, "main.shader", "FRAGMENT_SHADER=1; TYPE=float")) return 1;
	if(!pipeline_f32.create()) return 1;
	
	// create f64 pipeline
	Pipeline pipeline_f64;
	if(device.getFeatures().shaderf64) {
		pipeline_f64 = device.createPipeline(pipeline_f32);
		if(!pipeline_f64.loadShaderGLSL(Shader::TypeFragment, "main.shader", "FRAGMENT_SHADER=1; TYPE=double")) pipeline_f64.clear();
		if(!pipeline_f64.create()) pipeline_f64.clearPtr();
	}
	
	// create f16 pipeline
	Pipeline pipeline_f16;
	if(device.getFeatures().shaderf16) {
		pipeline_f16 = device.createPipeline(pipeline_f32);
		if(!pipeline_f16.loadShaderGLSL(Shader::TypeFragment, "main.shader", "FRAGMENT_SHADER=1; TYPE=float16_t")) pipeline_f16.clear();
		if(!pipeline_f16.create()) pipeline_f16.clearPtr();
	}
	
	// create target
	Target target = device.createTarget(window);
	
	// main loop
	DECLARE_GLOBAL
	window.run([&]() -> bool {
		DECLARE_COMMON
		
		Window::update();
		
		if(!window.render()) return false;
		
		// window title
		if(fps > 0.0f) window.setTitle(String::format("%s %.1f FPS", title.get(), fps));
		
		// window target
		target.begin();
		{
			// create command list
			Command command = device.createCommand(target);
			
			// common parameters
			CommonParameters parameters;
			parameters.aspect = (float32_t)window.getWidth() / (float32_t)window.getHeight();
			parameters.time = time;
			
			// precision
			Pipeline pipeline = pipeline_f32;
			if(window.getKeyboardKey('1') && pipeline_f64) pipeline = pipeline_f64;
			if(window.getKeyboardKey('2') && pipeline_f16) pipeline = pipeline_f16;
			
			// draw surface
			command.setPipeline(pipeline);
			command.setUniform(0, parameters);
			command.drawArrays(3);
		}
		target.end();
		
		if(!window.present()) return false;
		
		if(!device.check()) return false;
		
		return true;
	});
	
	// finish context
	window.finish();
	
	return 0;
}
