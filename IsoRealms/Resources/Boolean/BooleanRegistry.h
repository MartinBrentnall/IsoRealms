/*
 * Copyright 2015 Martin Brentnall
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
#ifndef BOOLEAN_REGISTRY_H
#define BOOLEAN_REGISTRY_H

#include <IsoRealms/Resources/ResourceRegistry.h>

#include "Boolean.h"

class BooleanRegistry:public ResourceRegistry<IBoolean> {
  private:
  std::vector<Boolean*> cLiteralBooleans;
  
  public:
  IBoolean* getSpecialResource(const std::string&);
  std::string getSpecialLocation(IBoolean*);
};

#endif
