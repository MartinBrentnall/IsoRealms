#ifndef TILE_SURFACE_TEMPLATE_H
#define TILE_SURFACE_TEMPLATE_H

#include "../ITileSurfaceTemplate.h"

class TileSurfaceTemplate:public ITileSurfaceTemplate {
  private:
  int cNorth;
  int cSouth;
  int cEast;
  int cWest;  

  public:
  TileSurfaceTemplate(int, int, int, int);

  int getNorth();
  int getEast();
  int getSouth();
  int getWest();
  bool alligned(int, int);
};

#endif
