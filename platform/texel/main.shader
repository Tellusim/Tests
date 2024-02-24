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
	
	layout(location = 0) in float in_index;
	
	layout(row_major, binding = 0) uniform CommonParameters {
		mat4 projection;
		mat4 modelview;
		mat4 transform;
		vec4 camera;
	};
	
	layout(binding = 0, set = 3) uniform samplerBuffer in_position;
	layout(binding = 1, set = 3) uniform samplerBuffer in_normal;
	
	layout(location = 0) out vec3 s_position;
	layout(location = 1) out vec3 s_direction;
	layout(location = 2) out vec3 s_normal;
	
	/*
	 */
	void main() {
		
		#if 1
			int index = gl_VertexIndex;
		#else
			int index = int(in_index);
		#endif
		
		vec4 position = transform * texelFetch(in_position, index);
		vec4 normal = transform * texelFetch(in_normal, index);
		
		gl_Position = projection * (modelview * position);
		
		s_position = texelFetch(in_position, index).xyz;
		s_direction = camera.xyz - position.xyz;
		s_normal = normal.xyz;
	}
	
#elif FRAGMENT_SHADER
	
	layout(binding = 0, set = 1) uniform texture2D in_texture;
	layout(binding = 0, set = 2) uniform sampler in_sampler;
	
	layout(location = 0) in vec3 s_position;
	layout(location = 1) in vec3 s_direction;
	layout(location = 2) in vec3 s_normal;
	
	layout(location = 0) out vec4 out_color;
	
	/*
	 */
	void main() {
		
		vec2 texcoord = vec2(atan(s_position.x, s_position.y), acos(s_position.z)) * vec2(2.0f, 4.0f) / 3.14159f;
		vec4 color = texture(sampler2D(in_texture, in_sampler), texcoord);
		
		vec3 direction = normalize(s_direction);
		vec3 normal = normalize(s_normal);
		
		float diffuse = clamp(dot(direction, normal), 0.0f, 1.0f) * 0.75f;
		float specular = pow(clamp(dot(reflect(-direction, normal), direction), 0.0f, 1.0f), 8.0f);
		
		out_color = color * (diffuse + specular);
	}
	
#endif
