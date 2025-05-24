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

#include <algorithm>
#include <vector>

#include "IsoRealms/Editing.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Basics/AssetSwitchWithTransition.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a switchable Colour variable based on
   * AssetSwitchWithTransition.
   */
  class ColourSwitch final : public AssetSwitchWithTransition<IColour> {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    ColourSwitch(IProject& project, Basics& basics, IResourceData& data);
    ColourSwitch(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    bool renderIcon();
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);

    /**********************\
     * Implements IColour *
    \**********************/
    void set() const override;
    float getRed() const override;
    float getGreen() const override;
    float getBlue() const override;
    float getAlpha() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_COLOUR;

    // definition data.
    Colour cDefColour; /// Initial value.

    // Scripting Interface.
    LuaBinding<ColourSwitch> cLuaBinding;
  };
}
