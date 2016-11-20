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
#ifndef RESOURCE_TYPE_SET_H
#define RESOURCE_TYPE_SET_H

#include <string>

#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>
#include <IsoRealms/Resources/IResourceRegistry.h>

#include "IDummyModule.h"
#include "IResourceType.h"
#include "IResourceSelector.h"

template <class TYPE, class RESOURCE, class DIALOG, class MODULE = IDummyModule> class ResourceTypeSet : public IResourceType<TYPE> {
  private: 
  MODULE* cModuleInterface;
  std::vector<RESOURCE*> cResources;
  
  // template <class RESOURCE, class DIALOG>
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
      
    void dialogConfirmed(IRectangularComponent* component) {
      RESOURCE* mNewResource = cDialog->getResource();
      std::string mNewResourceName = cDialog->getResourceName();
      std::vector<TYPE*> mResources = mNewResource->getResources();
      for (unsigned int j = 0; j < mResources.size(); j++) {
        cResourceRegistry->add(mResources[j], mNewResourceName);
      }
      cResources->push_back(mNewResource);
    }

    void dialogCancelled(IRectangularComponent* component) {
      RESOURCE* mNewResource = cDialog->getResource();
      delete mNewResource;
    }
  };
  
  public:
  ResourceTypeSet(MODULE* moduleInterface = NULL) {
    cModuleInterface = moduleInterface;
  }
    
  void loadResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceRegistry* resourceRegistry) {
    RESOURCE* mResourceSet = new RESOURCE(cModuleInterface, node, resourceRegistry);
    cResources.push_back(mResourceSet);
    std::string mResourceSetName = node->getAttribute("name");
    std::vector<std::string> mResourceNames = mResourceSet->getResourceNames();
    for (unsigned int i = 0; i < mResourceNames.size(); i++) {
      TYPE* mResource = mResourceSet->getResource(mResourceNames[i]);
      resourceRegistry->add(mResource, mResourceSetName + "_" + mResourceNames[i]);
    }
    resourceRegistry->add(mResourceSet, node);
  }

  void saveResources(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator, const std::string& tag) {
    for (unsigned int i = 0; i < cResources.size(); i++) {
      DOMNodeWriter* mResourceNode = node->addBranch(tag);
      cResources[i]->save(mResourceNode, cache, resourceLocator);
    }
  }

  void createResource(IResourceAccessor* resources, IResourceRegistry* resourceRegistry, IEditingContext* editingContext) {
    RESOURCE* mResource = new RESOURCE(cModuleInterface, NULL, resourceRegistry);
    DIALOG* mDialog = new DIALOG(editingContext, mResource, resources, "");
    ResourceDialogConfirmationListener* mListener = new ResourceDialogConfirmationListener(mDialog, resourceRegistry, &cResources);
    mDialog->addConfirmationListener(mListener);
    IComponentContainer* mComponentContainer = editingContext->getComponentContainer();
    mComponentContainer->addComponent(mDialog);
  }

  void editResource(TYPE* resource, IResourceAccessor* resources, IEditingContext* editingContext, const std::string& name) {
    for (unsigned int i = 0; i < cResources.size(); i++) {
      if (cResources[i]->contains(resource)) {
        std::string mSetName = cResources[i]->stripMemberName(name);
        DIALOG* mDialog = new DIALOG(editingContext, cResources[i], resources, mSetName);
        IComponentContainer* mComponentContainer = editingContext->getComponentContainer();
        mComponentContainer->addComponent(mDialog);
        return;
      }
    }
  }

  void removeResource(TYPE* resource, IResourceAccessor* resources, IResourceInstanceListener<TYPE>* listener) {
    for (unsigned int i = 0; i < cResources.size(); i++) {
      if (cResources[i]->contains(resource)) {
        listener->resourcePendingDestruction(resource);
        delete cResources[i];
        cResources.erase(cResources.begin() + i);
        return;
      }
    }
  }
};

#endif
