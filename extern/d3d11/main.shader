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

/*
 */
#if VERTEX_SHADER
	
	cbuffer CommonParameters : register(b0) {
		row_major float4x4 projection;
		row_major float4x4 modelview;
		row_major float4x4 transform;
		float4 camera;
	};
	
	struct ShaderIn {
		float4 position : Position;
		float3 normal : TexCoord0;
	};
	
	struct ShaderOut {
		float3 direction : TexCoord0;
		float3 normal : TexCoord1;
		float4 position : SV_Position;
	};
	
	/*
	 */
	ShaderOut main(ShaderIn IN) {
		
		ShaderOut OUT;
		
		float4 position = mul(transform, IN.position);
		
		OUT.position = mul(projection, mul(modelview, position));
		OUT.direction = camera.xyz - position.xyz;
		OUT.normal = mul(transform, float4(IN.normal, 0.0)).xyz;
		
		return OUT;
	}
	
#elif FRAGMENT_SHADER
	
	struct ShaderIn {
		float3 direction : TexCoord0;
		float3 normal : TexCoord1;
	};
	
	/*
	 */
	float4 main(ShaderIn IN) : SV_Target0 {
		
		float3 direction = normalize(IN.direction);
		float3 normal = normalize(IN.normal);
		
		float diffuse = clamp(dot(direction, normal), 0.0, 1.0);
		float specular = pow(clamp(dot(reflect(-direction, normal), direction), 0.0, 1.0), 32.0);
		float color = diffuse + specular;
		
		return float4(color, color, color, 1.0);
	}
	
#endif
