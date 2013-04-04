#ifndef COMPONENT_RESOURCE_BROWSER_H
#define COMPONENT_RESOURCE_BROWSER_H

#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/GUI/EdgeRelation.h>
#include <IsoRealms/GUI/RectangularComponent.h>
#include <IsoRealms/GUI/WrappingGridComponent.h>
#include <IsoRealms/Resources/IResourceManager.h>
#include <IsoRealms/MultipleClickDetector.h>

#include "../IProjectManager.h"

#include "IResourceBrowser.h"

template <class T, class T2> class ComponentResourceBrowser:public RectangularComponent,
                                                            public IProjectManagerListener,
                                                            public IResourceListener<T>,
                                                            public IResourceBrowser<T> {
  private:
  
  class CommandCreateResource:public ICommand {
    private:
    ComponentResourceBrowser* cParent;
    IResourceType<T>* cType;
    
    public:
    CommandCreateResource(ComponentResourceBrowser* parent, IResourceType<T>* type) {
      cParent = parent;
      cType = type;
    }

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute() {
      cType->createResource(cParent->cEditorResources);
    }
  };
  
  class CommandEditResource:public ICommand {
    private:
    ComponentResourceBrowser* cParent;
    
    public:
    CommandEditResource(ComponentResourceBrowser* parent) {
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
    ComponentResourceBrowser* cParent;
    
    public:
    CommandRemoveResource(ComponentResourceBrowser* parent) {
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
  
  std::map<T*, Icon<T>*> cResourceIcons;
  IResourceManager* cProjectResources;
  IResourceAccessor* cEditorResources;
  Icon<T>* cSelected;
  std::vector<IResourceSelectionListener<T>*> cListeners;
  
  public:
  ComponentResourceBrowser(IResourceAccessor* resources, IResourceManager* projectResources, IProjectManager* projectManager, IResourceLocator* resourceLocator, float padding) : RectangularComponent("Modules/Editor/ResourceManagerDialogs/ComponentResourceBrowser", resources) {
    cEditorResources = resources;
    cProjectResources = projectResources;
    projectManager->addProjectListener(this);
    cProjectResources->addResourceListener(this);
    cSelected = NULL;
    setComponentAction("editResource", new CommandEditResource(this));
    setComponentAction("removeResource", new CommandRemoveResource(this));
    setComponentPadding("icons", padding);
  }

  void addResourceSelectionListener(IResourceSelectionListener<T>* listener) {
    cListeners.push_back(listener);
  }

  void removeResourceSelectionListener(IResourceSelectionListener<T>* listener) {
    for (unsigned int i = 0; i < cListeners.size(); i++) {
      if (cListeners[i] == listener) {
	cListeners.erase(cListeners.begin() + i);
	return;
      }
    }
  }

  void setSelected(Icon<T>* icon) {
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
  void resourceAdded(T* resource) {
    Icon<T>* mIcon = new T2(this, resource);
    cResourceIcons[resource] = mIcon;
    addComponent("icons", mIcon);
  }

  void resourceDeleted(T* resource) {
    Icon<T>* mIcon = cResourceIcons[resource];
    removeComponent(mIcon);
    if (mIcon == cSelected) {
      cSelected = NULL;
    }
    delete mIcon;
    cResourceIcons.erase(resource);
  }

  void resourceTypeAdded(IResourceType<T>* resourceType, const std::string& type) {
    addComponentAction("createResource", type, new CommandCreateResource(this, resourceType));
  }

  /**************************************\
   * Implements IProjectManagerListener *
  \**************************************/
  void projectOpened(IProject* project) {
    cProjectResources = project->getResourceManager();
    cProjectResources->addResourceListener(this);
  }
  
  void editResource(T* resource) {
    cProjectResources->editResource(resource, cEditorResources);
  }
  
  void removeResource(T* resource) {
    cProjectResources->removeResource(resource, cEditorResources);
  }
  
  void initialise(std::vector<T*> resources) {
    for (unsigned int i = 0; i < resources.size(); i++) {
      resourceAdded(resources[i]);
    }
  }
  
  std::string getResourceLocation(T* resource) {
    return cProjectResources->getPath(resource);
  }
};

#endif
