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

#pragma entry(null)
#pragma target(lib_6_8)

struct Index {
	uint2 index;
};

cbuffer CommonParameters : register(b0) {
	float time;
};

RWTexture2D<unorm float4> out_surface : register(u0);

/*
 */
[shader("node")]
[nodeisprogramentry]
[nodelaunch("broadcasting")]
[nodedispatchgrid(128, 128, 1)]
[numthreads(8, 8, 1)]
void main(uint3 global_id : SV_DispatchThreadID,
	[MaxRecords(64)] NodeOutput<Index> plasma_node,
	[MaxRecords(64)] NodeOutput<Index> clear_node) {
	
	float distance = length(float2(global_id.xy) - 512.0f);
	
	// thread output
	if(distance < (0.75f + sin(time) * 0.25f) * 512.0f) {
		ThreadNodeOutputRecords<Index> OUT = plasma_node.GetThreadNodeOutputRecords(1);
		OUT[0].index = global_id.xy;
		OUT.OutputComplete();
	} else {
		ThreadNodeOutputRecords<Index> OUT = clear_node.GetThreadNodeOutputRecords(1);
		OUT[0].index = global_id.xy;
		OUT.OutputComplete();
	}
}

/*
 */
[shader("node")]
[nodelaunch("thread")]
void plasma_node(ThreadNodeInputRecord<Index> IN) {
	
	float k = time * 2.0f;
	float s = sin(time * 0.5f);
	float c = cos(time * 0.5f);
	
	float2 texcoord = (float2(IN.Get().index) / 1024.0f) - 0.5f;
	
	texcoord = float2((s * texcoord.x) + (c * texcoord.y), (c * texcoord.x) - (s * texcoord.y));
	
	float2 t = float4((texcoord * 32.0f) - 16.0f, 0.0f, 0.0f).xy;
	
	float v = (((sin(t.x + k) + sin(t.y + k)) + sin((t.x + t.y) + k)) + sin(length(t) + (k * 3.0f))) + (k * 2.0f);
	
	float4 color = float4((cos(float3(0.0f, 1.57f, 3.14f) + v) * 0.5f) + 0.5f, 1.0f);
	
	out_surface[IN.Get().index] = color;
}

/*
 */
[shader("node")]
[nodelaunch("thread")]
void clear_node(ThreadNodeInputRecord<Index> IN) {
	
	out_surface[IN.Get().index] = 0.0f;
}
