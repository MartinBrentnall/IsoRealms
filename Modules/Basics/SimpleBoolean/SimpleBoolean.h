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
    SimpleBoolean(IProject* project, Basics* basics);
    SimpleBoolean(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /***********************\
     * Implements IBoolean *
    \***********************/
    bool getValue() const override;
    bool renderAssetIcon() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(bool value);

    private:

    // DOM strings.
    static const std::string ATTRIBUTE_VALUE;

    // Property names.
    static const std::string PROPERTY_VALUE;

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
