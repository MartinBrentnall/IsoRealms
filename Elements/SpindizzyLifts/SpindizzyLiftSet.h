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

#include <IsoRealms/IElementSet.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/ICommandRegistry.h>

#include "../../Plugins/CollidableSurfaceRegistry/ICollidableSurfaceRegistry.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTexture.h"
#include "../../Plugins/SpindizzyTextureSet/ISpindizzyTextureSet.h"
#include "../../Plugins/ZoneContext/IZoneContext.h"
#include "../../Plugins/ZoneContext/IZoneContextListener.h"

#include "ISpindizzyLiftSet.h"
#include "SpindizzyLiftHandler.h"
#include "SpindizzyLiftFactory.h"
#include "SpindizzyLiftProperties.h"

class SpindizzyLiftSet:public ISpindizzyLiftSet,
                       public IZoneContextListener {
  private:
  std::vector<IElementFactory*> cElementFactories;
  ICommandRegistry* cCommandRegistry;
  IZoneContext* cZoneContext;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  Script* cLiftMovedScript;
  IZone* cZone;
  std::vector<IUserCommand*> cCommands;
  ISpindizzyTextureSet* cSpindizzyTextureSet;

  /**
   * Properties are applied to all lift types.
   */
  SpindizzyLiftProperties cSpindizzyLiftProperties;

  void setTextureSet(ISpindizzyTextureSet*);

  public:
  SpindizzyLiftSet();

  /**************************\
   * Implements IElementSet *
  \**************************/
  std::vector<IElementFactory*> getElementFactories();
  void setEditingContext(BlockLocation*, IElementGateway*, IComponentContainer*, ICommandRegistry*);
  void destroy(IElement*);

  /*************************\
   * Implements ElementSet *
  \*************************/
  DefaultElementHandler<SpindizzyLift>* createHandler(IElementContainer*);
  
  /**********************************************\
   * Implements IPluginSupport (in IElementSet) *
  \**********************************************/
  std::vector<PlugSocket*> getPlugSockets();
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
  
  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone* zone);
};

#endif
