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

#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for a screen that glows with a configured colour at
   * the edges, to indicate when damage is taken.
   */
  class DamageIndicator final : public IScreen {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    DamageIndicator(IProject* project, Spindizzy* spindizzy);
    DamageIndicator(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void registerAssets(IAssetRegistry* assets);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;

    private:

    // DOM strings.
    static const std::string TAG_COLOUR;
    static const std::string TAG_SIZE;

    // Definition data.
    Colour cDefColour;
    Float cDefSize;
  };
}
