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

#ifndef __TELLUSIM_Q_GL_WIDGET_H__
#define __TELLUSIM_Q_GL_WIDGET_H__

#include <QtCore/QTimer>
#include <QtWidgets/QOpenGLWidget>

#include <platform/TellusimDevice.h>
#include <platform/TellusimPipeline.h>
#include <platform/TellusimContext.h>
#include <platform/TellusimSurface.h>

/*
 */
namespace Tellusim {
	
	/*
	 */
	class QGLWidget : public QOpenGLWidget {
			
			Q_OBJECT
			
		public:
			
			QGLWidget(QWidget *parent = nullptr);
			~QGLWidget();
			
			virtual QSize sizeHint() const;
			
		protected:
			
			/// rendering loop
			virtual void initializeGL();
			virtual void resizeGL(int32_t width, int32_t height);
			virtual void paintGL();
			
		private:
			
			bool initialized = false;
			
			GLContext gl_context;
			
			GLSurface surface;
			
			Device device;
			Pipeline pipeline;
			Buffer vertex_buffer;
			Buffer index_buffer;
			
			QTimer timer;
	};
}

#endif /* __TELLUSIM_Q_GL_WIDGET_H__ */
