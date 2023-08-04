#include "geometry/GXGeometryData.hpp"

#include <algorithm>
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
    std::vector<ModernVertex> Triangles;

    for (size_t i = 2; i < mVertices.size(); i++) {
        bool isIndexOdd = i % 2 != 0;

        ModernVertex const& v0 = mVertices[i - 2];
        ModernVertex const& v1 = isIndexOdd ? mVertices[i] : mVertices[i - 1];
        ModernVertex const& v2 = isIndexOdd ? mVertices[i - 1] : mVertices[i];

        if (v0 == v1 || v0 == v2 || v1 == v2)
            continue;

        Triangles.push_back(v0);
        Triangles.push_back(v1);
        Triangles.push_back(v2);
    }

    mVertices = std::move(Triangles);
}

void GXPrimitive::TriangulateTriangleFan() {
    std::vector<ModernVertex> Triangles;

    for (size_t i = 1; i < mVertices.size() - 1; i++) {
        ModernVertex const& v0 = mVertices[i];
        ModernVertex const& v1 = mVertices[i + 1];
        ModernVertex const& v2 = mVertices[0];

        // Reject degenerate triangles (triangles where two or more vertices are the same)
        if (v0 == v1 || v0 == v2 || v1 == v2)
            continue;

        Triangles.push_back(v0);
        Triangles.push_back(v1);
        Triangles.push_back(v2);
    }

    mVertices = std::move(Triangles);
}

void GXShape::GetVertexOffsetAndCount(uint32_t& offset, uint32_t& count) const {
    offset = mFirstVertexOffset;
    count = mVertexCount;
}

void GXShape::CalculateCenterOfMass() {
    size_t vertexCount = 0;
    glm::vec3 center(0.0f, 0.0f, 0.0f);

    for (const GXPrimitive* p : mPrimitives)
    {
        for (const ModernVertex& v : p->GetVertices())
        {
            center.x += v.Position.x;
            center.y += v.Position.y;
            center.z += v.Position.z;

            vertexCount++;
        }
    }

    if (vertexCount == 0)
    {
        return;
    }

    mCenterOfMass.x = center.x / vertexCount;
    mCenterOfMass.y = center.y / vertexCount;
    mCenterOfMass.z = center.z / vertexCount;
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

    uint32_t PosMatIndex = 0;

    for (EGXAttribute Attribute : vat) {
        switch (Attribute) {
            case EGXAttribute::Position:
                NewVertex.Position = Attributes.GetPositions()[Vertex.GetIndex(Attribute)];
                NewVertex.Position.w = Vertex.GetIndex(EGXAttribute::PositionMatrixIdx);
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

void GXGeometry::CreateVertexArray() {
    size_t index = 0;

    for (GXShape* Shape : mShapes) {
        std::vector<GXPrimitive*>& Primitives = Shape->GetPrimitives();
        std::vector<EGXAttribute> AttributeTable = Shape->GetAttributeTable();

        Shape->mFirstVertexOffset = static_cast<uint32_t>(mModelIndices.size());

        // ...iterate the primitive data...
        for (GXPrimitive* Prim : Primitives) {
            Prim->TriangluatePrimitive();
            std::vector<ModernVertex>& Vertices = Prim->GetVertices();

            // ...and process each vertex into a
            // ModernVertex (containing the actual vertex data) and an index.
            for (ModernVertex& vertex : Vertices) {
                mModelVertices.push_back(vertex);
                mModelIndices.push_back(static_cast<uint32_t>(index++));
            }
        }

        Shape->mVertexCount = static_cast<uint32_t>(mModelIndices.size()) - Shape->mFirstVertexOffset;
    }
}
