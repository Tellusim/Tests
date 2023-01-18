// MIT License
// 
// Copyright (C) 2018-2023, Tellusim Technologies Inc. https://tellusim.com/
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
#include <platform/TellusimShader.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	// create compliler
	ShaderCompiler compiler;
	
	// load shader
	if(!compiler.loadGLSL(Shader::TypeCompute, "main.shader", "COMPUTE_SHADER=1")) return 1;
	
	// unknown source
	Log::print(compiler.getSource().get());
	
	// platform source
	for(uint32_t i = PlatformFusion + 1; i < PlatformAny; i++) {
		Platform platform = (Platform)i;
		if(!isPlatformAvailable(platform)) continue;
		Log::printf("\n%s:\n", getPlatformName(platform));
		Log::print(compiler.getSource(platform).get());
	}
	
	return 0;
}
