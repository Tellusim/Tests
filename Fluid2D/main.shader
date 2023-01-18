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

#version 430 core

/*
 */
#if COMPUTE_ADVECTION_SHADER
	
	layout(local_size_x = 8, local_size_y = 8) in;
	
	layout(std140, binding = 0) uniform AdvectionParameters {
		vec2 mouse_texcoord;
		vec2 mouse_velocity;
		float mouse_radius;
		float ifps;
	};
	
	layout(binding = 0, set = 2) uniform sampler in_sampler;
	layout(binding = 0, set = 1) uniform texture2D in_velocity;
	layout(binding = 1, set = 1, rg32f) uniform writeonly image2D out_surface;
	
	/*
	 */
	void main() {
		
		ivec2 size = textureSize(in_velocity, 0).xy;
		ivec2 itexcoord = ivec2(gl_GlobalInvocationID.xy);
		ivec2 hsize = size >> 1;
		
		vec2 velocity = texelFetch(in_velocity, itexcoord, 0).xy;
		vec2 texcoord = vec2(itexcoord) / vec2(size - 1) - velocity * ifps;
		velocity = textureLod(sampler2D(in_velocity, in_sampler), texcoord, 0.0f).xy;
		
		vec2 direction = (texcoord - mouse_texcoord) / mouse_radius;
		float attenuation = dot(direction, direction);
		velocity += mouse_velocity * clamp(1.0f - attenuation, 0.0f, 1.0f);
		
		if(abs(itexcoord.x - hsize.x) > hsize.x - 8) velocity = vec2(0.0f);
		if(abs(itexcoord.y - hsize.y) > hsize.y - 8) velocity = vec2(0.0f);
		
		imageStore(out_surface, itexcoord, vec4(velocity, 0.0f, 0.0f));
	}
	
#elif COMPUTE_DIFFUSE_SHADER
	
	layout(local_size_x = 8, local_size_y = 8) in;
	
	layout(std140, binding = 0) uniform DiffuseParameters {
		float viscosity;
		float ifps;
	};
	
	layout(binding = 0, set = 1) uniform texture2D in_velocity;
	layout(binding = 1, set = 1, rgba32f) uniform writeonly image2D out_surface;
	
	/*
	 */
	void main() {
		
		ivec2 size = textureSize(in_velocity, 0).xy;
		ivec2 itexcoord = ivec2(gl_GlobalInvocationID.xy);
		
		vec4 velocity = texelFetch(in_velocity, itexcoord, 0);
		
		float dx = float(itexcoord.x);
		float dy = float(itexcoord.y);
		if(dy > size.y / 2) dy -= size.y;
		
		float radius = dx * dx + dy * dy;
		velocity *= exp(-radius * viscosity * ifps);
		
		if(radius > 0.0f) {
			float r = (dx * velocity.x + dy * velocity.y);
			float i = (dx * velocity.z + dy * velocity.w);
			velocity -= vec4(r * dx, r * dy, i * dx, i * dy) / radius;
		}
		
		imageStore(out_surface, itexcoord, velocity);
	}
	
#elif COMPUTE_UPDATE_SHADER
	
	layout(local_size_x = 8, local_size_y = 8) in;
	
	layout(std140, binding = 0) uniform UpdateParameters {
		float ifps;
	};
	
	layout(binding = 0, set = 2) uniform sampler in_sampler;
	layout(binding = 0, set = 1) uniform texture2D in_velocity;
	layout(binding = 1, set = 1) uniform texture2D in_color;
	layout(binding = 2, set = 1, rgba8) uniform writeonly image2D out_surface;
	
	/*
	 */
	void main() {
		
		ivec2 size = textureSize(in_color, 0).xy;
		ivec2 itexcoord = ivec2(gl_GlobalInvocationID.xy);
		
		vec2 texcoord = (vec2(itexcoord) + 0.5f) / vec2(size);
		vec2 velocity = textureLod(sampler2D(in_velocity, in_sampler), texcoord, 0.0f).xy;
		
		texcoord -= velocity * ifps;
		vec4 color = textureLod(sampler2D(in_color, in_sampler), texcoord, 0.0f);
		
		imageStore(out_surface, itexcoord, color);
	}

#elif VERTEX_SHADER
	
	layout(location = 0) out vec2 s_texcoord;
	
	/*
	 */
	void main() {
		
		vec2 texcoord = vec2(0.0f);
		if(gl_VertexIndex == 0) texcoord.x = 2.0f;
		if(gl_VertexIndex == 2) texcoord.y = 2.0f;
		
		gl_Position = vec4(texcoord * 2.0f - 1.0f, 0.0f, 1.0f);
		
		#if CLAY_VK
			texcoord.y = 1.0f - texcoord.y;
		#endif
		
		s_texcoord = texcoord;
	}
	
#elif FRAGMENT_SHADER
	
	layout(binding = 0, set = 1) uniform sampler in_sampler;
	layout(binding = 0, set = 0) uniform texture2D in_texture;
	
	layout(location = 0) in vec2 s_texcoord;
	
	layout(location = 0) out vec4 out_color;
	
	/*
	 */
	void main() {
		
		out_color = texture(sampler2D(in_texture, in_sampler), s_texcoord);
	}
	
#endif
