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

#include <IsoRealms/DefaultCommandInfo.h>
#include <IsoRealms/IElementSet.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/OpenDialogCommand.h>

#include "../../Plugins/CollidableSurfaceRegistry/ICollidableSurfaceRegistry.h"
#include "../../Plugins/ZoneContext/IZoneContext.h"
#include "../../Plugins/ZoneContext/IZoneContextListener.h"

#include "ISpindizzyLiftSet.h"
#include "SpindizzyLiftConfigurationDialog.h"
#include "SpindizzyLiftHandler.h"
#include "SpindizzyLiftFactory.h"
#include "SpindizzyLiftProperties.h"

class SpindizzyLiftSet:public ISpindizzyLiftSet,
                       public IZoneContextListener,
                       public IComponentSource {
  private:
  std::vector<IElementFactory*> cElementFactories;
  IComponentContainer* cComponentContainer;
  IRuntimeContext* cRuntimeContext;
  IZoneContext* cZoneContext;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  ILuaScript* cLiftMovedScript;
  IZone* cZone;

  /**
   * Properties are applied to all lift types.
   */
  SpindizzyLiftProperties cSpindizzyLiftProperties;
  
  unsigned int cLocked;

  class LockControlCommand:public ICommand {
    private:
    SpindizzyLiftSet* cParent;
    bool cLock;
    
    public:
    LockControlCommand(SpindizzyLiftSet*, bool);
    
    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  public:
  SpindizzyLiftSet(IRuntimeContext*);

  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
  void destroy(IElement*);
  void setEditingContext(IEditingContext*);

  /*************************\
   * Implements ElementSet *
  \*************************/
  DefaultElementHandler<SpindizzyLift>* createHandler(IElementContainer*);
  
  /**********************************************\
   * Implements IPluginSupport (in IElementSet) *
  \**********************************************/
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);

  /********************************\
   * Implements ISpindizzyLiftSet *
  \********************************/
  IZone* getCurrentZone();
  void executeLiftMovedScript();
  void registerInterceptingSurface(IRollableSurface*);
  bool isLocked();
  bool isEditing();
  
  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone* zone);

  /*******************************\
   * Implements IComponentSource *
  \*******************************/
  IHUDComponent* createComponent();
};

#endif
