#include "geometry/GXVertexData.hpp"

GXVertexAttributeFormat::GXVertexAttributeFormat() : Attribute(EGXAttribute::Null),
	ComponentCount(EGXComponentCount::Position_XY), ComponentType(EGXComponentType::Unsigned8) {

}

uint16_t GXVertex::GetIndex(EGXAttribute attribute) {
	uint32_t attrAsInt = (uint32_t)attribute;

	if (attrAsInt >= (uint32_t)EGXAttribute::PositionMatrixIdx && attrAsInt < (uint32_t)EGXAttribute::Attribute_Max) {
		return AttributeIndices[attrAsInt];
	}
}

void GXVertex::SetIndex(EGXAttribute attribute, uint16_t value) {
	uint32_t attrAsInt = (uint32_t)attribute;

	if (attrAsInt >= (uint32_t)EGXAttribute::PositionMatrixIdx && attrAsInt < (uint32_t)EGXAttribute::Attribute_Max) {
		AttributeIndices[attrAsInt] = value;
	}
}

bool GXVertex::operator==(const GXVertex& b) const {
	for (uint32_t i = 0; i < (uint32_t)EGXAttribute::Attribute_Max; i++) {
		if (AttributeIndices[i] != b.AttributeIndices[i])
			return false;
	}

	return true;
}
