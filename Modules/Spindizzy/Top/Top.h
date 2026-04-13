/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <cmath>

#include "IsoRealms.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for a top craft model, where the colour of the top,
   * sides, and outline is configurable.
   */
  class Top final : public IModel,
                    public IModelInstance {
    public:
    Top(Spindizzy& spindizzy, IResourceData& data);
    Top(Spindizzy& spindizzy, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Module interfaces *
    \*********************/
    void updateEditing(unsigned int milliseconds);

    /*********************\
     * Implements IModel *
    \*********************/
    IModelInstance* createModel() override;
    bool renderPreview() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /*****************************\
     * Implements IModelInstance *
    \*****************************/
    void update(unsigned int milliseconds) override;
    void render() const override;
    
    private:

    // JSON members.
    static const std::string JSON_OUTLINE;
    static const std::string JSON_SIDES;
    static const std::string JSON_TOP;

    // Resource definition constants.
    static const float OUTLINE;
    static const float SPINDLE_WIDTH;
    static const float SPINDLE_HEIGHT;
    static const float WIDTH;
    static const float HEIGHT;

    // External interfaces.
    Project& cProject;

    // Definition data.
    Colour cDefColourTop;     /// Colour of the top.
    Colour cDefColourSide;    /// Colour of the sides.
    Colour cDefColourOutline; /// Colour of the outline.

    // Runtime data.
    LiteralTexture cRuntimeTextureTop;  /// Texture used on the top.
    LiteralTexture cRuntimeTextureSide; /// Texture used on the sides.
    bool cNeedsRedrawing;

    // Editing data.
    float cEditingIconAngle; /// Angle used to animate the icon in the editor.

    // Internal functions.
    void updateTextures();
    void generateTextureTop();
    void generateTextureSide();
    void setNeedsRedrawing();
  };
}
