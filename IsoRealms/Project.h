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

#include <IsoRealms/LuaSupport/ArgumentValue.h>

#include "Configuration.h"
#include "Persistence/DOMNodeWrapper.h"
#include "ICommand.h"
#include "Layer.h"
#include "Input/InputCommands.h"
#include "IProject.h"
#include "ModuleRegistry.h"
#include "Resources/IResources.h"
#include "Resources/Resources.h"
#include "Resources/Script/IScript.h"

class Project:public IProject {
  private:
  std::vector<ILayer*> cLayers;
  ILayer* cDefaultLayer;
  ModuleRegistry cModuleRegistry;
  Resources cResources;
  IScriptCall* cInitScript;
  bool cCompleted;
  
  std::vector<IDynamicElement*> cDynamicElements;
  
  public:
  Project();
  Project(DOMNodeWrapper*, const std::string&, IEditingContext*, bool, DOMNodeWrapper*);

  void initEditor();
  void initRuntime();
  void inputRuntime(SDL_Event&);
  void renderRuntime();
  void updateRuntime(unsigned int);
  ILayer* getDefaultLayer();
  void finish();
  bool hasCompleted();
  void reset();
  
  void save();

  void removeElement(IElement*);
  ModuleRegistry* getModuleRegistry();
  IResourceManager* getResourceManager();
  void staticChanged();

  float getAspectRatio();
};

#endif
