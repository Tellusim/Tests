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
#include <platform/TellusimCompute.h>
#include <platform/TellusimContext.h>
#include <graphics/TellusimEncoderBC67.h>

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
	
	// check compute shader support
	if(!device.hasShader(Shader::TypeCompute)) {
		TS_LOG(Error, "compute shader is not supported\n");
		return 0;
	}
	
	// create encoder
	EncoderBC67 encoder;
	Format encoder_format = FormatBC7RGBAu8n;
	EncoderBC67::Mode encoder_mode = EncoderBC67::ModeBC7RGBAu8n;
	if(!encoder.create(device, encoder_mode)) return 1;
	
	// load texture
	Texture src_texture = device.loadTexture("texture.png");
	
	// create intermediate image
	uint32_t width = src_texture.getWidth();
	uint32_t height = src_texture.getHeight();
	uint32_t block_size = getFormatBlockWidth(encoder_format);
	Image dest_image = Image(Image::Type2D, FormatRGBAu32, Size(udiv(width, block_size), udiv(height, block_size)));
	
	// create intermediate texture
	Texture dest_texture = device.createTexture(dest_image, Texture::FlagSurface | Texture::FlagSource);
	if(!dest_texture) return 1;
	
	// dispatch encoder
	{
		Compute compute = device.createCompute();
		encoder.dispatch(compute, encoder_mode, dest_texture, src_texture);
	}
	
	// flush context
	context.flush();
	
	// get intermediate image data
	if(!device.getTexture(dest_texture, dest_image)) return 1;
	
	// copy image data
	Image image = Image(Image::Type2D, encoder_format, Size(width, height));
	memcpy(image.getData(), dest_image.getData(), min(image.getDataSize(), dest_image.getDataSize()));
	
	// save encoded image
	image.save("texture.dds");
	
	// check errors
	device.check();
	
	// finish context
	context.finish();
	
	return 0;
}
