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
#include <common/sample_controls.h>
#include <format/TellusimMesh.h>
#include <platform/TellusimDevice.h>
#include <platform/TellusimKernel.h>
#include <platform/TellusimPipeline.h>
#include <platform/TellusimCompute.h>
#include <platform/TellusimCommand.h>
#include <geometry/TellusimSpatial.h>
#include <graphics/TellusimMeshModel.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	DECLARE_WINDOW
	
	// create window
	String title = String::format("%s Tellusim::ShadowTree", window.getPlatformName());
	if(!window.create(title) || !window.setHidden(false)) return 1;
	
	// structures
	struct CommonParameters {
		Matrix4x4f projection;
		Matrix4x4f modelview;
		Matrix4x4f imodelviewprojection;
		Matrix4x4f transform;
		Matrix4x4f itransform;
		Vector4f camera;
		Vector4f light;
		float32_t znear;
		float32_t radius;
		float32_t samples;
		uint32_t num_nodes;
	};
	
	// create device
	Device device(window);
	if(!device) return 1;
	
	// check compute shader support
	if(!device.hasShader(Shader::TypeCompute)) {
		TS_LOG(Error, "compute shader is not supported\n");
		return 0;
	}
	
	// create kernel
	Kernel kernel = device.createKernel().setSamplers(1).setTextures(3).setSurfaces(1).setUniforms(1).setStorages(2);
	if(!kernel.loadShaderGLSL("main.shader", "COMPUTE_SHADER=1")) return 1;
	if(!kernel.create()) return 1;
	
	// create pipeline
	Pipeline pipeline = device.createPipeline();
	pipeline.setUniformMask(0, Shader::MaskVertex);
	pipeline.addAttribute(Pipeline::AttributePosition, FormatRGBf32, 0, sizeof(float32_t) * 0, sizeof(float32_t) * 8);
	pipeline.addAttribute(Pipeline::AttributeNormal, FormatRGBf32, 0, sizeof(float32_t) * 3, sizeof(float32_t) * 8);
	pipeline.setColorFormat(0, FormatRGBAf16);
	pipeline.setDepthFormat(FormatDf32);
	pipeline.setDepthFunc(Pipeline::DepthFuncGreaterEqual);
	if(!pipeline.loadShaderGLSL(Shader::TypeVertex, "main.shader", "VERTEX_SHADER=1")) return 1;
	if(!pipeline.loadShaderGLSL(Shader::TypeFragment, "main.shader", "FRAGMENT_SHADER=1")) return 1;
	if(!pipeline.create()) return 1;
	
	// load mesh
	Mesh mesh;
	if(!mesh.load("model.glb")) return 1;
	uint32_t plane_index = mesh.findGeometry("plane");
	uint32_t caster_index = mesh.findGeometry("caster");
	if(plane_index == Maxu32 || caster_index == Maxu32) return 1;
	mesh.setBasis(Mesh::BasisZUpRight);
	
	// create model
	MeshModel model;
	if(!model.create(device, pipeline, mesh)) return 1;
	
	// geometry attribute
	MeshGeometry caster_geometry = mesh.getGeometry(caster_index);
	MeshAttribute position_attribute = caster_geometry.getAttribute(MeshAttribute::TypePosition);
	if(!position_attribute) return 1;
	
	// geometry indices
	MeshIndices position_indices = position_attribute.getIndices();
	if(!position_indices) return 1;
	
	// create spatial tree
	Array<Vector4f> positions;
	Array<Spatial::Node4f> nodes;
	uint32_t num_nodes = position_indices.getSize() / 3;
	positions.reserve(num_nodes * 3);
	nodes.reserve(num_nodes * 2);
	nodes.resize(num_nodes);
	for(uint32_t i = 0; i < position_indices.getSize(); i += 3) {
		Spatial::Node4f &node = nodes.append();
		const Vector3f &v0 = position_attribute.get<Vector3f>(position_indices.get(i + 0));
		const Vector3f &v1 = position_attribute.get<Vector3f>(position_indices.get(i + 1));
		const Vector3f &v2 = position_attribute.get<Vector3f>(position_indices.get(i + 2));
		node.bound.min = Vector4f(min(v0, v1, v2), 1.0f);
		node.bound.max = Vector4f(max(v0, v1, v2), 1.0f);
		positions.append(Vector4f(v0, 0.0f));
		positions.append(Vector4f(v1, 0.0f));
		positions.append(Vector4f(v2, 0.0f));
	}
	Spatial::create<float32_t>(nodes.get(), num_nodes);
	Spatial::optimize<float32_t>(nodes.get(), num_nodes);
	
	// create nodes buffer
	Buffer nodes_buffer = device.createBuffer(Buffer::FlagStorage, nodes.get(), nodes.bytes());
	Buffer positions_buffer = device.createBuffer(Buffer::FlagStorage, positions.get(), positions.bytes());
	if(!nodes_buffer || !positions_buffer) return 1;
	
	// create sampler
	Sampler noise_sampler = device.createSampler(Sampler::FilterPoint, Sampler::WrapModeRepeat);
	if(!noise_sampler) return 1;
	
	// create noise texture
	Texture noise_texture = device.loadTexture("noise.png");
	if(!noise_texture) return 1;
	
	// create target textures
	Texture normal_texture, depth_texture, color_surface;
	
	// create window target
	Target window_target = device.createTarget(window);
	
	// create render target
	Target render_target = device.createTarget();
	render_target.setClearDepth(0.0f);
	
	// create canvas
	Canvas canvas;
	
	// create root
	ControlRoot root(canvas, true);
	
	// create rect
	ControlRect color_rect(&root);
	color_rect.setAlign(Control::AlignExpand);
	color_rect.setMode(CanvasElement::ModeTextureFetch);
	color_rect.setBlend(Pipeline::BlendOpAdd, Pipeline::BlendFuncOne, Pipeline::BlendFuncZero);
	
	// create panel
	ControlPanel panel(&root, 1, 8.0f, 8.0f);
	panel.setAlign(Control::AlignRightTop);
	panel.setPosition(-8.0f, -8.0f);
	
	// create sliders
	ControlSlider radius_slider(&panel, "Radius", 2, 1.0f, 0.0f, 2.0f);
	ControlSlider samples_slider(&panel, "Samples", 0, 3.0f, 1.0f, 5.0f);
	radius_slider.setSize(192.0f, 0.0f);
	samples_slider.setSize(192.0f, 0.0f);
	
	float32_t animation_time = 0.0f;
	float32_t old_animation_time = 0.0f;
	bool animation = !app.isArgument("pause");
	
	// main loop
	DECLARE_GLOBAL
	window.run([&]() -> bool {
		DECLARE_COMMON
		
		using Tellusim::sin;
		
		Window::update();
		
		if(!window.render()) return false;
		
		// window title
		if(fps > 0.0f) window.setTitle(String::format("%s %.1f FPS", title.get(), fps));
		
		// resize target textures
		if(!color_surface || color_surface.getWidth() != window.getWidth() || color_surface.getHeight() != window.getHeight()) {
			device.releaseTexture(depth_texture);
			device.releaseTexture(normal_texture);
			device.releaseTexture(color_surface);
			depth_texture = device.createTexture2D(FormatDf32, window.getWidth(), window.getHeight(), Texture::FlagTarget);
			normal_texture = device.createTexture2D(FormatRGBAf16, window.getWidth(), window.getHeight(), Texture::FlagTarget);
			color_surface = device.createTexture2D(FormatRGBAu8n, window.getWidth(), window.getHeight(), Texture::FlagSurface);
			if(!depth_texture || !normal_texture || !color_surface) return false;
			color_rect.setTexture(color_surface);
		}
		
		// update controls
		update_controls(window, root);
		canvas.create(device, window_target);
		
		// animation time
		if(window.getKeyboardKey(' ', true)) animation = !animation;
		if(animation) animation_time += time - old_animation_time;
		old_animation_time = time;
		
		// shadow caster transform
		Matrix4x4f caster_transform = Matrix4x4f::rotateZ(sin(animation_time * 0.5f) * 15.0f + 90.0f);
		Matrix4x4f plane_transform = Matrix4x4f::translate(-4.5f, 0.0f, -1.2f);
		
		// common parameters
		CommonParameters common_parameters;
		common_parameters.znear = 0.1f;
		common_parameters.camera = Vector4f(-4.5f, 4.5f, 1.0f, 0.0f);
		common_parameters.light = Vector4f(4.0f, 0.0f, 2.3f, 0.0f);
		common_parameters.projection = Matrix4x4f::perspective(60.0f, (float32_t)window.getWidth() / window.getHeight(), common_parameters.znear, true);
		common_parameters.modelview = Matrix4x4f::lookAt(Vector3f(common_parameters.camera), Vector3f(-4.5f, 0.0f, -2.0f), Vector3f(0.0f, 0.0f, 1.0f));
		if(render_target.isFlipped()) common_parameters.projection = Matrix4x4f::scale(1.0f, -1.0f, 1.0f) * common_parameters.projection;
		common_parameters.radius = radius_slider.getValuef32() * 0.02f;
		common_parameters.samples = samples_slider.getValuef32();
		common_parameters.num_nodes = num_nodes;
		
		// flush textures
		device.flushTextures({ depth_texture, normal_texture }, Texture::FlagTarget);
		
		// render target
		render_target.setColorTexture(normal_texture);
		render_target.setDepthTexture(depth_texture);
		render_target.begin();
		{
			// create command list
			Command command = device.createCommand(render_target);
			
			// set pipeline
			command.setPipeline(pipeline);
			
			// set model buffers
			model.setBuffers(command);
			
			// draw plane
			common_parameters.transform = plane_transform;
			command.setUniform(0, common_parameters);
			model.draw(command, plane_index);
			
			// draw shadow caster
			common_parameters.transform = caster_transform;
			command.setUniform(0, common_parameters);
			model.draw(command, caster_index);
		}
		render_target.end();
		
		device.flushTextures({ depth_texture, normal_texture });
		
		// shadow kernel
		{
			// create command list
			Compute compute = device.createCompute();
			
			// set kernel
			compute.setKernel(kernel);
			common_parameters.imodelviewprojection = inverse(common_parameters.projection * common_parameters.modelview);
			common_parameters.itransform = inverse(caster_transform);
			compute.setUniform(0, common_parameters);
			compute.setSampler(0, noise_sampler);
			compute.setTextures(0, { depth_texture, normal_texture, noise_texture });
			compute.setStorageBuffers(0, { nodes_buffer, positions_buffer });
			compute.setSurfaceTexture(0, color_surface);
			
			// dispatch kernel
			compute.dispatch(color_surface);
			compute.barrier(color_surface);
		}
		
		// flush texture
		device.flushTexture(color_surface);
		
		// window target
		window_target.begin();
		{
			// create command list
			Command command = device.createCommand(window_target);
			
			// draw canvas
			canvas.draw(command, window_target);
		}
		window_target.end();
		
		if(!window.present()) return false;
		
		if(!device.check()) return false;
		
		return true;
	});
	
	// finish context
	window.finish();
	
	return 0;
}
