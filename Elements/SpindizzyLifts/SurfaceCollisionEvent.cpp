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

ICollidableWallSurface* SurfaceCollisionEvent::getWallSurface() {
  return NULL;
}

ICollisionData::CollisionType SurfaceCollisionEvent::getType() {
  return cType;  
}

Vertex* SurfaceCollisionEvent::getEventLocation() {
  return cLocation;
}

float SurfaceCollisionEvent::getXSlope() {
  return 0.0f;
}

float SurfaceCollisionEvent::getYSlope() {
  return 0.0f;
}

float SurfaceCollisionEvent::getGradient() {
  return cGradient;
}




