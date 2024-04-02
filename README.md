# [Tellusim Core SDK Tests](https://tellusim.com/core-sdk/)

C++, C#, Rust, Python, Swift, Java, Kotlin

Direct3D12, Direct3D11, Metal, Vulkan, OpenGL, OpenGLES, WebGL, WebGPU

Windows, Linux, macOS, Android, iOS, Web

https://tellusim.com/

---

## Clustered Lights

Forward shading with 16384 dynamic lights. This algorithm is compatible with deferred shading and transparent objects.

https://github.com/Tellusim/Tests/tree/main/graphics/lights/

![Lights](graphics/lights/graphics_lights.jpg)

## Meshlet Render

A massive meshlets rendering example with Mesh Shader for hardware and Compute Shader for software rasterization.

https://github.com/Tellusim/Tests/tree/main/graphics/meshlet/

![Meshlet](graphics/meshlet/graphics_meshlet.jpg)

## Mesh RayTracing

Traversal class for the simple raytracing pipeline access. Vulkan or Direct3D12 API is required.

https://github.com/Tellusim/Tests/tree/main/graphics/traversal/

![Traversal](graphics/traversal/graphics_traversal.jpg)

## Mesh Tracing

Ray Query raytracing of animated scene. Vulkan, Direct3D12, or Metal API is required.

https://github.com/Tellusim/Tests/tree/main/graphics/tracing/

![Tracing](graphics/tracing/graphics_tracing.jpg)

## Separable Filter

The SeparableFilter interface helps to create different separable filters like Gaussian, Sobel, Box, and custom weights.

https://github.com/Tellusim/Tests/tree/main/graphics/separable_filter/

![Separable Filter](graphics/separable_filter/graphics_separable_filter.jpg)

## Geometry Tessellation

Quadrilateral tessellation with Control + Evaluate or Mesh Shaders.

https://github.com/Tellusim/Tests/tree/main/graphics/tessellation/

![Tessellation](graphics/tessellation/graphics_tessellation.jpg)

## Ordered Independent Transparency

Ordered Independent Transparency with atomic buffer operations from the fragment shader.

https://github.com/Tellusim/Tests/tree/main/graphics/transparency/

![Transparency](graphics/transparency/graphics_transparency.jpg)

## Hardware Ray Tracing Shadows

Hardware raytracing shadows with simple deferred shading. An API with Ray Query support is required.

https://github.com/Tellusim/Tests/tree/main/graphics/shadow_tracing/

![Shadow Tracing](graphics/shadow_tracing/graphics_shadow_tracing.jpg)

## Software Ray Tracing Shadows

Software raytracing shadows with simple deferred shading. Compatible with all APIs.

https://github.com/Tellusim/Tests/tree/main/graphics/shadow_tree/

![Shadow Tree](graphics/shadow_tree/graphics_shadow_tree.jpg)

## Percentage Closer Filtering Shadow Map

PCF Shadow Maps provides shadow with variable penumbra size based on the distance between the shadow caster and receiver.

https://github.com/Tellusim/Tests/tree/main/graphics/shadow_pcf/

![Shadow PCF](graphics/shadow_pcf/graphics_shadow_pcf.jpg)

## Exponential Shadow Maps

Exponential Shadow Maps provides noise-free shadows with a fixed performance cost and constant penumbra size.

https://github.com/Tellusim/Tests/tree/main/graphics/shadow_esm/

![Shadow ESM](graphics/shadow_esm/graphics_shadow_esm.jpg)

## Parallax 2D

Parallax occlusion mapping with self-shadowing for the mesh geometry.

https://github.com/Tellusim/Tests/tree/main/graphics/parallax_2d/

![Parallax 2D](graphics/parallax_2d/graphics_parallax_2d.jpg)

## Parallax Cube

Parallax occlusion mapping with self-shadowing for the analytical sphere.

https://github.com/Tellusim/Tests/tree/main/graphics/parallax_cube/

![Parallax Cube](graphics/parallax_cube/graphics_parallax_cube.jpg)

## Mesh Tangent Basis

Mesh tangent basis renormalization at the Fragment Shader. Mesh class can re-create normal and tangent vectors.

https://github.com/Tellusim/Tests/tree/main/graphics/tangent/

![Tangent](graphics/tangent/graphics_tangent.jpg)

## Mesh Model

MeshModel class creates a rendering model representation for the input Mesh or MeshGeometry with a specified Pipeline layout.

https://github.com/Tellusim/Tests/tree/main/graphics/model/

![Model](graphics/model/graphics_model.jpg)

## Mesh Skinned

This is a basic skinned mesh animation example. Mesh classes give comprehensive access to all mesh data, including Nodes, Animations, Materials, Cameras, and Lights.

https://github.com/Tellusim/Tests/tree/main/graphics/skinned/

![Mesh Skinned](graphics/skinned/graphics_skinned.jpg)

## Line Rendering

Simple antialiased line rendering. The Vertex shader creates a screen-aligned billboard, and the fragment shader calculates the distance to the line.

https://github.com/Tellusim/Tests/tree/main/graphics/line/

![Line](graphics/line/graphics_line.jpg)

## Texture Compression

Real-time BC1-BC7 texture encoder using compute shader.

https://github.com/Tellusim/Tests/tree/main/graphics/encoder/

![Texture](graphics/encoder/texture.png)

---

## Mesh Reduce

MeshReduce is a simple way to simplify input mesh geometry. Simplification preserves all Mesh Attributes, and it is compatible with Skinning Animation.

https://github.com/Tellusim/Tests/tree/main/geometry/reduce/

![Mesh Reduce](geometry/reduce/geometry_reduce.jpg)

## Mesh Refine

MeshRefine refines geometry mesh using Catmull-Clark (for quadrilaterals) or Loop (for triangles) subdivision algorithms. The Crease Attribute allows additional control over the subdivision process.

https://github.com/Tellusim/Tests/tree/main/geometry/refine/

![Mesh Refine](geometry/refine/geometry_refine.jpg)

---

## Parallel TensorGraph from PyTorch

Simple convolutional autoencoder trained with PyTorch and imported into TensorGraph.

https://github.com/Tellusim/Tests/tree/main/parallel/tensor_torch/

![Tensor Torch](parallel/tensor_torch/parallel_tensor_torch.jpg)

## Parallel TensorGraph MNIST Digits Recognition

MNIST Digits Recognition using TensorGraph with PyTorch model.

https://github.com/Tellusim/Tests/tree/main/parallel/tensor_mnist/

![Tensor MNIST](parallel/tensor_mnist/parallel_tensor_mnist.jpg)

## Parallel Fluid 2D

Simple 2D fluid simulation based on Fast Fourier Transformation.

https://github.com/Tellusim/Tests/tree/main/parallel/fluid_2d/

![Fluid 2D](parallel/fluid_2d/parallel_fluid_2d.jpg)

## Parallel Radix Sort

Multiple independent sorting algorithms can be dispatched in parallel. There is no overhead in comparision with the single array sort. Additionally, dispatch parameters can be fetched from the indirect buffer.

https://github.com/Tellusim/Tests/tree/main/parallel/radix_sort/

![Radix Sort](parallel/radix_sort/parallel_radix_sort.jpg)

## Parallel Spatial Grid

Simple physics simulation with collisions based on the SpatialGrid class. It is the fastest way to collide objects of the same size.

https://github.com/Tellusim/Tests/tree/main/parallel/spatial_grid/

![Spatial Grid](parallel/spatial_grid/parallel_spatial_grid.jpg)

## Parallel Spatial Tree

Simple physics simulation with collisions based on the SpatialTree class. The SpatialTree allows collision and intersection tests with any primitive inside BVH.

https://github.com/Tellusim/Tests/tree/main/parallel/spatial_tree/

![Spatial Tree](parallel/spatial_tree/parallel_spatial_tree.jpg)

---

## Interface Canvas

Different CanvasElement classes, including texture filtration, gradients, contour outlines, and SVG rendering.

https://github.com/Tellusim/Tests/tree/main/interface/canvas/

![Canvas](interface/canvas/interface_canvas.jpg)

## Interface Controls

Different User Interface Control classes in resolution-independent configuration.

https://github.com/Tellusim/Tests/tree/main/interface/controls/

![Controls](interface/controls/interface_controls.jpg)

## Interface Layer

A transparent multilayer Controls with variable background blur.

https://github.com/Tellusim/Tests/tree/main/interface/layer/

![Layer](interface/layer/interface_layer.jpg)

## SVG Image

Simple SVG image loading and rendering. CanvasShape class accepts SVG path string as input data.

https://github.com/Tellusim/Tests/tree/main/interface/svg/

![SVG](interface/svg/interface_svg.jpg)

## Custom Control

Custom Controls can be created by simple ControlBase class inheritance. The Control behavior can be completely overridden.

https://github.com/Tellusim/Tests/tree/main/interface/control/

![Control](interface/control/interface_control.jpg)

---

## Platform Precision

Performance and precision difference between 16-bit, 32-bit, and 64-bit floating formats.

https://github.com/Tellusim/Tests/tree/main/platform/precision/

![Precision](platform/precision/platform_precision.jpg)

## Platform Bindless

TextureTable is an interface for an unlimited number of bindless textures that can be accessed from any shader by uniform or non-uniform index. Vulkan or Direct3D12 API is required.

https://github.com/Tellusim/Tests/tree/main/platform/table/

![Table](platform/table/platform_table.jpg)

## Platform Texel

Texel buffer provides cached buffer access that is compatible with OpenGLES platform. Moreover, it can work faster than the Storage buffer in some scenarios.

https://github.com/Tellusim/Tests/tree/main/platform/texel/

![Texel](platform/texel/platform_texel.jpg)

## Platform Texture

Dynamic 3D texture created with SIMD CPU instructions.

https://github.com/Tellusim/Tests/tree/main/platform/texture/

![Texture](platform/texture/platform_texture.jpg)

## Platform Command

Command class for basic rendering. Depth Cube texture for omnidirectional shadow map.

https://github.com/Tellusim/Tests/tree/main/platform/command/

![Command](platform/command/platform_command.jpg)

## Platform Tracing

Hardware accelerated raytracing shadows. An API with Ray Query support is required.

https://github.com/Tellusim/Tests/tree/main/platform/tracing/

![Tracing](platform/tracing/platform_tracing.jpg)

## Platform Compute

Compute class for simple compute shader texture generation.

https://github.com/Tellusim/Tests/tree/main/platform/compute/

![Compute](platform/compute/platform_compute.jpg)

## Platform Fence

Multi-GPU N-body simulation with Fence synchronization. A shared buffer is used for data exchange between GPUs.

https://github.com/Tellusim/Tests/tree/main/platform/fence/

![Fence](platform/fence/platform_fence.jpg)

## Platform Dynamic

A single-thread dynamic geometry rendering can provide more than 100M triangles per second rate.

https://github.com/Tellusim/Tests/tree/main/platform/dynamic/

![Dynamic](platform/dynamic/platform_dynamic.jpg)

## Platform Clip Planes

Vertex Shader clip distance output is useful for arbitrary geometry culling.

https://github.com/Tellusim/Tests/tree/main/platform/clipping/

![Clipping](platform/clipping/platform_clipping.jpg)

## Platform Stencil Buffer

Constructive Solid Geometry with the Stencil Buffer. This algorithm is compatible with procedural geometry.

https://github.com/Tellusim/Tests/tree/main/platform/stencil/

![Stencil](platform/stencil/platform_stencil.jpg)

## Platform Fusion

Fusion API allows replicating all commands and resources across multiple physical or logical devices. It dramatically simplifies the development of apps for multi-GPU or multi-channel rendering.

https://github.com/Tellusim/Tests/tree/main/platform/fusion/

![Fusion](platform/fusion/platform_fusion.jpg)

## Platform Layers

Multilayer rendering with Layer specified by the Vertex Shader / Geometry Shader / Geometry Passthrough Shader.

https://github.com/Tellusim/Tests/tree/main/platform/layers/

![Layers](platform/layers/platform_layers.jpg)

## Barycentric Coordinates

Build-in barycentric coordinate input is available in Vulkan, Direct3D12, and Metal APIs.

https://github.com/Tellusim/Tests/tree/main/platform/barycentric/

![Barycentric](platform/barycentric/platform_barycentric.jpg)

## Platform Texture Samples

Writing and reading individual multisample texture samples with active Pipeline sample write mask.

https://github.com/Tellusim/Tests/tree/main/platform/samples/

![Samples](platform/samples/platform_samples.jpg)

## Conservative Rasterization

Hardware conservative rasterization.

https://github.com/Tellusim/Tests/tree/main/platform/conservative/

![Conservative](platform/conservative/platform_conservative.jpg)

## Cooperative Matrix (Tensor Cores)

Cooperative matrix example.

https://github.com/Tellusim/Tests/tree/main/platform/cooperative/

![Cooperative](platform/cooperative/platform_cooperative.jpg)

## Matrix Multiplication

Matrix multiplication example.

https://github.com/Tellusim/Tests/tree/main/platform/matrix/

![Matrix](platform/matrix/platform_matrix.jpg)

## Shader Printf

A simple preprocessor-based printf() functionality for compute shaders that work with all APIs.

https://github.com/Tellusim/Tests/tree/main/platform/printf/

![Printf](platform/printf/platform_printf.jpg)
