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
   * Resource definition for a simple Vertex variable.
   */
  class SimpleVertex final : public IVertex {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    SimpleVertex(IProject* project, Basics* basics);
    SimpleVertex(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /**********************\
     * Implements IVertex *
    \**********************/
    double getX() const override;
    double getY() const override;
    double getZ() const override;
    bool renderAssetIcon() const override;

    /***********************\
     * Scripting Interface *
    \***********************/
    void setX(double x);
    void setY(double y);
    void setZ(double z);

    private:

    // DOM strings.
    static const std::string ATTRIBUTE_X;
    static const std::string ATTRIBUTE_Y;
    static const std::string ATTRIBUTE_Z;

    // Property names.
    static const std::string PROPERTY_X;
    static const std::string PROPERTY_Y;
    static const std::string PROPERTY_Z;

    // Definition data.
    double cDefX; /// Initial X value.
    double cDefY; /// Initial Y value.
    double cDefZ; /// Initial Z value.

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
