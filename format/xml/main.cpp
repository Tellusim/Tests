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
#include <format/TellusimXml.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	if(1) {
		
		Xml root("root", "version=\"2\"");
		root.setAttribute("attribute", "root");
		Xml(&root, "!-- comment line--");
		
		Xml first(&root, "first", "one=\"<one>\" two=\"&quot;two&quot;\"");
		first.setData("<first data>");
		
		Xml second(&root, "second");
		second.setAttribute("one", "one");
		second.setAttribute("two", "two");
		Xml(&second, "![CDATA[second data]]");
		
		Xml third(&root, "third", "one=\"one\" two=\"two\"");
		third.setData("\"third data\"");
		
		Xml fourth(&root, "fourth", "one=\"one\" two=\"two\"");
		fourth.setData("fifth", "'fifth data'");
		fourth.setData("sixth", "@@");
		fourth.setData("seventh", "/path/to/file");
		
		Xml copy = root.clonePtr();
		TS_LOGPTR(Message, "root: ", root);
		TS_LOGPTR(Message, "copy: ", copy);
		if(!copy.save("test_save_a.xml")) return 1;
		
		for(const Xml &xml : root.getChildren()) {
			TS_LOGF(Message, "%s\n", xml.getName().get());
		}
	}
	
	if(1) {
		
		Xml xml;
		if(!xml.load("test_load.xml")) return 1;
		
		Xml copy = xml.clonePtr();
		TS_LOGPTR(Message, " xml: ", xml);
		TS_LOGPTR(Message, "copy: ", copy);
		if(!copy.save("test_save_b.xml")) return 1;
	}
	
	return 0;
}
