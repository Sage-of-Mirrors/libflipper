#include "geometry/GXVertexData.hpp"

GXVertex::GXVertex() {
    for (uint32_t i = 0; i < (uint32_t)EGXAttribute::Attribute_Max; i++) {
        AttributeIndices[i] = UINT16_MAX;
    }
}

ModernVertex::ModernVertex() {
    Position = glm::vec4(0, 0, 0, 0);
    Normal = glm::vec3(0, 0, 0);

    Colors[0] = glm::vec4(0, 0, 0, 0);
    Colors[1] = glm::vec4(0, 0, 0, 0);

    TexCoords[0] = glm::vec3(0, 0, 0);
    TexCoords[1] = glm::vec3(0, 0, 0);
    TexCoords[2] = glm::vec3(0, 0, 0);
    TexCoords[3] = glm::vec3(0, 0, 0);
    TexCoords[4] = glm::vec3(0, 0, 0);
    TexCoords[5] = glm::vec3(0, 0, 0);
    TexCoords[6] = glm::vec3(0, 0, 0);
    TexCoords[7] = glm::vec3(0, 0, 0);
}

uint16_t GXVertex::GetIndex(EGXAttribute attribute) const {
    uint32_t attrAsInt = (uint32_t)attribute;

    if (attrAsInt >= (uint32_t)EGXAttribute::PositionMatrixIdx && attrAsInt < (uint32_t)EGXAttribute::Attribute_Max) {
        return AttributeIndices[attrAsInt];
    }

    return UINT16_MAX;
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
