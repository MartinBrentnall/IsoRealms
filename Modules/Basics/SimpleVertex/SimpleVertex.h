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
   * Component definition for a simple Vertex variable.
   */
  class SimpleVertex final : public IVertex {
    public:

    /***********************\
     * Component Interface *
    \***********************/
    SimpleVertex(Basics& basics, IComponentData& data);
    void define(IComponentDefiner& definer);
    void publish(ResourcePublisher& publisher);

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
    
    /***********************\
     * Scripting Interface *
    \***********************/
    void setX(double x);
    void setY(double y);
    void setZ(double z);

    private:
    
    // Definition data.
    float cDefX = 0.0; /// Initial X value.
    float cDefY = 0.0; /// Initial Y value.
    float cDefZ = 0.0; /// Initial Z value.

    // Runtime data.
    double cRuntimeX = 0.0; /// Current X value.
    double cRuntimeY = 0.0; /// Current Y value.
    double cRuntimeZ = 0.0; /// Current Z value.

    // Scripting Interface.
    LuaBinding<SimpleVertex> cLuaBinding;

    // Misc.
    IStateNotifier* cStateNotifier = nullptr;
  };
}
