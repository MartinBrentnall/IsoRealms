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

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Component definition for a simple Boolean variable.
   */
  class SimpleBoolean final : public IBoolean {
    public:

    /***********************\
     * Component Interface *
    \***********************/
    SimpleBoolean(Basics& basics, IComponentData& data);
    void registerAssets(ComponentAssetRegistry& assets);
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    /***********************\
     * Implements IBoolean *
    \***********************/
    bool getValue() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(bool value);

    private:
    // Definition data.
    bool cDefValue;     /// Initial value.

    // Runtime data.
    bool cRuntimeValue; /// Current value.

    // Scripting Interface.
    LuaBinding<SimpleBoolean> cLuaBinding;

    // Misc.
    IStateNotifier* cStateNotifier;
  };
}
