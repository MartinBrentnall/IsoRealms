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

#include <IsoRealms/PluginRegistry.h>

#include "../../Plugins/3DModel/ISimpleModelFactory.h"
#include "../../Plugins/Camera/ICamera.h"
#include "../../Plugins/Collectables/ICollectables.h"
#include "../../Plugins/CollidableSurfaceRegistry/ICollidableSurfaceRegistry.h"
#include "../../Plugins/LocationAwareness/ILocationAwareness.h"
#include "../../Plugins/ZoneContext/IZoneContext.h"

#include "ISpindizzyGERALDSet.h"
#include "SpindizzyGERALDFactory.h"

class SpindizzyGERALDSet:public ISpindizzyGERALDSet {
  private:
  std::vector<IElementFactory*> cElementFactories;
  ISimpleModelFactory* cGERALDModelFactory;
  ICamera* cCamera;
  ICollectables* cCollectables;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  ILocationAwareness* cLocationAwareness;
  IZoneContext* cZoneContext;
  ICommandRegistry* cCommandRegistry;
  std::vector<IUserCommand*> cCommands;
  unsigned int cLocks;
  bool cEditing;

  void setModel(ISimpleModelFactory*);
  void stop();

  class LockControlCommand:public IUserCommand {
    private:
    SpindizzyGERALDSet* cParent;
    bool cLock;
    
    public:
    LockControlCommand(SpindizzyGERALDSet*, bool);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };
  
  class StopCommand:public IUserCommand {
    private:
    SpindizzyGERALDSet* cParent;
    
    public:
    StopCommand(SpindizzyGERALDSet*);
    
    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getCommandName();
  };
  
  public:
  SpindizzyGERALDSet();

  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
  void setRuntimeContext(IRuntimeContext*);
  void destroy(IElement*);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /**********************************\
   * Implements ISpindizzyGERALDSet *
  \**********************************/
  bool isEditing();
  bool isLocked();
};

#endif
