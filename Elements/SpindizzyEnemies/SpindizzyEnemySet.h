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

#include <IsoRealms/IElementSet.h>
#include <IsoRealms/PluginRegistry.h>

#include "../../Plugins/3DModel/ISimpleModelFactory.h"
#include "../../Plugins/ZoneContext/IZoneContext.h"
#include "../../Plugins/ZoneContext/IZoneContextListener.h"

#include "ISpindizzyEnemySet.h"
#include "SpindizzyEnemyFactory.h"
#include "SpindizzyEnemyHandler.h"

class SpindizzyEnemySet:public ISpindizzyEnemySet,
                        public IZoneContextListener {
  private:
  std::vector<IElementFactory*> cElementFactories;
  std::vector<ISimpleModelFactory*> cEnemyModelFactories;
  IZoneContext* cZoneContext;
  IZone* cZone;
  unsigned int cLocks;
  IRuntimeContext* cRuntimeContext;
  std::vector<IUserCommand*> cCommands;

  void setModel(ISimpleModelFactory*);

  class LockControlCommand:public IUserCommand {
    private:
    SpindizzyEnemySet* cParent;
    bool cLock;
    
    public:
    LockControlCommand(SpindizzyEnemySet*, bool);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getName();
  };
  
  public:
  SpindizzyEnemySet();

  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
  void destroy(IElement*);

  /*************************\
   * Implements ElementSet *
  \*************************/
  void setRuntimeContext(IRuntimeContext*);
  DefaultElementHandler<SpindizzyEnemy>* createHandler(IElementContainer*);
  
  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
  void save(DOMNodeWriter*);

  /*********************************\
   * Implements ISpindizzyEnemySet *
  \*********************************/
  IZone* getCurrentZone();
  bool isLocked();
  
  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone* zone);
};

#endif
