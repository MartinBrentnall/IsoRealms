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
   * Resource definition for a simple Vertex variable.
   */
  class SimpleVertex final : public IVertex {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    SimpleVertex(Basics& basics, IResourceData& data);
    SimpleVertex(Basics& basics, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(PropertyMaker& owner, const Metadata& metadata);

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    /**********************\
     * Implements IVertex *
    \**********************/
    double getX() const override;
    double getY() const override;
    double getZ() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setX(double x);
    void setY(double y);
    void setZ(double z);

    private:

    // JSON members.
    static const std::string JSON_X;
    static const std::string JSON_Y;
    static const std::string JSON_Z;

    // Property names.
    static const std::string PROPERTY_X;
    static const std::string PROPERTY_Y;
    static const std::string PROPERTY_Z;

    // Definition data.
    float cDefX; /// Initial X value.
    float cDefY; /// Initial Y value.
    float cDefZ; /// Initial Z value.

    // Runtime data.
    double cRuntimeX; /// Current X value.
    double cRuntimeY; /// Current Y value.
    double cRuntimeZ; /// Current Z value.

    // Scripting Interface.
    LuaBinding<SimpleVertex> cLuaBinding;

    // Misc.
    IStateNotifier<IVertex>* cStateNotifier;
  };
}
