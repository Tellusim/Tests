#!/usr/bin/env python3

# MIT License
# 
# Copyright (C) 2018-2024, Tellusim Technologies Inc. https://tellusim.com/
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from tellusimd import *

# create compiler
compiler = ShaderCompiler()

# load shader
if not compiler.loadGLSL(Shader.TypeCompute, 'main.shader', 'COMPUTE_SHADER=1'): exit(1)

# unknown source
Log.print(compiler.getSource())

# platform source
for platform in range(PlatformFusion + 1, PlatformAny):
	if not isPlatformAvailable(platform): continue
	Log.printf('\n%s:\n', getPlatformName(platform))
	Log.print(compiler.getSource(platform))
