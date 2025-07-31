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

#include <vector>

#include "IsoRealms.h"

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
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(PropertyMaker& owner, const Metadata& metadata);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
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
