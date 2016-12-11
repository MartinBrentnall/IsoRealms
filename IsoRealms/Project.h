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
#ifndef PROJECT_H
#define PROJECT_H

#include <string>

#include <IsoRealms/LuaSupport/ArgumentValueCustomType.h>

#include "Configuration.h"
#include "Persistence/DOMNodeWrapper.h"
#include "ICommand.h"
#include "Layer.h"
#include "Input/InputCommands.h"
#include "IProject.h"
#include "IProjectOptions.h"
#include "ModuleRegistry.h"
#include "Resources/IResources.h"
#include "Resources/Resources.h"
#include "Resources/Script/IScript.h"
#include "Resources/String/String.h"

class Project:public IProject {
  private:
  class ReturnValue {
    public:
    std::string cType;
    std::string cReference;
    std::string cValue;
    ReturnValue(const std::string&, const std::string&);
  };
    
  class ReturnProject {
    public:
    std::string cReference;
    IProject*   cProject;
    ReturnProject(const std::string&);
  };
    
  std::vector<ILayer*> cLayers;
  ILayer* cDefaultLayer;
  ModuleRegistry cModuleRegistry;
  Resources cResources;
  IScriptCall* cInitScript;
  bool cCompleted;
  std::string cFileName;
  std::string cProjectDataPath;
  bool cFirstInitialised;
  std::map<std::string, ReturnValue*>   cReturnValues;
  std::map<std::string, ReturnProject*> cReturnProjects;
  
  std::vector<IDynamicElement*> cDynamicElements;
  
  std::vector<IObjectSelectionListener*> cObjectSelectionListeners;
  
  void loadProject(DOMNodeWrapper*, DOMNodeWrapper*, IEditingContext*, bool, IProjectOptions*);
  void updateCache();
  
  public:
  Project();
  Project(const std::string&, bool, IEditingContext*, bool, IProjectOptions*);

  void initEditor();
  void initRuntime();
  void inputRuntime(SDL_Event&);
  void renderRuntime();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  void initialised();
  ILayer* getDefaultLayer();
  std::vector<ILayer*> getAllLayers();
  std::string getName(ILayer*);
  void finish();
  bool hasCompleted();
  void reset();
  void addObjectSelectionListener(IObjectSelectionListener*);
  
  void save();
  void save(const std::string&);
  bool hasFileName();

  void removeElement(IElement*);
  ModuleRegistry* getModuleRegistry();
  IResourceManager* getResourceManager();
  void staticChanged();

  float getAspectRatio();
  
  InputCommands* getInputConfiguration();
  std::string getFileName();
  
  std::string getReturnValue(const std::string&);
};

#endif
