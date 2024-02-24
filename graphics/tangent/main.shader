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
	layout(location = 1) in vec4 in_normal;
	layout(location = 2) in vec4 in_tangent;
	layout(location = 3) in vec2 in_texcoord;
	
	layout(row_major, binding = 0) uniform CommonParameters {
		mat4 projection;
		mat4 modelview;
		mat4 transform;
		vec4 camera;
	};
	
	layout(location = 0) out vec3 s_normal;
	layout(location = 1) out vec4 s_tangent;
	layout(location = 2) out vec3 s_direction;
	layout(location = 3) out vec2 s_texcoord;
	
	/*
	 */
	void main() {
		
		vec4 position = transform * in_position;
		gl_Position = projection * (modelview * position);
		
		s_normal = (transform * vec4(in_normal.xyz, 0.0f)).xyz;
		s_tangent = vec4((transform * vec4(in_tangent.xyz, 0.0f)).xyz, in_tangent.w);
		s_direction = camera.xyz - position.xyz;
		
		s_texcoord.x = in_texcoord.x;
		s_texcoord.y = 1.0f - in_texcoord.y;
	}
	
#elif FRAGMENT_SHADER
	
	layout(binding = 0, set = 1) uniform texture2D in_texture;
	layout(binding = 0, set = 2) uniform sampler in_sampler;
	
	layout(location = 0) in vec3 s_normal;
	layout(location = 1) in vec4 s_tangent;
	layout(location = 2) in vec3 s_direction;
	layout(location = 3) in vec2 s_texcoord;
	
	layout(location = 0) out vec4 out_color;
	
	/*
	 */
	void main() {
		
		vec3 direction = normalize(s_direction);
		vec3 basis_normal = normalize(s_normal);
		vec3 basis_tangent = normalize(s_tangent.xyz - basis_normal * dot(basis_normal, s_tangent.xyz));
		vec3 basis_binormal = normalize(cross(basis_normal, basis_tangent)) * sign(s_tangent.w);
		
		vec3 normal = texture(sampler2D(in_texture, in_sampler), s_texcoord).xyz * 2.0f - 1.0f;
		normal.z = sqrt(clamp(1.0f - dot(normal.xy, normal.xy), 0.0f, 1.0f));
		
		normal = basis_tangent * normal.x + basis_binormal * normal.y + basis_normal * normal.z;
		
		float diffuse = clamp(dot(direction, normal), 0.0f, 1.0f) * 0.75f;
		float specular = pow(clamp(dot(reflect(-direction, normal), direction), 0.0f, 1.0f), 64.0f);
		
		out_color = vec4(vec3(0.8f, 0.9f, 1.0f) * (diffuse + specular), 1.0f);
	}
	
#endif
