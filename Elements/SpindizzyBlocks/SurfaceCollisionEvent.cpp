#include "SurfaceCollisionEvent.h"

SurfaceCollisionEvent::SurfaceCollisionEvent(IRollableSurface* surface) {
  cSurface = surface;
}

IRollableSurface* SurfaceCollisionEvent::getSurface() {
  return cSurface;
}

