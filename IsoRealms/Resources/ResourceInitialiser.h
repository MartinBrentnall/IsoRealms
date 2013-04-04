#ifndef RESOURCE_INITIALISER_H
#define RESOURCE_INITIALISER_H

#include <IsoRealms/Persistence/DOMNodeWrapper.h>

#include "IResource.h"
#include "IResourceAccessor.h"

class ResourceInitialiser {
  private:
  IResource* cResource;
  DOMNodeWrapper* cConfigurationNode;

  public:
  ResourceInitialiser(IResource*, DOMNodeWrapper*);
    
  void initialiseResource(IResourceAccessor*);
};

#endif
