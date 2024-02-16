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

#version 460 core

/*
 */
#if VERTEX_SHADER
	
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
	
	layout(binding = 0, set = 0) uniform texture2D in_texture;
	
	layout(location = 0) in vec2 s_texcoord;
	
	layout(location = 0) out vec4 out_color;
	
	/*
	 */
	void main() {
		
		ivec2 size = textureSize(in_texture, 0);
		
		ivec2 texcoord = ivec2(s_texcoord * size);
		
		out_color = texelFetch(in_texture, texcoord, 0);
	}
	
#elif COMPUTE_SHADER
	
	#define GROUP_SIZE	8u
	
	layout(local_size_x = GROUP_SIZE, local_size_y = GROUP_SIZE) in;
	
	layout(row_major, binding = 0) uniform common_parameters {
		mat4 projection;
		mat4 imodelview;
		vec4 camera;
		vec4 light;
	};
	
	layout(std430, binding = 1) readonly buffer VertexBuffer { vec4 vertex_buffer[]; };
	layout(std430, binding = 2) readonly buffer IndexBuffer { uint index_buffer[]; };
	
	layout(binding = 0, set = 1, rgba8) uniform writeonly image2D out_surface;
	
	layout(binding = 0, set = 2) uniform accelerationStructureEXT tracing;
	
	/*
	 */
	void main() {
		
		ivec2 global_id = ivec2(gl_GlobalInvocationID.xy);
		
		ivec2 surface_size = imageSize(out_surface);
		
		// ray parameters
		vec3 position = (imodelview * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
		float x = ((global_id.x + 0.5f) / float(surface_size.x) * 2.0f - 1.0f + projection[2].x) / projection[0].x;
		float y = ((global_id.y + 0.5f) / float(surface_size.y) * 2.0f - 1.0f + projection[2].y) / projection[1].y;
		vec3 direction = normalize((imodelview * vec4(x, y, -1.0f, 1.0f)).xyz - position);
		
		// closest intersection
		rayQueryEXT ray_query;
		rayQueryInitializeEXT(ray_query, tracing, gl_RayFlagsOpaqueEXT, 0xff, position, 0.0f, direction, 1000.0f);
		while(rayQueryProceedEXT(ray_query)) {
			if(rayQueryGetIntersectionTypeEXT(ray_query, false) == gl_RayQueryCandidateIntersectionTriangleEXT) {
				rayQueryConfirmIntersectionEXT(ray_query);
			}
		}
		
		// background color
		vec3 color = vec3(0.2f);
		
		// plane position
		vec3 plane_position = position - direction * (position.z / direction.z);
		
		// intersection data
		[[branch]] if(rayQueryGetIntersectionTypeEXT(ray_query, true) != gl_RayQueryCommittedIntersectionNoneEXT) {
			
			position += direction * rayQueryGetIntersectionTEXT(ray_query, true);
			
			direction = -direction;
			
			vec3 light_direction = normalize(light.xyz - position);
			
			// shadow intersection
			rayQueryEXT shadow_ray_query;
			rayQueryInitializeEXT(shadow_ray_query, tracing, gl_RayFlagsTerminateOnFirstHitEXT, 0xff, position, 0.001f, light_direction, 1000.0f);
			while(rayQueryProceedEXT(shadow_ray_query)) {
				if(rayQueryGetIntersectionTypeEXT(shadow_ray_query, false) == gl_RayQueryCandidateIntersectionTriangleEXT) {
					rayQueryConfirmIntersectionEXT(shadow_ray_query);
				}
			}
			float shadow_value = (rayQueryGetIntersectionTypeEXT(shadow_ray_query, true) != gl_RayQueryCommittedIntersectionNoneEXT) ? 0.1f : 1.0f;
			
			uint index = rayQueryGetIntersectionPrimitiveIndexEXT(ray_query, true) * 3u;
			vec2 texcoord = rayQueryGetIntersectionBarycentricsEXT(ray_query, true);
			mat4x3 transform = rayQueryGetIntersectionObjectToWorldEXT(ray_query, true);
			
			vec3 normal_0 = vertex_buffer[index_buffer[index + 0u] * 2u + 1u].xyz;
			vec3 normal_1 = vertex_buffer[index_buffer[index + 1u] * 2u + 1u].xyz;
			vec3 normal_2 = vertex_buffer[index_buffer[index + 2u] * 2u + 1u].xyz;
			vec3 normal = normal_0 + (normal_1 - normal_0) * texcoord.x + (normal_2 - normal_0) * texcoord.y;
			normal = normalize(transform[0].xyz * normal.x + transform[1].xyz * normal.y + transform[2].xyz * normal.z);
			
			float diffuse = clamp(dot(light_direction, normal), 0.0f, 1.0f);
			float specular = pow(clamp(dot(reflect(-light_direction, normal), direction), 0.0f, 1.0f), 16.0f);
			
			float seed = rayQueryGetIntersectionInstanceIdEXT(ray_query, true) * 13.7351f;
			vec3 instance_color = cos(vec3(vec3(1.0f, 0.5f, 0.0f) * 3.14f + seed)) * 0.4f + 0.6f;
			
			color = (instance_color * diffuse + specular) * shadow_value;
		}
		// plane intersection
		else if(direction.z < 0.0f && abs(plane_position.x) < 20.0f && abs(plane_position.y) < 20.0f) {
			
			position = plane_position;
			
			direction = -direction;
			
			vec3 light_direction = normalize(light.xyz - position);
			
			// shadow intersection
			rayQueryEXT shadow_ray_query;
			rayQueryInitializeEXT(shadow_ray_query, tracing, gl_RayFlagsTerminateOnFirstHitEXT, 0xff, position, 0.0f, light_direction, 1000.0f);
			while(rayQueryProceedEXT(shadow_ray_query)) {
				if(rayQueryGetIntersectionTypeEXT(shadow_ray_query, false) == gl_RayQueryCandidateIntersectionTriangleEXT) {
					rayQueryConfirmIntersectionEXT(shadow_ray_query);
				}
			}
			float shadow_value = (rayQueryGetIntersectionTypeEXT(shadow_ray_query, true) != gl_RayQueryCommittedIntersectionNoneEXT) ? 0.1f : 1.0f;
			
			vec3 normal = vec3(0.0f, 0.0f, 1.0f);
			
			float diffuse = clamp(dot(light_direction, normal), 0.0f, 1.0f);
			float specular = pow(clamp(dot(reflect(-light_direction, normal), direction), 0.0f, 1.0f), 16.0f);
			
			ivec2 index = ivec2(position.xy / 2.0f - 64.0f) & 0x01;
			vec3 plane_color = vec3(((index.x ^ index.y) == 0) ? 0.6f : 0.4f);
			
			color = (plane_color * diffuse + specular) * shadow_value;
		}
		
		if(all(lessThan(global_id, surface_size))) {
			imageStore(out_surface, global_id, vec4(color, 1.0f));
		}
	}
	
#endif
