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

#ifndef __TELLUSIM_Q_VK_WIDGET_H__
#define __TELLUSIM_Q_VK_WIDGET_H__

#include <vulkan/vulkan.h>

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
	class QVKWidget : public QWidget {
			
			Q_OBJECT
			
		public:
			
			QVKWidget(QWidget *parent = nullptr);
			~QVKWidget();
			
			virtual QSize sizeHint() const;
			
		protected:
			
			virtual QPaintEngine *paintEngine() const { return nullptr; }
			
			virtual void paintEvent(QPaintEvent *event);
			
		private:
			
			/// create context
			bool create_context();
			void release_context();
			
			/// create swap chain
			bool create_swap_chain();
			void release_swap_chain();
			
			/// create buffers
			bool create_buffers();
			void release_buffers();
			
			/// image barrier
			VkPipelineStageFlags get_stage_mask(VkAccessFlags access_mask) const;
			void barrier(VkImage image, VkAccessFlags src_mask, VkAccessFlags dest_mask, VkImageLayout old_layout, VkImageLayout new_layout, VkImageAspectFlags aspect_mask);
			
			/// rendering loop
			bool init_vk();
			void release_vk();
			void render_vk();
			
			enum {
				NumFrames = 2,
			};
			
			struct Frame {
				VkImage color_image = VK_NULL_HANDLE;
				VkImageView color_image_view = VK_NULL_HANDLE;
				VkSemaphore acquire_semaphore = VK_NULL_HANDLE;
				VkSemaphore present_semaphore = VK_NULL_HANDLE;
				VkFramebuffer framebuffer = VK_NULL_HANDLE;
			};
			
			bool failed = false;
			bool initialized = false;
			
			uint32_t widget_width = 0;
			uint32_t widget_height = 0;
			
			VkInstance vk_instance = VK_NULL_HANDLE;
			VkPhysicalDevice vk_adapter = VK_NULL_HANDLE;
			VkDevice vk_device = VK_NULL_HANDLE;
			uint32_t vk_family = Maxu32;
			
			VkSurfaceKHR window_surface = VK_NULL_HANDLE;
			VkRenderPass render_pass = VK_NULL_HANDLE;
			VkSwapchainKHR swap_chain = VK_NULL_HANDLE;
			
			Array<Frame> frames;
			uint32_t frame_index = 0;
			VkSurfaceFormatKHR color_format = {};
			
			VkImage depth_image = VK_NULL_HANDLE;
			VkImageView depth_image_view = VK_NULL_HANDLE;
			VkFormat depth_image_format = VK_FORMAT_UNDEFINED;
			VkDeviceMemory depth_image_memory = VK_NULL_HANDLE;
			
			VKContext vk_context;
			
			VKSurface surface;
			
			Device device;
			Pipeline pipeline;
			Buffer vertex_buffer;
			Buffer index_buffer;
			
			QTimer timer;
	};
}

#endif /* __TELLUSIM_Q_VK_WIDGET_H__ */
