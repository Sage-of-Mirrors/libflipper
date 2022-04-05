#pragma once

#include "GXGeometryEnums.hpp"
#include "GXVertexData.hpp"

#include <cstdint>
#include <vector>

// Represents a single primitive made up of a list of vertices.
class GXPrimitive {
	// What kind of shape the vertices in this primitive make - triangles, quads, etc.
	EGXPrimitiveType mType;
	// The vertices making up this primitive.
	std::vector<GXVertex> mVertices;

	// Converts this primitive from triangle strip to triangles.
	void TriangulateTriangleStrip();
	// Converts this primitive from triangl fan to triangles.
	void TriangulateTriangleFan();

public:
	GXPrimitive() : mType(EGXPrimitiveType::None) {}

	// Returns this primitive's type.
	EGXPrimitiveType GetType() const { return mType; }

	// Returns a reference to this primitive's list of vertices.
	std::vector<GXVertex>& GetVertices() { return mVertices; }
	// Returns a const reference to this primitive's list of vertices.
	const std::vector<GXVertex>& GetVertices() const { return mVertices; }

	// Reconfigures the indices in this primitive from whatever its
	// original primitive type was to triangles.
	void TriangluatePrimitive();
};


// Represents a set of primitives sharing the same Vertex Attribute Table setup,
// i.e. a set of primitives with the same attributes enabled.
class GXShape {
	// A list that indicates which attributes are enabled for the primitives in this shape.
	std::vector<EGXAttribute> mVertexAttributeTable;
	// The primitives that make up this shape.
	std::vector<GXPrimitive> mPrimitives;

	// The offset of this shape's first vertex index in the model index list.
	uint32_t mFirstVertexOffset;
	// The total number of vertex indices that this shape has in the model index list.
	uint32_t mVertexCount;

	// Arbitrary data that can be associated with this shape.
	void* mUserData;

public:
	GXShape() : mFirstVertexOffset(0), mVertexCount(0), mUserData(nullptr) {}

	// Returns a reference to this shape's list of enabled attributes.
	std::vector<EGXAttribute>& GetAttributeTable() { return mVertexAttributeTable; }
	// Returns a reference to this shape's list of primitives.
	std::vector<GXPrimitive>& GetPrimitives() { return mPrimitives; }
	
	// Returns a const reference to this shape's list of enabled attributes.
	const std::vector<EGXAttribute>& GetAttributeTable() const { return mVertexAttributeTable; }
	// Returns a const reference to this shape's list of primitives.
	const std::vector<GXPrimitive>& GetPrimitives() const { return mPrimitives; }

	// Fills the input references with the offset of this shape's first index in the global index list
	// and the number of indices belonging to it.
	void GetVertexOffsetAndCount(uint32_t& offset, uint32_t& count);

	void* GetUserData() { return mUserData; }
	void SetUserData(void* data) { mUserData = data; }
};

// Represents all of the geometry for a given model.
class GXGeometry {
	// The geometry data that makes up this model.
	std::vector<GXShape> mShapes;
	// All the vertex indices in the model, collated for one-and-done uploading to the GPU.
	std::vector<uint16_t> mModelIndices;

public:
	// Returns a reference to the list of shapes in this model.
	std::vector<GXShape>& GetShapes() { return mShapes; }
	// Returns a reference to the list of all vertex indices in this model.
	std::vector<uint16_t>& GetModelIndices() { return mModelIndices; }

	// Returns a const reference to the list of shapes in this model.
	const std::vector<GXShape>& GetShapes() const { return mShapes; }
	// Returns a const reference to the list of all vertex indices in this model.
	const std::vector<uint16_t>& GetModelIndices() const { return mModelIndices; }

	// Processes the loaded geometry to be easier for modern GPUs to render.
	void ModernizeGeometry();
};
