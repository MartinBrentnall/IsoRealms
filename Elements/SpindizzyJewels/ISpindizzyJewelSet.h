/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef I_SPINDIZZY_JEWEL_SET_H
#define I_SPINDIZZY_JEWEL_SET_H

#include <IsoRealms/ElementSet.h>
#include <IsoRealms/IElement.h>

#include "../../Plugins/Collectables/ICollectables.h"

class ISpindizzyJewelSet:public ElementSet<IElement> {
  public:
  virtual ICollectables* getCollectables() = 0;
  virtual void jewelCollected() = 0;
  virtual void allJewelsCollected() = 0;
  virtual bool isEditing() = 0;
};

#endif
