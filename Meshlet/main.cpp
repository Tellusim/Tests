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
#include <format/TellusimMesh.h>
#include <platform/TellusimDevice.h>
#include <platform/TellusimKernel.h>
#include <platform/TellusimPipeline.h>
#include <platform/TellusimCompute.h>
#include <platform/TellusimCommand.h>
#include <geometry/TellusimMeshRefine.h>
#include <graphics/TellusimMeshModel.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	DECLARE_WINDOW
	
	// create window
	String title = String::format("%s Tellusim::Meshlet", window.getPlatformName());
	if(!window.create(title) || !window.setHidden(false)) return 1;
	
	// scene size
	constexpr int32_t grid_size = 4;
	constexpr uint32_t num_instances = grid_size * 2 + 1;
	constexpr uint32_t num_instances2 = num_instances * num_instances;
	
	// mesh parameters
	constexpr uint32_t group_size = 32;
	constexpr uint32_t max_vertices = 64;
	constexpr uint32_t max_primitives = 126;
	MeshModel::Flags mesh_flags = MeshModel::FlagMeshlet64x126;
	
	// structures
	struct Vertex {
		float32_t position[4];
		float32_t normal[4];
	};
	
	struct CommonParameters {
		Matrix4x4f projection;
		Matrix4x4f modelview;
		Vector4f camera;
	};
	
	struct ComputeParameters {
		uint32_t num_meshlets;
		uint32_t group_offset;
		Vector2f surface_size;
		float32_t surface_stride;
	};
	
	// create device
	Device device(window);
	if(!device) return 1;
	
	// create common pipeline
	Pipeline common_pipeline = device.createPipeline();
	common_pipeline.setColorFormat(window.getColorFormat());
	common_pipeline.setDepthFormat(window.getDepthFormat());
	common_pipeline.setDepthFunc(Pipeline::DepthFuncGreater);
	common_pipeline.setCullMode((window.getPlatform() == PlatformVK) ? Pipeline::CullModeFront : Pipeline::CullModeBack);
	
	// vertex pipeline
	Pipeline vertex_pipeline = device.createPipeline(common_pipeline);
	vertex_pipeline.setUniformMask(0, Shader::MaskVertex);
	vertex_pipeline.setUniformMask(1, Shader::MaskVertex);
	vertex_pipeline.addAttribute(Pipeline::AttributePosition, FormatRGBf32, 0, offsetof(Vertex, position), sizeof(Vertex));
	vertex_pipeline.addAttribute(Pipeline::AttributeNormal, FormatRGBf32, 0, offsetof(Vertex, normal), sizeof(Vertex));
	if(!vertex_pipeline.loadShaderGLSL(Shader::TypeVertex, "main.shader", "VERTEX_PIPELINE=1; VERTEX_SHADER=1; NUM_INSTANCES=%uu", num_instances2)) return 1;
	if(!vertex_pipeline.loadShaderGLSL(Shader::TypeFragment, "main.shader", "VERTEX_PIPELINE=1; FRAGMENT_SHADER=1")) return 1;
	if(!vertex_pipeline.create()) return 1;
	
	// mesh pipeline
	Pipeline mesh_pipeline;
	if(device.hasShader(Shader::TypeMesh)) {
		mesh_pipeline = device.createPipeline(common_pipeline);
		mesh_pipeline.setUniformMask(0, Shader::MaskMesh);
		mesh_pipeline.setUniformMask(1, Shader::MaskTask | Shader::MaskMesh);
		mesh_pipeline.setStorageMasks(0, 3, Shader::MaskMesh);
		if(!mesh_pipeline.loadShaderGLSL(Shader::TypeTask, "main.shader", "MESH_PIPELINE=1; TASK_SHADER=1")) return 1;
		if(!mesh_pipeline.loadShaderGLSL(Shader::TypeMesh, "main.shader", "MESH_PIPELINE=1; MESH_SHADER=1; GROUP_SIZE=%uu; NUM_VERTICES=%uu; NUM_PRIMITIVES=%uu; NUM_INSTANCES=%uu", group_size, max_vertices, max_primitives, num_instances2)) return 1;
		if(!mesh_pipeline.loadShaderGLSL(Shader::TypeFragment, "main.shader", "MESH_PIPELINE=1; FRAGMENT_SHADER=1")) return 1;
		if(!mesh_pipeline.create()) return 1;
	}
	
	// compute pipeline
	Kernel draw_kernel;
	Kernel clear_kernel;
	Pipeline compute_pipeline;
	if(device.hasShader(Shader::TypeCompute)) {
		
		// create compute pipeline
		compute_pipeline = device.createPipeline();
		compute_pipeline.setTextureMask(0, Shader::MaskFragment);
		compute_pipeline.setColorFormat(window.getColorFormat());
		compute_pipeline.setDepthFormat(window.getDepthFormat());
		if(!compute_pipeline.loadShaderGLSL(Shader::TypeVertex, "main.shader", "COMPUTE_PIPELINE=1; VERTEX_SHADER=1")) return 1;
		if(!compute_pipeline.loadShaderGLSL(Shader::TypeFragment, "main.shader", "COMPUTE_PIPELINE=1; FRAGMENT_SHADER=1")) return 1;
		if(!compute_pipeline.create()) return 1;
		
		// create draw kernel
		draw_kernel = device.createKernel().setSurfaces(2).setUniforms(2).setStorages(3);
		if(!draw_kernel.loadShaderGLSL("main.shader", "COMPUTE_PIPELINE=1; COMPUTE_DRAW_SHADER=1; GROUP_SIZE=%uu; NUM_VERTICES=%uu; NUM_PRIMITIVES=%uu; NUM_INSTANCES=%uu", npot(max_primitives), max_vertices, max_primitives, num_instances2)) return 1;
		if(!draw_kernel.create()) return 1;
		
		// create clear kernel
		clear_kernel = device.createKernel().setUniforms(1).setSurfaces(1);
		if(!clear_kernel.loadShaderGLSL("main.shader", "COMPUTE_PIPELINE=1; COMPUTE_CLEAR_SHADER=1")) return 1;
		if(!clear_kernel.create()) return 1;
	}
	
	// load mesh
	Mesh mesh, src_mesh;
	if(!src_mesh.load("model.glb")) return 1;
	if(!MeshRefine::subdiv(mesh, src_mesh, 5)) return 1;
	mesh.createNormals();
	mesh.createIslands(max_vertices, max_primitives);
	
	// create vertex model
	MeshModel vertex_model;
	if(!vertex_model.create(device, vertex_pipeline, mesh, MeshModel::DefaultFlags)) return 1;
	
	// create mesh model
	MeshModel mesh_model;
	if(!mesh_model.create(device, vertex_pipeline, mesh, MeshModel::DefaultFlags | mesh_flags)) return 1;
	Buffer mesh_vertex_buffer = mesh_model.getVertexBuffer();
	Buffer mesh_meshlet_buffer = mesh_model.getMeshletBuffer();
	
	// mesh info
	uint32_t num_meshlets = mesh_model.getNumMeshlets();
	uint32_t num_vertices = num_instances2 * vertex_model.getNumVertices();
	uint32_t num_primitives = num_instances2 * vertex_model.getNumIndices() / 3;
	TS_LOGF(Message, "  Vertices: %s\n", String::fromNumber(num_vertices).get());
	TS_LOGF(Message, "Primitives: %s\n", String::fromNumber(num_primitives).get());
	TS_LOGF(Message, "  Meshlets: %u (%u)\n", num_meshlets * num_instances2, num_meshlets);
	TS_LOGF(Message, " Instances: %u\n", num_instances2);
	TS_LOGF(Message, " GroupSize: %u\n", group_size);
	
	// compute surfaces
	Texture depth_surface;
	Texture color_surface;
	
	// create target
	Target target = device.createTarget(window);
	target.setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	target.setClearDepth(0.0f);
	
	// current mode
	enum Mode {
		ModeVertex = 0,
		ModeMesh,
		ModeCompute,
		NumModes,
	};
	Mode mode = ModeVertex;
	if(mesh_pipeline) mode = ModeMesh;
	if(compute_pipeline) mode = ModeCompute;
	
	const char *mode_names[] = { "Vertex", "Mesh", "Compute" };
	TS_STATIC_ASSERT(TS_COUNTOF(mode_names) == NumModes);
	
	// instance transforms
	Array<Matrix4x3f> transforms(num_instances2);
	
	// main loop
	DECLARE_GLOBAL
	window.run([&]() -> bool {
		DECLARE_COMMON
		
		using Tellusim::sin;
		
		Window::update();
		
		if(!window.render()) return false;
		
		// switch mode
		if(window.getKeyboardKey('1')) mode = ModeVertex;
		else if(window.getKeyboardKey('2') && mesh_pipeline) mode = ModeMesh;
		else if(window.getKeyboardKey('3') && compute_pipeline) mode = ModeCompute;
		
		// window title
		if(fps > 0.0f) window.setTitle(String::format("%s %s %.1f FPS", title.get(), mode_names[mode], fps));
		
		// common parameters
		CommonParameters common_parameters;
		common_parameters.camera = Vector4f(4.0f + grid_size * 3.0f, 0.0f, 1.0f, 0.0f);
		common_parameters.projection = Matrix4x4f::perspective(60.0f, (float32_t)window.getWidth() / window.getHeight(), 0.1f, true);
		common_parameters.modelview = Matrix4x4f::lookAt(Vector3f(common_parameters.camera), Vector3f(common_parameters.camera) + Vector3f(-16.0f, 0.0f, -4.0f), Vector3f(0.0f, 0.0f, 1.0f));
		if(target.isFlipped()) common_parameters.projection = Matrix4x4f::scale(1.0f, -1.0f, 1.0f) * common_parameters.projection;
		
		// transform parameters
		transforms.clear();
		for(int32_t y = -grid_size; y <= grid_size; y++) {
			for(int32_t x = -grid_size; x <= grid_size; x++) {
				Matrix4x3f translate = Matrix4x3f::translate(x * 3.2f, y * 3.2f, 0.0f);
				Matrix4x3f rotate = Matrix4x3f::rotateZ(time * 32.0f + y * 2715.53f) * Matrix4x3f::rotateX(time * 16.0f + x * 9774.37f);
				Matrix4x3f scale = Matrix4x3f::scale(sin(time + (x ^ y) * 13.73f) * 0.2f + 0.8f);
				transforms.append(translate * rotate * scale);
			}
		}
		
		// compute rasterization
		if(mode == ModeCompute) {
			
			// create surfaces
			if(!depth_surface || depth_surface.getWidth() != window.getWidth() || depth_surface.getHeight() != window.getHeight()) {
				window.finish();
				depth_surface = device.createTexture2D(FormatRu32, window.getWidth(), window.getHeight(), Texture::FlagSurface | Texture::FlagBuffer);
				color_surface = device.createTexture2D(FormatRu32, window.getWidth(), window.getHeight(), Texture::FlagSurface | Texture::FlagBuffer);
			}
			
			// create command list
			Compute compute = device.createCompute();
			
			// clear depth surface
			compute.setKernel(clear_kernel);
			compute.setUniform(0, 0.0f);
			compute.setSurfaceTexture(0, depth_surface);
			compute.dispatch(depth_surface);
			compute.barrier(depth_surface);
			
			// clear color texture
			compute.setKernel(clear_kernel);
			compute.setUniform(0, target.getClearColor().getRGBAu8());
			compute.setSurfaceTexture(0, color_surface);
			compute.dispatch(color_surface);
			compute.barrier(color_surface);
			
			// compute parameters
			ComputeParameters compute_parameters;
			compute_parameters.num_meshlets = num_meshlets;
			compute_parameters.surface_size = Vector2f(Vector2u(window.getWidth(), window.getHeight()));
			compute_parameters.surface_stride = (float32_t)TS_ALIGN64(window.getWidth());
			
			// dispatch compute kernel
			compute.setKernel(draw_kernel);
			compute.setUniform(0, common_parameters);
			compute.setSurfaceTextures(0, { depth_surface, color_surface });
			compute.setStorageData(0, transforms.get(), transforms.bytes());
			compute.setStorageBuffers(1, { mesh_vertex_buffer, mesh_meshlet_buffer });
			uint32_t max_groups = device.getFeatures().maxGroupCountX;
			for(uint32_t i = 0; i < num_meshlets * num_instances2; i += max_groups) {
				uint32_t size = min(num_meshlets * num_instances2 - i, max_groups);
				compute_parameters.group_offset = i;
				compute.setUniform(1, compute_parameters);
				compute.dispatch(npot(max_primitives) * size);
			}
			
			// flush surface
			device.flushTextures({ depth_surface, color_surface });
		}
		
		// window target
		target.begin();
		{
			// create command list
			Command command = device.createCommand(target);
			
			// mesh pipeline
			if(mode == ModeMesh) {
				command.setPipeline(mesh_pipeline);
				command.setUniform(0, common_parameters);
				command.setStorageData(0, transforms.get(), transforms.bytes());
				command.setStorageBuffers(1, { mesh_vertex_buffer, mesh_meshlet_buffer });
				uint32_t max_meshlets = device.getFeatures().maxTaskMeshes;
				for(uint32_t i = 0; i < num_meshlets; i += max_meshlets) {
					uint32_t size = min(num_meshlets - i, max_meshlets);
					command.setUniform(1, Vector2u(size, i));
					if(window.getKeyboardKey('i')) {
						command.setIndirect(Command::DrawMeshIndirect { num_instances2, 1, 1 });
						command.drawMeshIndirect(1);
					} else {
						command.drawMesh(num_instances2);
					}
				}
			}
			// compute pipeline
			else if(mode == ModeCompute) {
				command.setPipeline(compute_pipeline);
				command.setTexture(0, color_surface);
				command.drawArrays(3);
			}
			// vertex pipeline
			else {
				command.setPipeline(vertex_pipeline);
				command.setUniform(0, common_parameters);
				command.setUniformData(1, transforms.get(), transforms.bytes());
				vertex_model.setBuffers(command);
				vertex_model.drawInstanced(command, 0, num_instances2);
			}
		}
		target.end();
		
		if(!window.present()) return false;
		
		return true;
	});
	
	// finish context
	window.finish();
	
	return 0;
}
