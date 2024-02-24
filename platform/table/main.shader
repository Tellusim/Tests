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

#version 420 core

/*
 */
#if VERTEX_SHADER
	
	layout(location = 0) in vec4 in_position;
	layout(location = 1) in vec2 in_texcoord;
	
	layout(binding = 0) uniform CommonParameters {
		vec4 transform;
		uvec4 indices;
	};
	
	layout(location = 0) out vec2 s_texcoord;
	layout(location = 1) flat out uvec4 s_indices;
	
	/*
	 */
	void main() {
		
		gl_Position = vec4(in_position.xy * transform.xy + transform.zw, 0.0f, 1.0f);
		
		s_texcoord = in_texcoord;
		
		s_indices = indices;
	}
	
#elif FRAGMENT_SHADER
	
	layout(binding = 0, set = 1) uniform sampler in_sampler;
	layout(binding = 0, set = 2) uniform texture2D in_textures[];
	
	layout(location = 0) in vec2 s_texcoord;
	layout(location = 1) flat in uvec4 s_indices;
	
	layout(location = 0) out vec4 out_color;
	
	/*
	 */
	void main() {
		
		vec4 texture_0 = texture(sampler2D(in_textures[s_indices.x], in_sampler), s_texcoord);
		vec4 texture_1 = texture(sampler2D(in_textures[s_indices.y], in_sampler), s_texcoord);
		vec4 texture_2 = texture(sampler2D(in_textures[s_indices.z], in_sampler), s_texcoord);
		vec4 texture_3 = texture(sampler2D(in_textures[s_indices.w], in_sampler), s_texcoord);
		
		out_color = texture_0 * texture_2 + texture_1 * texture_3;
	}
	
#endif
