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

#include <GL/glew.h>

#include <iostream>

#include "IsoRealms/Assets/Type/IVertex.h"

namespace IsoRealms {
  class LiteralVertex : public IVertex {
    public:
    double x;
    double y;
    double z;

    public:
    LiteralVertex();
    LiteralVertex(double, double, double);
    void translate();
    void set(LiteralVertex);
    void set(IVertex*);
    void set(double, double, double);
    void debug();
    
    bool isEqual(LiteralVertex*);

    /**********************\
     * Implements IVertex * 
    \**********************/   
    double getX() const override;
    double getY() const override;
    double getZ() const override;
    bool renderAssetIcon() const override;

    virtual ~LiteralVertex() {}
  };
}
