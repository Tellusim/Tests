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
#if SHADOW_TARGET
	
	#if VERTEX_SHADER
		
		layout(row_major, binding = 0) uniform CommonParameters {
			mat4 projection;
			mat4 modelview;
			mat4 transform;
		};
		
		layout(location = 0) in vec4 in_position;
		
		/*
		 */
		void main() {
			
			vec4 position = transform * in_position;
			
			gl_Position = projection * (modelview * position);
		}
		
	#elif FRAGMENT_SHADER
		
		/*
		 */
		void main() {
			
		}
		
	#endif
	
/*
 */
#elif VERTEX_SHADER
	
	layout(location = 0) in vec4 in_position;
	layout(location = 1) in vec3 in_normal;
	
	layout(row_major, binding = 0) uniform CommonParameters {
		mat4 projection;
		mat4 modelview;
		mat4 transform;
		vec4 texcoord;
		vec4 camera;
		vec4 light;
	};
	
	layout(location = 0) out vec3 s_normal;
	layout(location = 1) out vec4 s_texcoord;
	layout(location = 2) out vec3 s_view_vector;
	layout(location = 3) out vec3 s_light_vector;
	
	/*
	 */
	void main() {
		
		vec4 position = transform * in_position;
		gl_Position = projection * (modelview * position);
		
		s_normal = (transform * vec4(in_normal, 0.0f)).xyz;
		
		vec3 light_vector = light.xyz - position.xyz;
		s_texcoord = vec4(-light_vector * texcoord.x, texcoord.y);
		
		s_view_vector = camera.xyz - position.xyz;
		s_light_vector = light_vector;
	}
	
#elif FRAGMENT_SHADER
	
	layout(binding = 0, set = 1) uniform textureCube in_texture;
	layout(binding = 0, set = 2) uniform samplerShadow in_sampler;
	
	layout(location = 0) in vec3 s_normal;
	layout(location = 1) in vec4 s_texcoord;
	layout(location = 2) in vec3 s_view_vector;
	layout(location = 3) in vec3 s_light_vector;
	
	layout(location = 0) out vec4 out_color;
	
	/*
	 */
	void main() {
		
		vec3 normal = normalize(s_normal);
		vec3 view_vector = normalize(s_view_vector);
		vec3 light_vector = normalize(s_light_vector);
		
		float diffuse = clamp(dot(light_vector, normal), 0.0f, 1.0f) * 0.75f;
		float specular = pow(clamp(dot(reflect(-light_vector, normal), view_vector), 0.0f, 1.0f), 16.0f) * 0.75f;
		
		vec3 direction = abs(s_texcoord.xyz);
		float compare = -1.0f / max(max(direction.x, direction.y), direction.z) + s_texcoord.w;
		float shadow = texture(samplerCubeShadow(in_texture, in_sampler), vec4(s_texcoord.xyz, compare));
		
		out_color = vec4((diffuse + specular) * clamp(shadow + 0.1f, 0.0f, 1.0f));
	}
	
#endif
