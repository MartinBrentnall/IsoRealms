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

#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/Resources/Collectables/ICollectables.h>
#include <IsoRealms/Resources/SurfaceRegistry/ICollidableSurfaceRegistry.h>

#include "ISpindizzyGERALDSet.h"
#include "SpindizzyGERALDType.h"

class SpindizzyGERALDSet:public ISpindizzyGERALDSet {
  private:
  bool cEditing;
  std::vector<SpindizzyGERALDType*> cElementTypes;
  Vertex cLocation;
  std::vector<ICommand*> cCommands;
  unsigned int cLocks;

  public:
  void addLock();
  void removeLock();
  
  /**************************\
   * Implements IElementSet *
  \**************************/
  void save(DOMNodeWriter*, IResourceLocator*);
  void createResources(DOMNodeWrapper*, IRuntimeContext*);

  /**********************************\
   * Implements ISpindizzyGERALDSet *
  \**********************************/
  bool isEditing();
  bool isLocked();
};

#endif
