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

#ifndef __TELLUSIM_Q_D3D12_WIDGET_H__
#define __TELLUSIM_Q_D3D12_WIDGET_H__

#include <d3d12.h>
#include <dxgi1_4.h>

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>

#include <platform/TellusimDevice.h>
#include <platform/TellusimPipeline.h>
#include <platform/TellusimContext.h>
#include <platform/TellusimSurface.h>

/*
 */
namespace Tellusim {
	
	/*
	 */
	class QD3D12Widget : public QWidget {
			
			Q_OBJECT
			
		public:
			
			QD3D12Widget(QWidget *parent = nullptr);
			~QD3D12Widget();
			
			virtual QSize sizeHint() const;
			
		protected:
			
			virtual QPaintEngine *paintEngine() const { return nullptr; }
			
			virtual void paintEvent(QPaintEvent *event);
			
		private:
			
			enum {
				NumFrames = 3,
			};
			
			/// create context
			bool create_context();
			void release_context();
			
			/// create swap chain
			bool create_swap_chain();
			void release_swap_chain();
			
			/// create buffers
			bool create_buffers();
			void release_buffers();
			
			/// rendering loop
			bool initD3D12();
			void renderD3D12();
			
			bool failed = false;
			bool initialized = false;
			
			uint32_t widget_width = 0;
			uint32_t widget_height = 0;
			
			IDXGIFactory4 *dxgi_factory = nullptr;
			ID3D12Device *d3d12_device = nullptr;
			ID3D12CommandQueue *d3d12_queue = nullptr;
			IDXGISwapChain3 *dxgi_swap_chain = nullptr;
			
			ID3D12Resource *render_targets[NumFrames] = {};
			ID3D12DescriptorHeap *render_target_heap = nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE render_target_views[NumFrames] = {};
			
			ID3D12Resource *depth_stencil = nullptr;
			ID3D12DescriptorHeap *depth_stencil_heap = nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_view = {};
			
			D3D12Context context;
			D3D12Surface surface;
			
			Device device;
			Pipeline pipeline;
			Buffer vertex_buffer;
			Buffer index_buffer;
			
			QTimer timer;
	};
}

#endif /* __TELLUSIM_Q_D3D12_WIDGET_H__ */
