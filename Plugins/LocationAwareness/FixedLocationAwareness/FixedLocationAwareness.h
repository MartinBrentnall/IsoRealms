#ifndef FIXED_LOCATION_AWARENESS_H
#define FIXED_LOCATION_AWARENESS_H

#include "../ILocationAwareness.h"

class FixedLocationAwareness:public ILocationAwareness {
  private:
  Vertex* cLocation;

  public:
  
  /*********************************\
   * Implements ILocationAwareness *
  \*********************************/
  void setLocation(Vertex*);
  Vertex* getLocation();

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::string getName();
};

#endif
