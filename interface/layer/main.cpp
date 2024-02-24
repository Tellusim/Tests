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
#include <core/TellusimPointer.h>
#include <interface/TellusimControls.h>
#include <platform/TellusimDevice.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	DECLARE_WINDOW
	if(!window) return 1;
	
	// create window
	String title = String::format("%s Tellusim::Layer", window.getPlatformName());
	if(!window.create(title) || !window.setHidden(false)) return 1;
	
	// create device
	Device device(window);
	if(!device) return 1;
	
	// create canvas
	Canvas canvas;
	
	// create root control
	ControlRoot root(canvas, true);
	root.setFontSize(16);
	
	// create rect
	ControlRect rect(&root);
	rect.setMode(CanvasElement::ModeTexture);
	rect.setFullscreen(true);
	
	// create window
	ControlDialog dialog(&root, 1, 0.0f, 8.0f);
	dialog.setAlign(Control::AlignCenter);
	dialog.setFilter(Sampler::FilterTrilinear);
	dialog.setMode(CanvasElement::ModeTextureCubic3x3);
	dialog.setColor(0.95f, 0.95f, 0.95f, 1.0f);
	dialog.setMargin(64.0f, 96.0f);
	dialog.setTextureProj(true);
	dialog.setRadius(64.0f);
	dialog.setResizeArea(32.0f);
	dialog.setStrokeStyle(StrokeStyle(4.0f, Color(0.0f, 0.0f, 0.0f, 0.75f)));
	dialog.setUpdatedCallback([](ControlDialog window) { TS_LOGF(Message, "Window updated: %.0f %.0f %.0fx%.0f\n", window.getPosition().x, window.getPosition().y, window.getWidth(), window.getHeight()); });
	
	// create text
	ControlText text(&dialog, "Hello Blur!!!");
	text.setAlign(Control::AlignCenter | Control::AlignExpand);
	text.setMargin(0.0f, 0.0f, 32.0f, 0.0f);
	text.setFontName("sansb.ttf");
	text.getFontStyle().offset = Vector3f(4.0f, -4.0f, 0.0f);
	text.setFontSize(48);
	
	// create sliders
	ControlSlider sliders[] = {
		ControlSlider(&dialog, "Red",	2, 0.3f),
		ControlSlider(&dialog, "Green",	2, 0.9f),
		ControlSlider(&dialog, "Blue",	2, 0.9f),
		ControlSlider(&dialog, "Mipmap", 1, 5.0f, 0.0f, 6.0f),
	};
	sliders[0].setFontColor(Color(1.0f, 0.2f, 0.2f, 1.0f));
	sliders[1].setFontColor(Color(0.2f, 1.0f, 0.2f, 1.0f));
	sliders[2].setFontColor(Color(0.2f, 0.2f, 1.0f, 1.0f));
	for(uint32_t i = 0; i < TS_COUNTOF(sliders); i++) {
		sliders[i].setAlign(Control::AlignExpandX);
	}
	
	// create check
	ControlCheck check(&dialog, "Animation", true);
	check.setMargin(0.0f, 0.0f, 0.0f, 16.0f);
	check.setAlign(Control::AlignCenterX);
	
	// create back canvas
	Canvas back_canvas;
	
	// create lines
	CanvasMesh line(back_canvas);
	line.setMode(CanvasElement::ModeSolid);
	line.setPrimitive(Pipeline::PrimitiveLine);
	for(uint32_t j = 0; j < 128; j++) {
		float32_t radius = 1.3f - j / 128.0f;
		for(uint32_t i = 0; i <= 7; i++) {
			float32_t angle = Pi05 + 3.0f * Pi2 * i / 7.0f;
			uint32_t color = (j >= 64) ? ((127 - j) << 2) : (j << 2);
			uint32_t index = line.addVertex(Tellusim::cos(angle) * radius, Tellusim::sin(angle) * radius, 0xff000000u | (color << 16) | (color << 8) | (color << 0));
			if(i) line.addIndices(index - 1, index);
		}
	}
	
	// render texture
	Texture color_texture;
	
	// create targets
	Target render_target = device.createTarget();
	Target window_target = device.createTarget(window);
	
	// parameters
	float32_t animation_time = 0.0f;
	float32_t old_animation_time = 0.0f;
	
	// main loop
	DECLARE_GLOBAL
	window.run([&]() -> bool {
		DECLARE_COMMON
		
		Window::update();
		
		if(!window.render()) return false;
		
		// animation time
		if(check.isChecked()) animation_time += time - old_animation_time;
		old_animation_time = time;
		
		// update element
		float32_t center_x = window.getWidth() / 2.0f;
		float32_t center_y = window.getHeight() / 2.0f;
		Matrix4x4f offset = Matrix4x4f::translate(0.0f, center_y, 0.0f);
		Matrix4x4f center = Matrix4x4f::translate(center_x, center_y, 0.0f);
		line.setTransform(center * offset * Matrix4x4f::rotateZ(Tellusim::sin(animation_time) * 24.0f) * inverse(offset) * Matrix4x4f::scale(256.0f, 256.0f, 1.0f));
		line.setColor(sliders[0].getValuef32(), sliders[1].getValuef32(), sliders[2].getValuef32(), 1.0f);
		
		// update controls
		if(!pause) root.setMouse(window.getMouseX(), window.getMouseY(), translate_button(window.getMouseButtons()));
		root.setViewport(window.getWidth(), window.getHeight());
		root.update();
		
		// create render texture
		if(!color_texture || color_texture.getWidth() != window.getWidth() || color_texture.getHeight() != window.getHeight()) {
			color_texture = device.createTexture2D(FormatRGBAu8n, window.getWidth(), window.getHeight(), Texture::FlagTarget | Texture::FlagMipmaps);
			render_target.setColorTexture(color_texture, Target::BeginClear | Target::EndStore);
			float32_t dx = 0.5f / window.getWidth();
			float32_t dy = 0.5f / window.getHeight();
			rect.setTexCoord(dx, 1.0f + dx, dy, 1.0f + dy);
			rect.setTexture(color_texture);
			dialog.setTexture(color_texture);
		}
		
		// create canvas
		back_canvas.create(device, render_target);
		
		// render target
		render_target.setClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		render_target.begin();
		{
			// create command list
			Command command = device.createCommand(render_target);
			
			// draw canvas
			back_canvas.setViewport(window.getWidth(), window.getHeight());
			back_canvas.draw(command, render_target);
		}
		render_target.end();
		
		// create mipmaps
		dialog.setMipmap(sliders[3].getValuef32());
		device.createMipmaps(color_texture);
		device.flushTexture(color_texture);
		
		// create canvas
		canvas.create(device, window_target);
		
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
