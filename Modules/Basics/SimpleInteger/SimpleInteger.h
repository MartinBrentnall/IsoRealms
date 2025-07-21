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
   * Resource definition for a simple integer variable.
   */
  class SimpleInteger final : public IInteger {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    SimpleInteger(IProject& project, Basics& basics, IResourceData& data);
    SimpleInteger(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IPropertyOwner& owner);

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    /***********************\
    * Implements IInteger *
    \***********************/
    int getValue() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) override;
    bool isDefaultConfiguration() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(int value);

    private:

    // JSON members.
    static const std::string JSON_VALUE;

    // Property names.
    static const std::string PROPERTY_VALUE;

    // Definition data.
    int cDefValue;     /// Initial configured value.

    // Runtime data.
    int cRuntimeValue; /// Current value at runtime.

    // Scripting Interface.
    LuaBinding<SimpleInteger> cLuaBinding;

    // Misc.
    IStateNotifier<IInteger>* cStateNotifier;
  };
}
