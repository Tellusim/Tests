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
#include <core/TellusimTime.h>
#include <core/TellusimBlob.h>
#include <core/TellusimArray.h>
#include <format/TellusimArchive.h>

/*
 */
using namespace Tellusim;

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	// test archives
	if(1) {
		
		const char *names[] = { "test_archive.tar", "test_archive.tar.gz", "test_archive.zip" };
		
		for(uint32_t i = 0; i < TS_COUNTOF(names); i++) {
			
			TS_LOG(Message, "\n");
			
			Archive archive;
			if(!archive.open(names[i])) return 1;
			TS_LOGF(Message, "%s: %u\n", archive.getName().get(), archive.getNumFiles());
			
			for(uint32_t j = 0; j < archive.getNumFiles(); j++) {
				
				String name = archive.getFileName(j);
				TS_LOGF(Message, "%2u: %8s | %s | %s\n", j, String::fromBytes(archive.getFileSize(j)).get(), Date(archive.getFileMTime(j)).getString().get(), name.get());
				
				Stream stream = archive.openFile(name.get());
				if(!stream) return 1;
				
				if(name.extension() != "txt") {
					for(uint32_t k = 0; k < 1024 * 32; k++) {
						if(stream.readu16() != k) return 2;
					}
				} else {
					if(stream.gets() != name) return 2;
				}
			}
		}
	}
	
	// extern archive stream
	if(1) {
		
		// extern archive format
		class ExternArchiveStream : public ArchiveStream {
				
			private:
				
				ExternArchiveStream() { }
				
			public:
				
				// register archive format
				ExternArchiveStream(const char *name) : ArchiveStream(name) { }
				
				// create instance
				virtual ArchiveStream *instance() const {
					ArchiveStream *instance = new ExternArchiveStream();
					TS_LOGF(Message, "create archive instance %p\n", instance);
					return instance;
				}
				virtual void destructor(ArchiveStream *instance) const {
					TS_LOGF(Message, "delete archive instance %p\n", instance);
					delete instance;
				}
				
				// open archive
				virtual bool open(Stream &stream) {
					
					// extern header
					bool status = true;
					String header = stream.gets(&status);
					if(!status || header != "ExternArchiveStream") return false;
					
					// create files
					for(uint32_t i = 0; i < 4; i++) {
						files.append(String::format("file_%u.txt", i));
					}
					
					return true;
				}
				
				// files list
				virtual uint32_t getNumFiles() const { return files.size(); }
				virtual const String &getFileName(uint32_t index) const { return files[index]; }
				virtual uint64_t getFileMTime(uint32_t index) const { return index + 10; }
				virtual size_t getFileSize(uint32_t index) const { return index + 100; }
				
				// open file
				virtual Stream openFile(uint32_t index) {
					
					Blob blob;
					
					// write blob data
					blob.puts(files[index]);
					
					// seek to the beginning
					blob.seek(0);
					
					// move blob
					return blob.move();
				}
				
			private:
				
				Array<String> files;
		};
		
		// register archive format
		ExternArchiveStream archive_stream("eas");
		
		{
			Archive archive;
			if(!archive.open("test_archive.eas")) return 1;
			TS_LOGF(Message, "%s: %u\n", archive.getName().get(), archive.getNumFiles());
			
			for(uint32_t i = 0; i < archive.getNumFiles(); i++) {
				
				String name = archive.getFileName(i);
				TS_LOGF(Message, "%2u: %u | %u | %s\n", i, (uint32_t)archive.getFileSize(i), (uint32_t)archive.getFileMTime(i), name.get());
				
				Stream stream = archive.openFile(name.get());
				if(!stream) return 1;
				
				if(stream.gets() != name) return 2;
			}
		}
	}
	
	return 0;
}
