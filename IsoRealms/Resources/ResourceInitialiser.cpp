#include "ResourceInitialiser.h"

ResourceInitialiser::ResourceInitialiser(IResource* resource, DOMNodeWrapper* configurationNode) {
  cResource = resource;
  cConfigurationNode = configurationNode;
}
    
void ResourceInitialiser::initialiseResource(IResourceAccessor* resourceAccessor) {
  cResource->initialiseResource(cConfigurationNode, resourceAccessor);
}
