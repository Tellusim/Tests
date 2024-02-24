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
#include <format/TellusimMesh.h>

/*
 */
using namespace Tellusim;

/*
 */
void print_nodes(const Mesh &mesh, const MeshNode &node, uint32_t offset) {
	
	TS_LOGF(Message, "%d:%s<%s> %u %u %d\n", mesh.findNode(node), String(offset, ' ').get(), node.getName().get(),
		node.getNumChildren(), node.getNumGeometries(), mesh.findNode(node.getParent()));
	
	for(uint32_t i = 0; i < node.getNumChildren(); i++) {
		print_nodes(mesh, node.getChild(i), offset + 1);
	}
}

/*
 */
int32_t main(int32_t argc, char **argv) {
	
	if(1) {
		
		Mesh mesh;
		
		if(!mesh.load("test_box.fbx")) return 1;
		
		// mesh nodes
		if(mesh.getNumNodes()) {
			TS_LOGF(Message, "nodes %u\n", mesh.getNumNodes());
			for(const MeshNode &node : mesh.getNodes()) {
				if(node.getParent()) continue;
				print_nodes(mesh, node, 1);
			}
		}
		
		// mesh geometries
		if(mesh.getNumGeometries()) {
			TS_LOGF(Message, "geometries %u\n", mesh.getNumGeometries());
			for(const MeshGeometry &geometry : mesh.getGeometries()) {
				TS_LOGF(Message, "%u: <%s>\n", geometry.getIndex(), geometry.getName().get());
				
				TS_LOGF(Message, " indices: %u\n", geometry.getNumIndices());
				for(uint32_t j = 0; j < geometry.getNumIndices(); j++) {
					const MeshIndices &indices = geometry.getIndices(j);
					TS_LOGF(Message, "  %u: %s %s <%s> %u\n", j, indices.getTypeName(), indices.getFormatName(), indices.getName().get(), indices.getSize());
				}
				
				TS_LOGF(Message, " attributes: %u\n", geometry.getNumAttributes());
				for(uint32_t j = 0; j < geometry.getNumAttributes(); j++) {
					const MeshAttribute &attribute = geometry.getAttribute(j);
					TS_LOGF(Message, "  %u: %s %s <%s> %u %u\n", j, attribute.getTypeName(), attribute.getFormatName(), attribute.getName().get(), attribute.getSize(), geometry.findIndices(attribute.getIndices()));
				}
				
				// materials
				if(geometry.getNumMaterials()) {
					TS_LOGF(Message, " materials: %u\n", geometry.getNumMaterials());
					for(uint32_t j = 0; j < geometry.getNumMaterials(); j++) {
						const MeshMaterial &material = geometry.getMaterial(j);
						TS_LOGF(Message, "  %u: <%s>\n", j, material.getName().get());
					}
				}
				
				// joints
				if(geometry.getNumJoints()) {
					TS_LOGF(Message, " joints: %u\n", geometry.getNumJoints());
					for(uint32_t j = 0; j < geometry.getNumJoints(); j++) {
						const MeshJoint &joint = geometry.getJoint(j);
						TS_LOGF(Message, "  %u: <%s> %d\n", j, joint.getName().get(), mesh.findNode(joint.getNode()));
					}
				}
			}
		}
		
		// mesh animations
		if(mesh.getNumAnimations()) {
			TS_LOGF(Message, "animations %u\n", mesh.getNumAnimations());
			for(uint32_t i = 0; i < mesh.getNumAnimations(); i++) {
				const MeshAnimation &animation = mesh.getAnimation(i);
				TS_LOGF(Message, "%u: <%s> %f - %f\n", i, animation.getName().get(), animation.getMinTime(), animation.getMaxTime());
			}
		}
		
		// save mesh
		if(!mesh.save("test_save.obj")) return 1;
		if(!mesh.save("test_save.glb")) return 1;
		if(!mesh.save("test_save.gltf")) return 1;
		if(!mesh.save("test_save.mesh", Mesh::Flag32Bit)) return 1;
	}
	
	return 0;
}
