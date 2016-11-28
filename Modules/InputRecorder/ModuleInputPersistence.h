/*
 * Copyright 2016 Martin Brentnall
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
#ifndef MODULE_INPUT_PERSISTENCE_H
#define MODULE_INPUT_PERSISTENCE_H

#include <iostream>

#include <IsoRealms/IModule.h>
#include <IsoRealms/Project.h>

#include "IModuleInputPersistence.h"
#include "ResourceLayerInputPlayer.h"
#include "ResourceLayerInputRecorder.h"

class ModuleInputPersistence:public IModule,
                             public IModuleInputPersistence,
                             public IResource {
  private:
  std::string cRecordingFile;
  Project* cProject;
  ResourceLayerInputPlayer cLayerTypeInputPlayer;
  ResourceLayerInputRecorder cLayerTypeInputRecorder;
  IScriptCall* cScriptQuit;
    
  public:
  ModuleInputPersistence(IResourceTypeRegistry*);

  /************************\
   * Implements IResource *
  \************************/
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);
  
  /**********************\
   * Implements IModule *
  \**********************/
  void load(DOMNodeWrapper*, DOMNodeWrapper*, IResourceRegistry*, IModuleOptions*);
  void save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*);
  void projectInitialised();
  
  /**************************************\
   * Implements IModuleInputPersistence *
  \**************************************/
  Project* getProject();
  std::ifstream* getRecording();
  void quit();
};

#endif
