#include "SurfaceCollisionEvent.h"

SurfaceCollisionEvent::SurfaceCollisionEvent(IRollableSurface* surface, ICollisionData::CollisionType type, Vertex* location, float gradient) {
  cSurface = surface;
  cType = type;
  cLocation = location;
  cGradient = gradient;
}

IRollableSurface* SurfaceCollisionEvent::getSurface() {
  return cSurface;
}

ICollisionData::CollisionType SurfaceCollisionEvent::getType() {
  return cType;  
}

Vertex* SurfaceCollisionEvent::getEventLocation() {
  return cLocation;
}

float SurfaceCollisionEvent::getGradient() {
  return cGradient;
}




