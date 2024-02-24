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
#include <format/TellusimJson.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	if(1) {
		
		Json root("root");
		
		root.setData("null", nullptr);
		root.setData("bool_true", true);
		root.setData("bool_false", false);
		root.setData("number_int32", -113);
		root.setData("number_uint32", 113);
		root.setData("number_float32", 113.133f);
		root.setData("string", "this is a string");
		
		Json object(&root, "object");
		object.setData("first", "first string");
		object.setData("second", "second string");
		
		Json null_array(&root, "null_array");
		null_array.setData(nullptr, nullptr);
		null_array.setData(nullptr, nullptr);
		null_array.setData(nullptr, nullptr);
		
		Json bool_array(&root, "bool_array");
		bool_array.setData(nullptr, true);
		bool_array.setData(nullptr, false);
		bool_array.setData(nullptr, true);
		
		Json number_array(&root, "number_array");
		number_array.setData(nullptr, 0.0f);
		number_array.setData(nullptr, 1.0f);
		number_array.setData(nullptr, 2.0f);
		number_array.setData(nullptr, 3.0f);
		
		Json string_array(&root, "string_array");
		string_array.setData(nullptr, "first");
		string_array.setData(nullptr, "second");
		string_array.setData(nullptr, "third");
		
		Json object_array(&root, "object_array");
		
		Json(&object_array, nullptr).setData("first", "second");
		Json(&object_array, nullptr).setData("third", "fourth");
		
		Json mixed_array(&root, "mixed_array");
		mixed_array.setData(nullptr, nullptr);
		mixed_array.setData(nullptr, true);
		mixed_array.setData(nullptr, 1.0f);
		mixed_array.setData(nullptr, "first");
		Json(&mixed_array, nullptr, Json::TypeObject);
		Json(&mixed_array, nullptr, Json::TypeArray);
		
		Json copy = root.clonePtr();
		TS_LOGPTR(Message, "root: ", root);
		TS_LOGPTR(Message, "copy: ", copy);
		if(!copy.save("test_save_a.json")) return 1;
	}
	
	if(1) {
		
		Json json;
		if(!json.load("test_load.json")) return 1;
		
		Json copy = json.clonePtr();
		TS_LOGPTR(Message, "json: ", json);
		TS_LOGPTR(Message, "copy: ", copy);
		if(!copy.save("test_save_b.json")) return 1;
	}
	
	return 0;
}
