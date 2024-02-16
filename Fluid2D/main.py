#!/usr/bin/env python3

# MIT License
# 
# Copyright (C) 2018-2024, Tellusim Technologies Inc. https://tellusim.com/
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys
import math

from tellusimd import *

#
# main
#
def main(argv):
	
	# create app
	app = App(sys.argv)
	if not app.create(): return 1
	
	# create window
	window = Window(app.getPlatform(), app.getDevice())
	if not window: return 1
	
	window.setSize(app.getWidth(), app.getHeight())
	window.setCloseClickedCallback(lambda: window.stop())
	window.setKeyboardPressedCallback(lambda key, code: window.stop() if key == Window.KeyEsc else None)
	
	title = window.getPlatformName() + ' Tellusim::FourierTransform Python'
	if not window.create(title) or not window.setHidden(False): return 1
	
	# create device
	device = Device(window)
	if not device: return 1
	
	# check compute tracing support
	if not device.hasShader(Shader.TypeCompute):
		Log.print(Log.Error, 'compute shader is not supported\n')
		return 0
	
	# fluid parameters
	size = 2048
	viscosity = 0.04
	ifps = 1.0 / 8000.0
	
	# shader cache
	Shader.setCache('main.cache')
	
	# create advection kernel
	advection_kernel = device.createKernel().setSamplers(1).setTextures(1).setSurfaces(1).setUniforms(1)
	if not advection_kernel.loadShaderGLSL('main.shader', 'COMPUTE_ADVECTION_SHADER=1'): return 1
	if not advection_kernel.create(): return 1
	
	# diffuse kernel
	diffuse_kernel = device.createKernel().setTextures(1).setSurfaces(1).setUniforms(1)
	if not diffuse_kernel.loadShaderGLSL('main.shader', 'COMPUTE_DIFFUSE_SHADER=1'): return 1
	if not diffuse_kernel.create(): return 1
	
	# update kernel
	update_kernel = device.createKernel().setSamplers(1).setTextures(2).setSurfaces(1).setUniforms(1)
	if not update_kernel.loadShaderGLSL('main.shader', 'COMPUTE_UPDATE_SHADER=1'): return 1
	if not update_kernel.create(): return 1
	
	# create pipeline
	pipeline = device.createPipeline()
	pipeline.setSamplerMask(0, Shader.MaskFragment)
	pipeline.setTextureMask(0, Shader.MaskFragment)
	pipeline.setColorFormat(window.getColorFormat())
	pipeline.setDepthFormat(window.getDepthFormat())
	if not pipeline.loadShaderGLSL(Shader.TypeVertex, 'main.shader', 'VERTEX_SHADER=1'): return 1
	if not pipeline.loadShaderGLSL(Shader.TypeFragment, 'main.shader', 'FRAGMENT_SHADER=1'): return 1
	if not pipeline.create(): return 1
	
	# create transform
	transform = FourierTransform()
	if not transform.create(device, mode = FourierTransform.ModeRGf32i, width = size // 2, height = size): return 1
	
	# create textures
	fft_textures = [None] * 2
	velocity_textures = [None] * 2
	color_textures = [None] * 2
	for i in range(2):
		fft_textures[i] = device.createTexture2D(FormatRGBAf32, size // 2 + 1, size, flags = Texture.FlagSurface)
		velocity_textures[i] = device.createTexture2D(FormatRGf32, size, flags = Texture.FlagTarget | Texture.FlagSurface)
		color_textures[i] = device.createTexture2D(FormatRGBAu8n, size, flags = Texture.FlagTarget | Texture.FlagSurface)
		if not fft_textures[i] or not velocity_textures[i] or not color_textures[i]: return 1
	if not device.clearTexture(velocity_textures[0], 0): return 1
	
	# initialize texture
	image = Image()
	if not image.load('image.jpg'): return 1
	image = image.toFormat(FormatRGBAu8n)
	image = image.getResized(color_textures[0].getSize())
	if not device.setTexture(color_textures[0], image): return 1
	
	# create sampler
	sampler = device.createSampler(Sampler.FilterLinear, Sampler.WrapModeClamp)
	if not sampler: return 1
	
	# create target
	target = device.createTarget(window)
	
	frame_counter = 0
	
	# main loop
	def main_loop():
		nonlocal frame_counter
		
		Window.update()
		
		if not window.render(): return False
		
		if True:
			
			# create command list
			compute = device.createCompute()
			
			# advection parameters
			advection_parameters = bytearray()
			if frame_counter < 16:
				advection_parameters += Vector2f(0.5, 0.1)
				advection_parameters += Vector2f(0.1, 32.0)
				advection_parameters += Scalarf(16.0 / size)
				frame_counter += 1
			else:
				iwidth = 1.0 / window.getWidth()
				iheight = 1.0 / window.getHeight()
				advection_parameters += Vector2f(window.getMouseX() * iwidth, window.getMouseY() * iheight)
				advection_parameters += Vector2f(window.getMouseDX() * iwidth, window.getMouseDY() * iheight) / ifps
				advection_parameters += Scalarf(16.0 / size if window.getMouseButtons() else 0.0)
			advection_parameters += Scalarf(ifps)
			
			# diffuse parameters
			diffuse_parameters = bytearray()
			diffuse_parameters += Vector2f(viscosity, ifps)
			
			# update parameters
			update_parameters = bytearray()
			update_parameters += Scalarf(ifps * 2.0)
			
			# dispatch advection kernel
			compute.setKernel(advection_kernel)
			compute.setUniform(0, advection_parameters)
			compute.setSampler(0, sampler)
			compute.setTexture(0, velocity_textures[0])
			compute.setSurfaceTexture(0, velocity_textures[1])
			compute.dispatch(velocity_textures[1])
			compute.barrier(velocity_textures[1])
			
			# dispatch forward transform
			transform.dispatch(compute, FourierTransform.ModeRGf32i, FourierTransform.ForwardRtoC, fft_textures[0], velocity_textures[1])
			
			# dispatch diffuse kernel
			compute.setKernel(diffuse_kernel)
			compute.setUniform(0, diffuse_parameters)
			compute.setTexture(0, fft_textures[0])
			compute.setSurfaceTexture(0, fft_textures[1])
			compute.dispatch(fft_textures[1])
			compute.barrier(fft_textures[1])
			
			# dispatch inverse transform
			transform.dispatch(compute, FourierTransform.ModeRGf32i, FourierTransform.BackwardCtoR, velocity_textures[0], fft_textures[1])
			
			# dispatch update kernel
			compute.setKernel(update_kernel)
			compute.setUniform(0, update_parameters)
			compute.setSampler(0, sampler)
			compute.setTexture(0, velocity_textures[0])
			compute.setTexture(1, color_textures[0])
			compute.setSurfaceTexture(0, color_textures[1])
			compute.dispatch(color_textures[1])
			compute.barrier(color_textures[1])
			
			# swap textures
			color_textures[0], color_textures[1] = color_textures[1], color_textures[0]
			
			compute = None
		
		# flush texture
		device.flushTexture(color_textures[0])
		
		# window target
		if target.begin():
			
			# create command list
			command = device.createCommand(target)
			
			# draw color texture
			command.setPipeline(pipeline)
			command.setSampler(0, sampler)
			command.setTexture(0, color_textures[0])
			command.drawArrays(3)
			
			command = None
			
			target.end()
		
		if not window.present(): return False
		
		device.check()
		
		return True
	
	window.run(main_loop)
	
	# finish context
	window.finish()
	
	return 0

#
# entry point
#
if __name__ == '__main__':
	try:
		exit(main(sys.argv))
	except Exception as error:
		print('\n' + str(error))
		exit(1)
