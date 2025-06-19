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

#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a simple Boolean variable.
   */
  class SimpleBoolean final : public IBoolean {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    SimpleBoolean(IProject& project, Basics& basics, IResourceData& data);
    SimpleBoolean(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner, IAssetBrowser& browser, IAssetRegistry& assets);

    /***********************\
     * Implements IBoolean *
    \***********************/
    bool getValue() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(bool value);

    private:

    // JSON members.
    static const std::string JSON_VALUE;

    // Property names.
    static const std::string PROPERTY_VALUE;

    // External interfaces.
    ProjectCallbackManager cProjectCallbackManager;

    // Definition data.
    bool cDefValue;     /// Initial value.

    // Runtime data.
    bool cRuntimeValue; /// Current value.

    // Scripting Interface.
    LuaBinding<SimpleBoolean> cLuaBinding;

    // Misc.
    IStateNotifier<IBoolean>* cStateNotifier;
  };
}
