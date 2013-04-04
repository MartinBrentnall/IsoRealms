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
#ifndef SPINDIZZY_GERALD_SET_H
#define SPINDIZZY_GERALD_SET_H

#include <vector>

#include <IsoRealms/IScript.h>
#include <IsoRealms/Resources/Collectables/ICollectables.h>
#include <IsoRealms/Resources/Integer/Integer.h>

#include "ISpindizzyJewelSet.h"
#include "SpindizzyJewelType.h"

class SpindizzyJewelSet:public ISpindizzyJewelSet {
  private:
  std::vector<SpindizzyJewelType*> cElementTypes;
  bool cEditing;
  Integer* cCollectedCount;
 
  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void save(DOMNodeWriter*, IResourceLocator*);

  /**********************\
   * ISpindizzyJewelSet *
  \**********************/
  bool isEditing();
  void itemCollected();

  ~SpindizzyJewelSet();  
};

#endif
