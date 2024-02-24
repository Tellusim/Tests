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

#version 430 core

/*
 */
#if COMPUTE_SHADER
	
	layout(local_size_x = 32) in;
	
	layout(std430, binding = 0) buffer data_buffer { float16_t data[]; };
	
	coopmat<float16_t, gl_ScopeSubgroup, M, N, gl_MatrixUseAccumulator> c;
	coopmat<float16_t, gl_ScopeSubgroup, M, N, gl_MatrixUseAccumulator> d;
	
	/*
	 */
	void main() {
		
		coopmat<float16_t, gl_ScopeSubgroup, M, K, gl_MatrixUseA> a;
		coopmat<float16_t, gl_ScopeSubgroup, K, N, gl_MatrixUseB> b;
		
		coopMatLoad(a, data, M * M * 0, K, gl_CooperativeMatrixLayoutRowMajor);
		coopMatLoad(b, data, M * M * 1, N, gl_CooperativeMatrixLayoutRowMajor);
		coopMatLoad(c, data, M * M * 2, N, gl_CooperativeMatrixLayoutRowMajor);
		
		d = coopMatMulAdd(a, b, c);
		
		coopMatStore(d, data, M * M * 3, N, gl_CooperativeMatrixLayoutRowMajor);
	}
	
#endif
