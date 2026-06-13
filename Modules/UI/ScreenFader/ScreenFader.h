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
    ScreenFader(UI& ui, IResourceData& data);
    void registerAssets(ResourceAssetRegistry& assets);
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;
    
    private:
    
    // External interfaces.
    Project& cProject;

    // Definition data.
    Screen cDefScreenA;
    Screen cDefScreenB;
    Float cDefTransition;
    
    // Runtime data.
    LiteralTexture cRuntimeScreenA;
    LiteralTexture cRuntimeScreenB;
  };
}
