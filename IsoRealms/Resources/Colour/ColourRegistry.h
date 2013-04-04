#ifndef COLOUR_REGISTRY_H
#define COLOUR_REGISTRY_H

#include <IsoRealms/Resources/ResourceRegistry.h>

#include "Colour.h"

class ColourRegistry:public ResourceRegistry<IColour> {
  private:
  std::vector<Colour*> cLiteralColours;
  
  public:
  IColour* getSpecialResource(const std::string&);
  std::string getSpecialLocation(IColour* resource);
};

#endif
