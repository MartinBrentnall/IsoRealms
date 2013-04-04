#ifndef COMPONENT_CUSTOM_TYPE_RESOURCES_H
#define COMPONENT_CUSTOM_TYPE_RESOURCES_H

#include <IsoRealms/IComponentCustomType.h>
#include <IsoRealms/Resources/IResourceManager.h>

#include "IProjectManager.h"

template <class T> class ComponentCustomTypeResources:public IComponentCustomType,
                                                      public IProjectManagerListener {
  private:
  IResourceManager* cProjectResources;
  IProjectManager* cProjectManager;
  
  public:
  ComponentCustomTypeResources(IResourceManager* projectResources, IProjectManager* projectManager) {
    cProjectResources = projectResources;
    cProjectManager = projectManager;
    cProjectManager->addProjectListener(this);
  }
    
  AbstractRectangularComponent* createComponent(DOMNodeWrapper* node, IResourceAccessor* resources) {
    return new T(node, resources, cProjectResources, cProjectManager);
  }
  
  void projectOpened(IProject* project) {
    cProjectResources = project->getResourceManager();
  }
};

#endif
