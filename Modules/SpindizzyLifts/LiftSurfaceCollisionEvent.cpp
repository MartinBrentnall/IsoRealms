#include "LiftSurfaceCollisionEvent.h"

LiftSurfaceCollisionEvent::LiftSurfaceCollisionEvent(IRollableSurface* surface, ICollisionData::CollisionType type, Vertex* location, float gradient) {
  cSurface = surface;
  cType = type;
  cLocation = location;
  cGradient = gradient;
}

IRollableSurface* LiftSurfaceCollisionEvent::getSurface() {
  return cSurface;
}

ICollidableWallSurface* LiftSurfaceCollisionEvent::getWallSurface() {
  return NULL;
}

ICollisionData::CollisionType LiftSurfaceCollisionEvent::getType() {
  return cType;  
}

Vertex* LiftSurfaceCollisionEvent::getEventLocation() {
  return cLocation;
}

float LiftSurfaceCollisionEvent::getXSlope() {
  return 0.0f;
}

float LiftSurfaceCollisionEvent::getYSlope() {
  return 0.0f;
}

float LiftSurfaceCollisionEvent::getGradient() {
  return cGradient;
}




