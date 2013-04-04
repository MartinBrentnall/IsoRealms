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
#ifndef USER_DEFINABLE_PALETTE_H
#define USER_DEFINABLE_PALETTE_H

#include <map>
#include <vector>

#include <IsoRealms/DefaultCommandInfo.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/IComponentSource.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/OpenDialogCommand.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/Colour/IColour.h>
#include <IsoRealms/Resources/IResourceType.h>
#include <IsoRealms/Resources/IResourceAccessor.h>
#include <IsoRealms/Resources/IResourceManager.h>
#include <IsoRealms/SingleResourceTypeModule.h>

#include "DialogColourSelector.h"

class UserDefinablePalette:public IResourceType<IColour>,
			   public IPlugin,
			   public IConfirmationListener {
  private:
  IRuntimeContext* cRuntimeContext;
  std::vector<Colour*> cResources;
  std::vector<DialogColourSelector*> cCreateDialogs;
  IComponentContainer* cComponentContainer;

  public:

  /**********************\
   * Implements IPlugin *
  \**********************/
  void setEditingContext(IEditingContext*, IResourceManager*);
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  void save(DOMNodeWriter*, IResourceLocator*);

  /**********************************\
   * Implements IColourResourceType *
  \**********************************/
  void createResource(IResourceAccessor*);
  void editResource(IColour*, IResourceAccessor*);
  void removeResource(IColour*, IResourceAccessor*, IResourceInstanceListener<IColour>*);
  
  /************************************\
   * Implements IConfirmationListener *
  \************************************/
  void dialogConfirmed(IRectangularComponent*);
  void dialogCancelled(IRectangularComponent*);
};

#endif
