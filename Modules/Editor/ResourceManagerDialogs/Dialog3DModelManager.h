/*
 * Copyright 2015 Martin Brentnall
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
#ifndef DIALOG_3D_MODEL_MANAGER_H
#define DIALOG_3D_MODEL_MANAGER_H

#include <IsoRealms/GUI/Dialog.h>
#include <IsoRealms/GUI/Dialogs/ComponentColourSelector.h>
#include <IsoRealms/Resources/IResourceSelectionListener.h>
#include <IsoRealms/Resources/IResourceType.h>

#include "ComponentResourceBrowser.h"
#include "ModelIcon.h"

class Dialog3DModelManager:public Dialog,
                           public IProjectManagerListener {
  private:
  ComponentResourceBrowser<I3DModelType, ModelIcon>* c3DModelBrowser;
    
  public:
  Dialog3DModelManager(IComponentContainer*, IResourceAccessor*, IResourceManager*, IProjectManager*, IResourceLocator*, IEditingContext*);
  
  /******************************\
   * Implements IProjectManager *
  \******************************/
  void projectOpened(IProject* project);
};

#endif
