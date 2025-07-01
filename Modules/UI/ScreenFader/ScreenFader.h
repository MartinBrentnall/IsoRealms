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

#include <vector>

#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::UI {
  class UI;

  /**
   * Resource definition for a switchable input handler.
   */
  class ScreenFader final : public IScreen {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    ScreenFader(IProject& project, UI& ui, IResourceData& data);
    ScreenFader(IProject& project, UI& ui, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;
    
    private:

    // JSON members.
    static const std::string JSON_SCREEN_A;
    static const std::string JSON_SCREEN_B;
    static const std::string JSON_TRANSITION;

    // External interfaces.
    IProject& cProject;

    // Definition data.
    Screen cDefScreenA;
    Screen cDefScreenB;
    Float cDefTransition;
    
    // Runtime data.
    LiteralTexture cRuntimeScreenA;
    LiteralTexture cRuntimeScreenB;
  };
}
