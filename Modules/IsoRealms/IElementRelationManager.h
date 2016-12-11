/*
 * Copyright 2016 Martin Brentnall
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
#ifndef I_ELEMENT_RELATION_MANAGER_H
#define I_ELEMENT_RELATION_MANAGER_H

#include <string>

#include <IsoRealms/IUniverse.h>

#include "IHUDComponentRelation.h"

class HUDComponentPosition;

class IElementRelationManager {
  public:
  virtual IHUDComponentRelation* getRelation(IUniverse*, const std::string&, const std::string&) = 0;
  virtual void addRelatableElement(IUniverse*, const std::string&, HUDComponentPosition*) = 0;
  virtual std::string getRelatableElementName(HUDComponentPosition*) = 0;
};

#endif

