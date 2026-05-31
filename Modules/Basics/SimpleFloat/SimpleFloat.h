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

#include <queue>

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a simple Float variable.
   */
  class SimpleFloat final : public IFloat {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    SimpleFloat(Basics& basics, IResourceData& data);
    SimpleFloat(Basics& basics, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    /*********************\
     * Implements IFloat *
    \*********************/
    float getValue() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setValue(float value);

    private:

    // JSON members.
    inline static const std::string JSON_VALUE = "value";

    // Property names.
    inline static const std::string PROPERTY_VALUE = "Initial Value";

    // Definition data
    float cDefValue;

    // Runtime data
    float cRuntimeValue;

    // Scripting Interface.
    LuaBinding<SimpleFloat> cLuaBinding;

    // Misc.
    IStateNotifier* cStateNotifier;
  };
}
