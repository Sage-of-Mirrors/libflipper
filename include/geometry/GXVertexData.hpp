#pragma once

#include "GXGeometryEnums.hpp"

#include <cstdint>
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

// Represents a model's per-vertex attribute data (position, normals, etc).
struct GXAttributeData {
	std::vector<glm::vec4> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec4> Colors[2];
	std::vector<glm::vec3> TexCoords[8];

	bool HasPositions() { return Positions.size() != 0; }
	bool HasNormals() { return Normals.size() != 0; }
	bool HasColors(uint32_t index) { return index >= 0 && index < 2 && Colors[index].size() != 0; }
	bool HasTexCoords(uint32_t index) { return index >= 0 && index < 8 && TexCoords[index].size() != 0; }
};

// Represents the format that a per-vertex attribute's data is stored in.
struct GXVertexAttributeFormat {
	EGXAttribute Attribute;
	EGXComponentCount ComponentCount;
	EGXComponentType ComponentType;

	GXVertexAttributeFormat();
};

// Represents a vertex found inside a primitive.
class GXVertex {
	uint16_t AttributeIndices[(uint32_t)EGXAttribute::Attribute_Max];

public:
	GXVertex();

	uint16_t GetIndex(EGXAttribute attribute);
	void SetIndex(EGXAttribute attribute, uint16_t value);

	bool operator==(const GXVertex& b) const;
	bool operator!=(const GXVertex& b) const { return !operator==(b); };
};
