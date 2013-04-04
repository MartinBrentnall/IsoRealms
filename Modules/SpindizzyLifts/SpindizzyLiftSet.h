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
#include <IsoRealms/IZoneContextListener.h>
#include <IsoRealms/OpenDialogCommand.h>
#include <IsoRealms/Resources/SurfaceRegistry/ICollidableSurfaceRegistry.h>

#include "ISpindizzyLiftSet.h"
#include "SpindizzyLiftHandler.h"
#include "SpindizzyLiftProperties.h"
#include "SpindizzyLiftType.h"

class SpindizzyLiftSet:public ISpindizzyLiftSet,
                       public IResource {
  private:
  std::map<IElementContainer*, std::vector<SpindizzyLift*> > cLiftsByContainer;
  std::vector<SpindizzyLiftType*> cElementTypes;

  bool cEditing;
  std::vector<IElementType*> cElementFactories;
  IComponentContainer* cComponentContainer;
  ICollidableSurfaceRegistry* cCollidableSurfaceRegistry;
  IScript* cLiftMovedScript;
  IZone* cZone;

  /**
   * Properties are applied to all lift types.
   */
  SpindizzyLiftProperties cSpindizzyLiftProperties;
  
  unsigned int cLocked;

  public:
  void addLock();
  void removeLock();
  void reset(IZone*);  
  
  /**************************\
   * Implements IElementSet *
  \**************************/
  void createResources(DOMNodeWrapper*, IRuntimeContext*);

  /*************************\
   * Implements ElementSet *
  \*************************/
  DefaultElementHandler<SpindizzyLift>* createHandler();
  
  /**********************************************\
   * Implements IPluginSupport (in IElementSet) *
  \**********************************************/
  void save(DOMNodeWriter*, IResourceLocator*);
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

  /********************************\
   * Implements ISpindizzyLiftSet *
  \********************************/
  IZone* getCurrentZone();
  void executeLiftMovedScript();
  void registerInterceptingSurface(IRollableSurface*);
  bool isLocked();
  bool isEditing();
  void registerElement(IElementContainer*, SpindizzyLift*);
};

#endif
