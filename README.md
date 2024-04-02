# [Tellusim Core SDK Tests](https://tellusim.com/core-sdk/)

C++, C#, Rust, Python, Swift, Java, Kotlin

Direct3D12, Direct3D11, Metal, Vulkan, OpenGL, OpenGLES, WebGL, WebGPU

Windows, Linux, macOS, Android, iOS, Web

https://tellusim.com/

---

## Clustered Lights

Forward shading with 16384 dynamic lights. This algorithm is compatible with deferred shading and transparent objects.

![Lights](graphics/lights/graphics_lights.jpg)

## Meshlet Render

A massive meshlets rendering example with Mesh Shader for hardware and Compute Shader for software rasterization.

![Meshlet](graphics/meshlet/graphics_meshlet.jpg)

## Mesh RayTracing

Traversal class for the simple raytracing pipeline access. Vulkan or Direct3D12 API is required.

![Traversal](graphics/traversal/graphics_traversal.jpg)

## Mesh Tracing

Ray Query raytracing of animated scene. Vulkan, Direct3D12, or Metal API is required.

![Tracing](graphics/tracing/graphics_tracing.jpg)

## Separable Filter

The SeparableFilter interface helps to create different separable filters like Gaussian, Sobel, Box, and custom weights.

![Separable Filter](graphics/separable_filter/graphics_separable_filter.png)

## Geometry Tessellation

Quadrilateral tessellation with Control + Evaluate or Mesh Shaders.

![Tessellation](graphics/tessellation/graphics_tessellation.jpg)

## Ordered Independent Transparency

Ordered Independent Transparency with atomic buffer operations from the fragment shader.

![Transparency](graphics/transparency/graphics_transparency.png)

## Hardware Ray Tracing Shadows

Hardware raytracing shadows with simple deferred shading. An API with Ray Query support is required.

![Shadow Tracing](graphics/shadow_tracing/graphics_shadow_tracing.png)

## Software Ray Tracing Shadows

Software raytracing shadows with simple deferred shading. Compatible with all APIs.

![Shadow Tree](graphics/shadow_tree/graphics_shadow_tree.png)

## Percentage Closer Filtering Shadow Map

PCF Shadow Maps provides shadow with variable penumbra size based on the distance between the shadow caster and receiver.

![Shadow PCF](graphics/shadow_pcf/graphics_shadow_pcf.png)

## Exponential Shadow Maps

Exponential Shadow Maps provides noise-free shadows with a fixed performance cost and constant penumbra size.

![Shadow ESM](graphics/shadow_esm/graphics_shadow_esm.png)

## Parallax 2D

Parallax occlusion mapping with self-shadowing for the mesh geometry.

![Parallax 2D](graphics/parallax_2d/graphics_parallax_2d.png)

## Parallax Cube

Parallax occlusion mapping with self-shadowing for the analytical sphere.

![Parallax Cube](graphics/parallax_cube/graphics_parallax_cube.png)

## Mesh Tangent Basis

Mesh tangent basis renormalization at the Fragment Shader. Mesh class can re-create normal and tangent vectors.

![Tangent](graphics/tangent/graphics_tangent.png)

## Mesh Model

MeshModel class creates a rendering model representation for the input Mesh or MeshGeometry with a specified Pipeline layout.

![Model](graphics/model/graphics_model.png)

## Mesh Skinned

This is a basic skinned mesh animation example. Mesh classes give comprehensive access to all mesh data, including Nodes, Animations, Materials, Cameras, and Lights.

![Mesh Skinned](graphics/skinned/graphics_skinned.png)

## Line Rendering

Simple antialiased line rendering. The Vertex shader creates a screen-aligned billboard, and the fragment shader calculates the distance to the line.

![Line](graphics/line/graphics_line.png)

## Texture Compression

Real-time BC1-BC7 texture encoder using compute shader.

![Texture](graphics/encoder/texture.png)

---

## Mesh Reduce

MeshReduce is a simple way to simplify input mesh geometry. Simplification preserves all Mesh Attributes, and it is compatible with Skinning Animation.

![Mesh Reduce](geometry/reduce/geometry_reduce.png)

## Mesh Refine

MeshRefine refines geometry mesh using Catmull-Clark (for quadrilaterals) or Loop (for triangles) subdivision algorithms. The Crease Attribute allows additional control over the subdivision process.

![Mesh Refine](geometry/refine/geometry_refine.png)

---

## Parallel TensorGraph from PyTorch

Simple convolutional autoencoder trained with PyTorch and imported into TensorGraph.

![Tensor Torch](parallel/tensor_torch/parallel_tensor_torch.jpg)

## Parallel TensorGraph MNIST Digits Recognition

MNIST Digits Recognition using TensorGraph with PyTorch model.

![Tensor MNIST](parallel/tensor_mnist/parallel_tensor_mnist.png)

## Parallel Fluid 2D

Simple 2D fluid simulation based on Fast Fourier Transformation.

![Fluid 2D](parallel/fluid_2d/parallel_fluid_2d.jpg)

## Parallel Radix Sort

Multiple independent sorting algorithms can be dispatched in parallel. There is no overhead in comparision with the single array sort. Additionally, dispatch parameters can be fetched from the indirect buffer.

![Radix Sort](parallel/radix_sort/parallel_radix_sort.png)

## Parallel Spatial Grid

Simple physics simulation with collisions based on the SpatialGrid class. It is the fastest way to collide objects of the same size.

![Spatial Grid](parallel/spatial_grid/parallel_spatial_grid.png)

## Parallel Spatial Tree

Simple physics simulation with collisions based on the SpatialTree class. The SpatialTree allows collision and intersection tests with any primitive inside BVH.

![Spatial Tree](parallel/spatial_tree/parallel_spatial_tree.png)

---

## Interface Canvas

Different CanvasElement classes, including texture filtration, gradients, contour outlines, and SVG rendering.

![Canvas](interface/canvas/interface_canvas.png)

## Interface Controls

Different User Interface Control classes in resolution-independent configuration.

![Controls](interface/controls/interface_controls.png)

## Interface Layer

A transparent multilayer Controls with variable background blur.

![Layer](interface/layer/interface_layer.png)

## SVG Image

Simple SVG image loading and rendering. CanvasShape class accepts SVG path string as input data.

![SVG](interface/svg/interface_svg.png)

## Custom Control

Custom Controls can be created by simple ControlBase class inheritance. The Control behavior can be completely overridden.

![Control](interface/control/interface_control.png)

---

## Platform Precision

Performance and precision difference between 16-bit, 32-bit, and 64-bit floating formats.

![Precision](platform/precision/platform_precision.jpg)

## Platform Bindless

TextureTable is an interface for an unlimited number of bindless textures that can be accessed from any shader by uniform or non-uniform index. Vulkan or Direct3D12 API is required.

![Table](platform/table/platform_table.png)

## Platform Texel

Texel buffer provides cached buffer access that is compatible with OpenGLES platform. Moreover, it can work faster than the Storage buffer in some scenarios.

![Texel](platform/texel/platform_texel.png)

## Platform Texture

Dynamic 3D texture created with SIMD CPU instructions.

![Texture](platform/texture/platform_texture.png)

## Platform Command

Command class for basic rendering. Depth Cube texture for omnidirectional shadow map.

![Command](platform/command/platform_command.png)

## Platform Tracing

Hardware accelerated raytracing shadows. An API with Ray Query support is required.

![Tracing](platform/tracing/platform_tracing.png)

## Platform Compute

Compute class for simple compute shader texture generation.

![Compute](platform/compute/platform_compute.png)

## Platform Fence

Multi-GPU N-body simulation with Fence synchronization. A shared buffer is used for data exchange between GPUs.

![Fence](platform/fence/platform_fence.png)

## Platform Dynamic

A single-thread dynamic geometry rendering can provide more than 100M triangles per second rate.

![Dynamic](platform/dynamic/platform_dynamic.jpg)

## Platform Clip Planes

Vertex Shader clip distance output is useful for arbitrary geometry culling.

![Clipping](platform/clipping/platform_clipping.png)

## Platform Stencil Buffer

Constructive Solid Geometry with the Stencil Buffer. This algorithm is compatible with procedural geometry.

![Stencil](platform/stencil/platform_stencil.png)

## Platform Fusion

Fusion API allows replicating all commands and resources across multiple physical or logical devices. It dramatically simplifies the development of apps for multi-GPU or multi-channel rendering.

![Fusion](platform/fusion/platform_fusion.png)

## Platform Layers

Multilayer rendering with Layer specified by the Vertex Shader / Geometry Shader / Geometry Passthrough Shader.

![Layers](platform/layers/platform_layers.jpg)

## Barycentric Coordinates

Build-in barycentric coordinate input is available in Vulkan, Direct3D12, and Metal APIs.

![Barycentric](platform/barycentric/platform_barycentric.png)

## Platform Texture Samples

Writing and reading individual multisample texture samples with active Pipeline sample write mask.

![Samples](platform/samples/platform_samples.png)

## Conservative Rasterization

Hardware conservative rasterization.

![Conservative](platform/conservative/platform_conservative.png)

## Cooperative Matrix (Tensor Cores)

Cooperative matrix example.

![Cooperative](platform/cooperative/platform_cooperative.png)

## Matrix Multiplication

Matrix multiplication example.

![Matrix](platform/matrix/platform_matrix.png)

## Shader Printf

A simple preprocessor-based printf() functionality for compute shaders that work with all APIs.

![Printf](platform/printf/platform_printf.png)
