/*
 * Copyright 2009 Martin Brentnall
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
#ifndef I_RECTANGULAR_COMPONENT_H
#define I_RECTANGULAR_COMPONENT_H

#include "../IHUDComponent.h"
#include "../IRectangle.h"

/**
 * This interface allows a component to define its own rectangle.  This is
 * useful when we need to arrange multiple components in a space.
 */
class IRectangularComponent:public IHUDComponent,
                            public IRectangle {
  public:
  virtual ~IRectangularComponent() {}

  enum Edge {
    TOP,
    LEFT,
    BOTTOM,
    RIGHT
  };
};

#endif
