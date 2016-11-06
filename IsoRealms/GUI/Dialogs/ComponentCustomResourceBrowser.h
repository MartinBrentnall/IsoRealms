
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
#ifndef COMPONENT_CUSTOM_RESOURCE_BROWSER_H
#define COMPONENT_CUSTOM_RESOURCE_BROWSER_H

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/GUI/EdgeRelation.h>
#include <IsoRealms/GUI/IResourceBrowser.h>
#include <IsoRealms/GUI/RectangularComponent.h>
#include <IsoRealms/GUI/WrappingGridComponent.h>
#include <IsoRealms/Resources/IResourceManager.h>
#include <IsoRealms/MultipleClickDetector.h>

#include "ICustomResourceManager.h"

template <class TYPE, class ICON> class ComponentCustomResourceBrowser:public RectangularComponent,
                                                                       public IResourceListener<TYPE>,
                                                                       public IResourceBrowser<TYPE> {
  private:
  
  class CommandCreateResource:public ICommand {
    private:
    ComponentCustomResourceBrowser* cParent;
    IResourceType<TYPE>* cType;
    
    public:
    CommandCreateResource(ComponentCustomResourceBrowser* parent, IResourceType<TYPE>* type) {
      cParent = parent;
      cType = type;
    }

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute() {
      cType->createResource(cParent->cEditorResources, cParent->cResourceRegistry, cParent->cEditingContext);
    }
  };
  
  class CommandEditResource:public ICommand {
    private:
    ComponentCustomResourceBrowser* cParent;
    
    public:
    CommandEditResource(ComponentCustomResourceBrowser* parent) {
      cParent = parent;
    }

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute() {
      if (cParent->cSelected != NULL) {
        cParent->cSelected->editResource();
      }
    }    
  };
  
  class CommandRemoveResource:public ICommand {
    private:
    ComponentCustomResourceBrowser* cParent;
    
    public:
    CommandRemoveResource(ComponentCustomResourceBrowser* parent) {
      cParent = parent;
    }

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute() {
      if (cParent->cSelected != NULL) {
        cParent->cSelected->removeResource();
      }
    }
  };
  
  IEditingContext* cEditingContext;
  std::map<TYPE*, Icon<TYPE>*> cResourceIcons;
  ICustomResourceManager* cProjectResources;
  IResourceAccessor* cEditorResources; // TODO: Need to set this!
  IResourceRegistry* cResourceRegistry;
  Icon<TYPE>* cSelected;
  std::vector<IResourceSelectionListener<TYPE>*> cListeners;
  
  public:
  ComponentCustomResourceBrowser(IResourceAccessor* resources, IEditingContext* editingContext, ICustomResourceManager* projectResources, float padding) : RectangularComponent("IsoRealms/GUI/Dialogs/ComponentCustomResourceBrowser", resources) {
    cEditingContext = editingContext;
    cEditorResources = resources;
    cProjectResources = projectResources;
//    projectManager->addProjectListener(this);
//    cProjectResources->addResourceListener(this);
    cSelected = NULL;
    setComponentAction("editResource", new CommandEditResource(this));
    setComponentAction("removeResource", new CommandRemoveResource(this));
    setComponentPadding("icons", padding);
  }

  void addResourceSelectionListener(IResourceSelectionListener<TYPE>* listener) {
    cListeners.push_back(listener);
  }

  void removeResourceSelectionListener(IResourceSelectionListener<TYPE>* listener) {
    for (unsigned int i = 0; i < cListeners.size(); i++) {
      if (cListeners[i] == listener) {
        cListeners.erase(cListeners.begin() + i);
        return;
      }
    }
  }

  void setSelected(Icon<TYPE>* icon) {
    if (cSelected != NULL && cSelected != icon) {
      cSelected->setSelected(false);
    }
    cSelected = icon;
    for (unsigned int i = 0; i < cListeners.size(); i++) {
      cListeners[i]->resourceSelected(icon->getResource());
    }
  }
  
  /***********************************\
   * Implements IResourceListener<*> *
  \***********************************/
  void resourceAdded(TYPE* resource) {
    Icon<TYPE>* mIcon = new ICON(this, resource);
    cResourceIcons[resource] = mIcon;
    addComponent("icons", mIcon);
  }

  void resourceDeleted(TYPE* resource) {
    Icon<TYPE>* mIcon = cResourceIcons[resource];
    removeComponent(mIcon);
    if (mIcon == cSelected) {
      cSelected = NULL;
    }
    delete mIcon;
    cResourceIcons.erase(resource);
  }

  void resourceTypeAdded(IResourceType<TYPE>* resourceType, const std::string& type) {
    addComponentAction("createResource", type, new CommandCreateResource(this, resourceType));
  }

  void editResource(TYPE* resource) {
//    cProjectResources->editResource(resource, cEditorResources, cEditingContext);
  }
  
  void removeResource(TYPE* resource) {
//    cProjectResources->removeResource(resource, cEditorResources);
  }
  
  void initialise(std::vector<TYPE*> resources) {
    for (unsigned int i = 0; i < resources.size(); i++) {
      resourceAdded(resources[i]);
    }
  }
  
  std::string getResourceLocation(TYPE* resource) {
    return cProjectResources->getPath(resource);
  }
};

#endif
