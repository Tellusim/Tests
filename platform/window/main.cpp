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
#include <math/TellusimColor.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	// create window
	Window window = Window(PlatformUnknown);
	if(!window || !window.create("Tellusim::Window")) return 1;
	if(!window.setHidden(false)) return 1;
	
	// window callbacks
	bool is_running = true;
	window.setMousePressedCallback([&](Window::Button button) { TS_LOGF(Message, "Mouse Pressed: %u\n", button); });
	window.setMouseReleasedCallback([&](Window::Button button) { TS_LOGF(Message, "Mouse Released: %u\n", button); });
	window.setMouseRotatedCallback([&](Window::Axis axis, float32_t value) { TS_LOGF(Message, "Mouse Rotated: %u %f\n", axis, value); });
	window.setKeyboardPressedCallback([&](uint32_t key, uint32_t code) { TS_LOGF(Message, "Keyboard Pressed: %u %u\n", key, code); });
	window.setKeyboardReleasedCallback([&](uint32_t key) { TS_LOGF(Message, "Keyboard Released: %u\n", key); });
	window.setCloseClickedCallback([&]() { is_running = false; });
	
	// main loop
	DECLARE_GLOBAL
	while(is_running) {
		DECLARE_COMMON
		
		// wait for events
		Window::update(true);
		
		// close window
		if(window.getKeyboardKey('q')) break;
		if(window.getKeyboardKey(Window::KeyEsc)) break;
		
		// clear window
		float32_t r = saturate((float32_t)window.getMouseX() / (float32_t)window.getWidth());
		float32_t g = saturate((float32_t)window.getMouseY() / (float32_t)window.getHeight());
		window.clear(Color(r, g, 0.0f, 1.0f));
	}
	
	return 0;
}
