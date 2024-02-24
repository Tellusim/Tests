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
	
	layout(local_size_x = 8, local_size_y = 4) in;
	
	layout(std430, binding = 0) buffer ScalarBuffer { uint scalar_buffer[]; };
	
	shared uint value;
	
	/*
	 */
	inline uint printv(uint value) { return value; }
	inline uint printv(float value) { return floatBitsToUint(value); }
	#define PRINTV(VALUE, INDEX) scalar_buffer[i + (INDEX + 1u)] = printv(VALUE);
	#define PRINTF(ARGC, ...) { \
		uint i = atomicAdd(scalar_buffer[0u], __VA_ARGC__ + 1u) + 1u; \
		scalar_buffer[i] = __VA_ARGC__ | ((ARGC) << 16u); \
		__VA_ARGM__(PRINTV) \
	}
	#define printf(FORMAT, ...) PRINTF(__VA_ARGC__, @FORMAT, __VA_ARGS__)
	
	/*
	 */
	void main() {
		
		uint local_id = gl_LocalInvocationIndex;
		uvec2 global_id = gl_GlobalInvocationID.xy;
		
		[[branch]] if(local_id == 0u) value = 0u;
		memoryBarrierShared(); barrier();
		
		printf("global: %ux%u local: %2u shared: %2.0f\n", global_id.x, global_id.y, local_id, float(atomicIncrement(value)));
	}
	
#endif
