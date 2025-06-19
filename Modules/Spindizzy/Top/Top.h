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
   * Resource definition for a top craft model, where the colour of the top,
   * sides, and outline is configurable.
   */
  class Top final : public IModel,
                    public IModelInstance {
    public:
    Top(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    Top(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner, IAssetBrowser& browser, IAssetRegistry& assets);

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
    static const std::string JSON_SIDES;
    static const std::string JSON_TOP;

    // Resource definition constants.
    static const float OUTLINE;
    static const float SPINDLE_WIDTH;
    static const float SPINDLE_HEIGHT;
    static const float WIDTH;
    static const float HEIGHT;

    // External interfaces.
    ProjectCallbackManager cProjectCallbackManager;
    IProject& cProject;

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
