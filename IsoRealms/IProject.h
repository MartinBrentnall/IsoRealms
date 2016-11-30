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
#ifndef I_PROJECT_H
#define I_PROJECT_H

#include <IsoRealms/Resources/Camera/ICamera.h>
#include <IsoRealms/Resources/ElementType/IElement.h>

#include "BlockLocation.h"
#include "Persistence/DOMNodeWrapper.h"
#include "IEditingContext.h"
#include "IObjectSelectionListener.h"

class ILayer;
class IResourceManager;
class ModuleRegistry;

class IProject {
  public:

  // TODO: Editor functions should probably not be here
  virtual void removeElement(IElement*) = 0;

  virtual void updateRuntime(unsigned int) = 0;
  virtual void renderRuntime() = 0;
  virtual void inputRuntime(SDL_Event&) = 0;
  virtual void save() = 0;
  virtual void save(const std::string&) = 0;
  virtual bool hasFileName() = 0;
  virtual ModuleRegistry* getModuleRegistry() = 0;
  virtual IResourceManager* getResourceManager() = 0;
  virtual ILayer* getDefaultLayer() = 0;
  virtual std::vector<ILayer*> getAllLayers() = 0;
  virtual std::string getName(ILayer*) = 0;
  virtual void initEditor() = 0;
  virtual void initRuntime() = 0;
  virtual bool hasCompleted() = 0;
  virtual void reset() = 0;
  virtual void addObjectSelectionListener(IObjectSelectionListener*) = 0;
  
  virtual void staticChanged() = 0;
  
  virtual void initialised() = 0;
  
  virtual std::string getReturnValue(const std::string&) = 0;

  virtual ~IProject() {}
};

#endif
