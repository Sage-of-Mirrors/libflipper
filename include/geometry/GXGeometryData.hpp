#pragma once

#include "GXGeometryEnums.hpp"
#include "GXVertexData.hpp"

#include <cstdint>
#include <vector>

// Represents a single primitive made up of a list of vertices.
class GXPrimitive {
	EGXPrimitiveType mType;
	std::vector<GXVertex> mVertices;

	void TriangulateTriangleStrip();
	void TriangulateTriangleFan();

public:
	GXPrimitive();

	EGXPrimitiveType GetType() const { return mType; }
	// Returns a reference to this primitive's list of vertices.
	std::vector<GXVertex>& GetVertices() { return mVertices; }

	// Reconfigures the indices in this primitive from whatever its
	// original primitive type was to triangles.
	void TriangluatePrimitive();
};


// Represents a set of primitives sharing the same Vertex Attribute Table setup,
// i.e. a set of primitives with the same attributes enabled.
struct GXShape {
	std::vector<EGXAttribute> mVertexAttributeTable;
	std::vector<GXPrimitive> mPrimitives;
};
