/*
 * Copyright 2023 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <cmath>

#include "IsoRealms/Editing.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for a model that can be used to represent a chunk
   * of debris from a broken craft or other object.  Specifically, it is a
   * tetrahedron with configurable colours for the outline and for each of its
   * four faces.
   */
  class DebrisChunk final : public IModel,
                            public IModelInstance {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    DebrisChunk(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    DebrisChunk(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner);

    /*********************\
     * Implements IModel *
    \*********************/
    IModelInstance* createModel() override;
    bool renderPreview() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    /*****************************\
     * Implements IModelInstance *
    \*****************************/
    void update(unsigned int milliseconds) override;
    void render() const override;

    private:

    // JSON members.
    static const std::string JSON_OUTLINE;
    static const std::string JSON_OUTLINE_WIDTH;
    static const std::string JSON_SIDE_1;
    static const std::string JSON_SIDE_2;
    static const std::string JSON_SIDE_3;
    static const std::string JSON_SIDE_4;

    // Default constants.
    static const float DEFAULT_OUTLINE_WIDTH;

    ProjectCallbackManager cProjectCallbackManager;
    IProject& cProject;

    // Definition data.
    Colour cDefSide[4];               /// Colours used for each side of this debris chunk.
    Colour cDefOutline;               /// Colour used for the outline of this debris chunk.
    float cDefOutlineWidth;           /// Width of the outline drawn on each side of this debris chunk.

    // Runtime data.
    LiteralTexture cTextures[4];          /// Textures applied to each side of this debris chunk.
    bool cNeedsRedrawing;

    // Editing data.
    float cEditingIconRotation; /// Animation value for rotation of the model icon.

    // Internal functions.
    void regenerateTextures();
    void generateTexture(Colour& colour);
    void setNeedsRedrawing();
  };
}
