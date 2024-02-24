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

#include <common/common.h>
#include <platform/TellusimDevice.h>
#include <platform/TellusimContext.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	// create app
	App app(argc, argv);
	if(!app.create()) return 1;
	
	// create context
	Context context(app.getPlatform(), app.getDevice());
	if(!context || !context.create()) return 1;
	
	// create device
	Device device(context);
	if(!device) return 1;
	
	// device name
	TS_LOGF(Message, "%s (%s)\n", device.getName().get(), device.getPlatformName());
	if(device.getVendor()) TS_LOGF(Message, "Vendor: %s\n", device.getVendor().get());
	if(device.getVersion()) TS_LOGF(Message, "Version: %s\n", device.getVersion().get());
	
	// device features
	const Device::Features &features = device.getFeatures();
	
	#define DECLARE_PRINT_BOOL(NAME) TS_LOGF(Message, #NAME ": %s\n", features.NAME ? "true" : "false")
	#define DECLARE_PRINT_UINT(NAME) TS_LOGF(Message, #NAME ": %u\n", features.NAME)
	#define DECLARE_PRINT_UHEX(NAME) TS_LOGF(Message, #NAME ": 0x%x\n", features.NAME)
	#define DECLARE_PRINT_UXYZ(NAME) TS_LOGF(Message, #NAME ": %u %u %u\n", features.NAME ## X, features.NAME ## Y, features.NAME ## Z)
	#define DECLARE_PRINT_SIZE(NAME) TS_LOGF(Message, #NAME ": %s\n", String::fromBytes(features.NAME).get())
	
	DECLARE_PRINT_BOOL(threadAccess);
	
	DECLARE_PRINT_BOOL(sparseBuffer);
	
	DECLARE_PRINT_BOOL(sparseTexture);
	DECLARE_PRINT_BOOL(sparseArrayTexture);
	DECLARE_PRINT_BOOL(cubeArrayTexture);
	DECLARE_PRINT_BOOL(textureTable);
	
	DECLARE_PRINT_BOOL(baseInstanceIndex);
	DECLARE_PRINT_BOOL(drawIndirectIndex);
	DECLARE_PRINT_BOOL(drawIndirectCount);
	DECLARE_PRINT_BOOL(taskIndirectCount);
	
	DECLARE_PRINT_BOOL(vertexStorage);
	DECLARE_PRINT_BOOL(vertexIndexLayer);
	DECLARE_PRINT_BOOL(geometryPassthrough);
	DECLARE_PRINT_BOOL(fragmentStencilExport);
	
	DECLARE_PRINT_BOOL(dualSourceBlending);
	DECLARE_PRINT_BOOL(depthRangeOneToOne);
	
	DECLARE_PRINT_BOOL(conservativeRaster);
	DECLARE_PRINT_BOOL(viewportSwizzle);
	
	DECLARE_PRINT_BOOL(conditionalRendering);
	
	DECLARE_PRINT_BOOL(rayTracing);
	DECLARE_PRINT_BOOL(computeTracing);
	DECLARE_PRINT_BOOL(fragmentTracing);
	DECLARE_PRINT_BOOL(indirectTracing);
	DECLARE_PRINT_UINT(recursionDepth);
	
	DECLARE_PRINT_BOOL(subgroupVote);
	DECLARE_PRINT_BOOL(subgroupMath);
	DECLARE_PRINT_BOOL(subgroupShuffle);
	DECLARE_PRINT_UINT(subgroupSize);
	
	DECLARE_PRINT_BOOL(shaderu8);
	DECLARE_PRINT_BOOL(shaderf16);
	DECLARE_PRINT_BOOL(shaderu16);
	DECLARE_PRINT_BOOL(shaderf64);
	DECLARE_PRINT_BOOL(shaderu64);
	
	DECLARE_PRINT_BOOL(atomicGroupf32);
	DECLARE_PRINT_BOOL(atomicGroupu64);
	DECLARE_PRINT_BOOL(atomicBufferf32);
	DECLARE_PRINT_BOOL(atomicBufferu64);
	DECLARE_PRINT_BOOL(atomicTexturef32);
	DECLARE_PRINT_BOOL(atomicTextureu32);
	DECLARE_PRINT_BOOL(atomicTextureu64);
	
	DECLARE_PRINT_BOOL(matrix16f16);
	DECLARE_PRINT_BOOL(matrix16x8x8f16);
	DECLARE_PRINT_BOOL(matrix16x8x16f16);
	DECLARE_PRINT_BOOL(matrix16f16f32);
	DECLARE_PRINT_BOOL(matrix16x8x8f16f32);
	DECLARE_PRINT_BOOL(matrix16x8x16f16f32);
	
	DECLARE_PRINT_UINT(uniformAlignment);
	DECLARE_PRINT_UINT(storageAlignment);
	
	DECLARE_PRINT_UINT(maxTextureSamples);
	DECLARE_PRINT_UINT(maxTexture2DSize);
	DECLARE_PRINT_UINT(maxTexture3DSize);
	DECLARE_PRINT_UINT(maxTextureLayers);
	
	DECLARE_PRINT_UXYZ(maxGroupSize);
	DECLARE_PRINT_UXYZ(maxGroupCount);
	
	DECLARE_PRINT_UINT(maxTaskCount);
	DECLARE_PRINT_SIZE(maxTaskMemory);
	DECLARE_PRINT_UINT(maxTaskMeshes);
	
	DECLARE_PRINT_SIZE(maxMeshMemory);
	DECLARE_PRINT_UINT(maxMeshVertices);
	DECLARE_PRINT_UINT(maxMeshPrimitives);
	
	DECLARE_PRINT_UINT(maxViewportCount);
	DECLARE_PRINT_UINT(maxClipCullCount);
	
	DECLARE_PRINT_SIZE(maxUniformSize);
	DECLARE_PRINT_SIZE(maxStorageSize);
	
	DECLARE_PRINT_SIZE(groupMemory);
	DECLARE_PRINT_SIZE(videoMemory);
	
	DECLARE_PRINT_UHEX(vendorID);
	DECLARE_PRINT_UHEX(deviceID);
	DECLARE_PRINT_UHEX(pciBusID);
	DECLARE_PRINT_UHEX(pciDomainID);
	DECLARE_PRINT_UHEX(pciDeviceID);
	
	// finish context
	context.finish();
	
	return 0;
}
