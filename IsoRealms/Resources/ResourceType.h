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
#ifndef RESOURCE_TYPE_H
#define RESOURCE_TYPE_H

#include <string>

#include <IsoRealms/IConfirmationListener.h>

#include "IDummyModule.h"
#include "IResourceRegistry.h"
#include "IResourceType.h"

template <class TYPE, class RESOURCE, class DIALOG, class MODULE = IDummyModule> class ResourceType : public IResourceType<TYPE> {
  private: 
  MODULE* cModuleInterface;
  std::vector<RESOURCE*> cResources;

  //template <class RESOURCE, class DIALOG> 
  class ResourceDialogConfirmationListener : public IConfirmationListener {
    private:
    DIALOG* cDialog;
    IResourceRegistry* cResourceRegistry;
    std::vector<RESOURCE*>* cResources;
      
    public:
    ResourceDialogConfirmationListener(DIALOG* dialog, IResourceRegistry* resourceRegistry, std::vector<RESOURCE*>* resources) {
      cDialog = dialog;
      cResourceRegistry = resourceRegistry;
      cResources = resources;
    }
      
    void dialogConfirmed(IRectangularComponent* dialog) {
      RESOURCE* mNewResource = cDialog->getResource();
      std::string mNewResourceName = cDialog->getResourceName();
      cResourceRegistry->add(mNewResource, mNewResourceName);
      cResources->push_back(mNewResource);
    }

    void dialogCancelled(IRectangularComponent* dialog) {
      RESOURCE* mNewResource = cDialog->getResource();
      delete mNewResource;
    }
  };
                                                                                                      
  public:
  ResourceType(MODULE* moduleInterface = nullptr) {
    cModuleInterface = moduleInterface;
  }
    
  void loadResource(DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
    RESOURCE* mResource = new RESOURCE(cModuleInterface, node, resourceRegistry);
    cResources.push_back(mResource);
    std::string mResourceName = node->getAttribute("name");
    resourceRegistry->add(mResource, mResourceName, node);
  }

  void saveResources(DOMNodeWriter* node, IResourceLocator* resourceLocator, const std::string& tag) {
    for (unsigned int i = 0; i < cResources.size(); i++) {
      DOMNodeWriter* mResourceNode = node->addBranch(tag);
      std::string mResourceName = resourceLocator->getPath(cResources[i]);
      mResourceName = mResourceName.substr(mResourceName.find_last_of('/') + 1);
      mResourceNode->addAttribute("name", mResourceName);
      cResources[i]->save(mResourceNode, resourceLocator);
    }
  }

  void createResource(IResourceAccessor* resources, IResourceRegistry* resourceRegistry, IEditingContext* editingContext) {
    RESOURCE* mResource = new RESOURCE(cModuleInterface, nullptr, resourceRegistry);
    IComponentContainer* mComponentContainer = editingContext->getComponentContainer();
    DIALOG* mDialog = new DIALOG(editingContext, mResource, resources, "");
    ResourceDialogConfirmationListener* mListener = new ResourceDialogConfirmationListener(mDialog, resourceRegistry, &cResources);
    mDialog->addConfirmationListener(mListener);
    mComponentContainer->addComponent(mDialog);
  }

  void editResource(TYPE* resource, IResourceAccessor* resources, IEditingContext* editingContext, const std::string& name) {
    for (unsigned int i = 0; i < cResources.size(); i++) {
      if (cResources[i] == resource) {
        IComponentContainer* mComponentContainer = editingContext->getComponentContainer();
        DIALOG* mDialog = new DIALOG(editingContext, cResources[i], resources, name);
        mComponentContainer->addComponent(mDialog);
        return;
      }
    }
  }

  void removeResource(TYPE* resource, IResourceAccessor* resources, IResourceInstanceListener<TYPE>* listener) {
    for (unsigned int i = 0; i < cResources.size(); i++) {
      if (cResources[i] == resource) {
        listener->resourcePendingDestruction(resource);
        delete cResources[i];
        cResources.erase(cResources.begin() + i);
        return;
      }
    }
  }
};

#endif