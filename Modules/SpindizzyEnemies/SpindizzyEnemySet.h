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
#ifndef SPINDIZZY_ENEMY_SET_H
#define SPINDIZZY_ENEMY_SET_H

#include <vector>

#include <IsoRealms/IZoneContextListener.h>
#include <IsoRealms/LuaSupport/ArgumentSourceCustom.h>
#include <IsoRealms/PluginRegistry.h>

#include "ISpindizzyEnemySet.h"
#include "SpindizzyEnemyHandler.h"
#include "SpindizzyEnemyType.h"

class SpindizzyEnemySet:public ISpindizzyEnemySet {
  private:
  std::map<IElementContainer*, std::vector<SpindizzyEnemy*> > cEnemiesByContainer;
  std::vector<SpindizzyEnemyType*> cElementTypes;

  IZone* cZone;
  unsigned int cLocks;

  public:
  SpindizzyEnemySet();
  
  void addLock();
  void removeLock();
  void reset(IZone*);  

  /**************************\
   * Implements IElementSet *
  \**************************/
  void save(DOMNodeWriter*, IResourceLocator*);
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void registerElement(IElementContainer*, SpindizzyEnemy*);

  /*************************\
   * Implements ElementSet *
  \*************************/
  DefaultElementHandler<SpindizzyEnemy>* createHandler();

  /*********************************\
   * Implements ISpindizzyEnemySet *
  \*********************************/
  IZone* getCurrentZone();
  bool isLocked();
};

#endif
