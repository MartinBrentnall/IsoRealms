#ifndef I_COLOUR_SOURCE_H
#define I_COLOUR_SOURCE_H

#include "DOMNodeWrapper.h"
#include "IColour.h"

class IColourSource {
  public:
  virtual IColour* getColour(DOMNodeWrapper*) = 0;
};

#endif
