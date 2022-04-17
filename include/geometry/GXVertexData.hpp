#pragma once

#include "GXGeometryEnums.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <cstdint>
#include <vector>
#include <stdexcept>

// Represents a model's per-vertex attribute data (position, normals, etc).
class GXAttributeData {
    // A list of all the positions the model uses.
    std::vector<glm::vec4> mPositions;
    // A list of all the normals the model uses.
    std::vector<glm::vec3> mNormals;
    // Lists of all the colors the model uses. GX supports up to 2 color channels per vertex.
    std::vector<glm::vec4> mColors[2];
    // Lists of all the tex coords the modeluses. GX supports up to 8 tex coord channels per vertex.
    std::vector<glm::vec3> mTexCoords[8];

public:
    // Returns a reference to the model's list of position data.
    std::vector<glm::vec4>& GetPositions() { return mPositions; }
    // Returns a reference to the model's list of normal data.
    std::vector<glm::vec3>& GetNormals() { return mNormals; }
    // Returns a reference to the model's list of color data for the given channel.
    std::vector<glm::vec4>& GetColors(uint32_t index) {
        if (index >= 0 && index < 2)
            return mColors[index];
        else
            throw std::out_of_range("Specified invalid vertex color ID! (range is 0 to 1)");
    }
    // Returns a reference to the model's list of tex coord data for the given channel.
    std::vector<glm::vec3>& GetTexCoords(uint32_t index) {
        if (index >= 0 && index < 8)
            return mTexCoords[index];
        else
            throw std::out_of_range("Specified invalid vertex tex coord ID! (range is 0 to 7)");
    }

    // Returns a const reference to the model's list of position data.
    const std::vector<glm::vec4>& GetPositions() const { return mPositions; }
    // Returns a const reference to the model's list of normal data.
    const std::vector<glm::vec3>& GetNormals() const { return mNormals; }
    // Returns a const reference to the model's list of color data for the given channel.
    const std::vector<glm::vec4>& GetColors(uint32_t index) const {
        if (index >= 0 && index < 2)
            return mColors[index];
        else
            throw std::out_of_range("Specified invalid vertex color ID! (range is 0 to 1)");
    }
    // Returns a const reference to the model's list of tex coord data for the given channel.
    const std::vector<glm::vec3>& GetTexCoords(uint32_t index) const {
        if (index >= 0 && index < 8)
            return mTexCoords[index];
        else
            throw std::out_of_range("Specified invalid vertex tex coord ID! (range is 0 to 7)");
    }

    // Returns whether this model has position data.
    bool HasPositions() { return mPositions.size() != 0; }
    // Returns whether this model has normal data.
    bool HasNormals() { return mNormals.size() != 0; }
    // Returns whether the given channel in this model has color data.
    bool HasColors(uint32_t index) { return index >= 0 && index < 2 && mColors[index].size() != 0; }
    // Returns whether the given channel in this model has tex coord data.
    bool HasTexCoords(uint32_t index) { return index >= 0 && index < 8 && mTexCoords[index].size() != 0; }
};

// Represents the format that a per-vertex attribute's data is stored in.
struct GXVertexAttributeFormat {
    // What attribute this format is for.
    EGXAttribute Attribute;
    // What components make up this attribute; varies depending on the attribute type.
    EGXComponentCount ComponentCount;
    // The underlying type of the attribute data - float, int16, etc.
    EGXComponentType ComponentType;

    GXVertexAttributeFormat() : Attribute(EGXAttribute::Null), ComponentCount(EGXComponentCount::Position_XY), ComponentType(EGXComponentType::Unsigned8) {}
};

// Represents a vertex found inside a primitive.
class GXVertex {
    // The indices that point to the data that this vertex is made up of.
    uint16_t AttributeIndices[(uint32_t)EGXAttribute::Attribute_Max];

public:
    GXVertex();

    // Returns the index for the given attribute. Returns UINT16_MAX if the attribute is invalid.
    uint16_t GetIndex(EGXAttribute attribute) const;
    // Sets the given attribute to the given value.
    void SetIndex(EGXAttribute attribute, uint16_t value);

    bool operator==(const GXVertex& b) const;
    bool operator!=(const GXVertex& b) const { return !operator==(b); };
};

// Represents a vertex for use with modern GPUs.
struct ModernVertex {
    glm::vec4 Position;
    glm::vec3 Normal;
    glm::vec4 Colors[2];
    glm::vec3 TexCoords[8];

    ModernVertex();
};
