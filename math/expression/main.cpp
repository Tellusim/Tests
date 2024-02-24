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

#include <core/TellusimLog.h>
#include <math/TellusimString.h>
#include <math/TellusimExpression.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	try {
		const char *expression = "31 * 3 + 3";
		TS_LOGF(Message, "%s = %d\n", expression, (int32_t)Expression::getScalari64(expression));
	}
	catch(...) {
		TS_LOG(Error, "can't evaluate expression\n");
	}
	
	try {
		const char *expression = "Vector3f(31.0f) * Vector3f(1.0f, 2.0f, 3.0f) * 3.0f + 3.0f";
		TS_LOGF(Message, "%s = %s\n", expression, toString(Expression::getVector3f(expression)).get());
	}
	catch(...) {
		TS_LOG(Error, "can't evaluate expression\n");
	}
	
	try {
		const char *expression = "perspectiveR(60.0f, 1.0f, 0.01f) * rotateX(90.0f)";
		TS_LOGF(Message, "%s = %s\n", expression, toString(Expression::getMatrix4x4f(expression)).get());
	}
	catch(...) {
		TS_LOG(Error, "can't evaluate expression\n");
	}
	
	return 0;
}
