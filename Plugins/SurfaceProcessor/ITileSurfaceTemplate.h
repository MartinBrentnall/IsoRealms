#ifndef I_TILE_SURFACE_TEMPLATE_H
#define I_TILE_SURFACE_TEMPLATE_H

class ITileSurfaceTemplate {
  public:
  virtual int getNorth() = 0;
  virtual int getEast() = 0;
  virtual int getSouth() = 0;
  virtual int getWest() = 0;
  virtual bool alligned(int, int) = 0;
};

#endif
