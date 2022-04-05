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

ptrdiff_t VectorIndexOf(const std::vector<GXVertex>& vec, const GXVertex& elem)
{
	ptrdiff_t result = -1;

	auto it = std::find(vec.begin(), vec.end(), elem);
	if (it != vec.end())
		result = it - vec.begin();

	return result;
}

bool VectorContains(const std::vector<GXVertex>& vec, const GXVertex& elem, ptrdiff_t& index)
{
	index = VectorIndexOf(vec, elem);
	return index != -1;
}

ModernVertex GXVertexToModern(const GXAttributeData& Attributes, const std::vector<EGXAttribute>& vat, const GXVertex& Vertex) {
	ModernVertex NewVertex;

	for (EGXAttribute Attribute : vat) {
		switch (Attribute) {
			case EGXAttribute::Position:
				NewVertex.Position = Attributes.GetPositions()[Vertex.GetIndex(Attribute)];
				break;
			case EGXAttribute::Normal:
				NewVertex.Normal = Attributes.GetNormals()[Vertex.GetIndex(Attribute)];
				break;
			case EGXAttribute::Color0:
			case EGXAttribute::Color1:
			{
				uint32_t index = (uint32_t)Attribute - (uint32_t)EGXAttribute::Color0;
				NewVertex.Colors[index] = Attributes.GetColors(index)[Vertex.GetIndex(Attribute)];
				break;
			}
			case EGXAttribute::TexCoord0:
			case EGXAttribute::TexCoord1:
			case EGXAttribute::TexCoord2:
			case EGXAttribute::TexCoord3:
			case EGXAttribute::TexCoord4:
			case EGXAttribute::TexCoord5:
			case EGXAttribute::TexCoord6:
			case EGXAttribute::TexCoord7:
			{
				uint32_t index = (uint32_t)Attribute - (uint32_t)EGXAttribute::TexCoord0;
				NewVertex.TexCoords[index] = Attributes.GetTexCoords(index)[Vertex.GetIndex(Attribute)];
				break;
			}
		}
	}

	return NewVertex;
}

void GXGeometry::ModernizeGeometry(GXAttributeData& AttributeData) {
	std::vector<GXVertex> UniqueGXVertices;
	ptrdiff_t Index = -1;

	// For each shape...
	for (GXShape& Shape : mShapes) {
		std::vector<GXPrimitive>& Primitives = Shape.GetPrimitives();
		std::vector<EGXAttribute> AttributeTable = Shape.GetAttributeTable();

		Shape.mFirstVertexOffset = mModelIndices.size();

		// ...iterate the primitive data...
		for (GXPrimitive& Prim : Primitives) {
			Prim.TriangluatePrimitive();
			std::vector<GXVertex>& Vertices = Prim.GetVertices();

			// ...and process each vertex into a
			// ModernVertex (containing the actual vertex data) and an index.
			for (GXVertex& Vertex : Vertices) {
				if (!VectorContains(UniqueGXVertices, Vertex, Index)) {
					Index = UniqueGXVertices.size();
					UniqueGXVertices.push_back(Vertex);

					mModelVertices.push_back(GXVertexToModern(AttributeData, AttributeTable, Vertex));
				}

				mModelIndices.push_back(Index);
			}
		}

		Shape.mVertexCount = mModelIndices.size() - Shape.mFirstVertexOffset;
	}
}
