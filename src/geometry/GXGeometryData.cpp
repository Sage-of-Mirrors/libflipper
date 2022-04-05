#include "geometry/GXGeometryData.hpp"

#include <utility>

void GXPrimitive::TriangluatePrimitive() {
	switch (mType) {
		case EGXPrimitiveType::TriangleStrips:
			TriangulateTriangleStrip();
			break;
		case EGXPrimitiveType::TriangleFan:
			TriangulateTriangleFan();
			break;
		case EGXPrimitiveType::Triangles:
		default:
			break;
	}
}

void GXPrimitive::TriangulateTriangleStrip() {
	std::vector<GXVertex> Triangles;

	for (size_t i = 2; i < mVertices.size(); i++) {
		bool isIndexOdd = i % 2 != 0;

		GXVertex const& v0 = mVertices[i - 2];
		GXVertex const& v1 = isIndexOdd ? mVertices[i] : mVertices[i - 1];
		GXVertex const& v2 = isIndexOdd ? mVertices[i - 1] : mVertices[i];

		Triangles.push_back(v0);
		Triangles.push_back(v1);
		Triangles.push_back(v2);
	}

	mVertices = std::move(Triangles);
}

void GXPrimitive::TriangulateTriangleFan() {
	std::vector<GXVertex> Triangles;

	for (size_t i = 1; i < mVertices.size() - 1; i++) {
		GXVertex const& v0 = mVertices[i];
		GXVertex const& v1 = mVertices[i + 1];
		GXVertex const& v2 = mVertices[0];

		// Reject degenerate triangles (triangles where two or more vertices are the same)
		if (v0 == v1 || v0 == v2 || v1 == v2)
			continue;

		Triangles.push_back(v0);
		Triangles.push_back(v1);
		Triangles.push_back(v2);
	}

	mVertices = std::move(Triangles);
}

void GXShape::GetVertexOffsetAndCount(uint32_t& offset, uint32_t& count) {
	offset = mFirstVertexOffset;
	count = mVertexCount;
}
