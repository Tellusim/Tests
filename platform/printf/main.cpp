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
#include <platform/TellusimDevice.h>
#include <platform/TellusimBuffer.h>
#include <platform/TellusimKernel.h>
#include <platform/TellusimCompute.h>
#include <platform/TellusimContext.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	// create app
	App app(argc, argv);
	if(!app.create()) return 1;
	
	// create context
	Context context(app.getPlatform(), app.getDevice());
	if(!context || !context.create()) return 1;
	
	// create device
	Device device(context);
	if(!device) return 1;
	
	// check compute shader support
	if(!device.hasShader(Shader::TypeCompute)) {
		TS_LOG(Error, "compute shader is not supported\n");
		return 0;
	}
	
	// create kernel
	Kernel kernel = device.createKernel().setStorages(1);
	if(!kernel.loadShaderGLSL("main.shader", "COMPUTE_SHADER=1")) return 1;
	if(!kernel.create()) return 1;
	
	// create buffer
	Array<uint32_t> data(1024 * 1024, 0u);
	Buffer storage_buffer = device.createBuffer(Buffer::FlagStorage | Buffer::FlagSource, data.get(), data.bytes());
	if(!storage_buffer) return 1;
	
	{
		// create command list
		Compute compute = device.createCompute();
		
		// run kernel
		compute.setKernel(kernel);
		compute.setStorageBuffer(0, storage_buffer);
		compute.dispatch(1);
		
		// buffer barrier
		compute.barrier(storage_buffer);
	}
	
	// get buffer data
	if(!device.getBuffer(storage_buffer, data.get())) return 1;
	
	// print buffer
	String str, format;
	for(uint32_t i = 1; i < data[0]; i++) {
		uint32_t num = data[i] >> 16;
		uint32_t size = data[i] & 0xffff;
		if(i + size > data[0] || size < num) break;
		const char *f = (const char*)&data[i + 1];
		const uint32_t *args = &data[i + 1 + size - num];
		str.clear();
		while(*f) {
			if(*f == '%') {
				format.clear();
				format += *f++;
				while(*f) {
					char c = *f++;
					format += c;
					if(!strchr("0123456789+-.", c)) {
						if(format == "%d") String::fromi32(str, *args++, 10);
						else if(format == "%i") String::fromi32(str, *args++, 10);
						else if(format == "%u") String::fromu32(str, *args++, 10);
						else if(format == "%x") String::fromu32(str, *args++, 16);
						else if(format == "%f") String::fromf64(str, f32u32(*args++).f, 6, false, false);
						else if(format == "%g") String::fromf64(str, f32u32(*args++).f, 6, false, true);
						else if(format == "%s") str += (const char*)args++;
						else if(format == "%%") str += '%';
						else if(strchr("fegEG", c)) str += String::format(format.get(), f32u32(*args++).f);
						else str += String::format(format.get(), *args++);
						break;
					}
				}
			} else {
				str += *f++;
			}
		}
		if(str) Log::print(Log::Message, str.get());
		i += size;
	}
	
	// check errors
	device.check();
	
	// finish context
	context.finish();
	
	return 0;
}
