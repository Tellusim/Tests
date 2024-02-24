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
#if RENDER_TARGET
	
	#if VERTEX_SHADER
		
		layout(location = 0) in vec4 in_position;
		layout(location = 1) in vec2 in_texcoord;
		
		/*
		 */
		void main() {
			
			gl_Position = in_position;
		}
		
	#elif FRAGMENT_SHADER
		
		layout(binding = 0) uniform CommonParameters {
			float time;
		};
		
		layout(location = 0) out vec4 out_color;
		
		/*
		 */
		void main() {
			
			vec3 color = cos(vec3(0.0f, 0.5f, 1.0f) * 3.14f + float(gl_SampleID) + time);
			
			out_color = vec4(color * 0.5f + 0.5f, 1.0f);
		}
		
	#endif
	
/*
 */
#elif VERTEX_SHADER
	
	layout(location = 0) in vec4 in_position;
	layout(location = 1) in vec2 in_texcoord;
	
	layout(location = 0) out vec2 s_texcoord;
	
	/*
	 */
	void main() {
		
		gl_Position = in_position;
		
		s_texcoord = in_texcoord;
	}
	
#elif FRAGMENT_SHADER
	
	layout(binding = 0, set = 0) uniform texture2DMS in_texture;
	
	layout(location = 0) in vec2 s_texcoord;
	
	layout(location = 0) out vec4 out_color;
	
	/*
	 */
	void main() {
		
		int index = 0;
		#if SAMPLES == 2
			if(s_texcoord.x > 0.5f) index += 1;
		#elif SAMPLES == 4
			if(s_texcoord.x > 0.5f) index += 2;
			if(s_texcoord.y > 0.5f) index += 1;
		#elif SAMPLES == 8
			if(s_texcoord.x > 0.25f) index += 2;
			if(s_texcoord.x > 0.50f) index += 2;
			if(s_texcoord.x > 0.75f) index += 2;
			if(s_texcoord.y > 0.50f) index += 1;
		#endif
		
		ivec2 size = textureSize(in_texture);
		
		out_color = texelFetch(in_texture, ivec2(s_texcoord * size), index);
	}
	
#endif
