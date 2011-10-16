#include "SurfaceCollisionEvent.h"

SurfaceCollisionEvent::SurfaceCollisionEvent(IRollableSurface* surface, ICollisionData::CollisionType type, Vertex* location, float xSlope, float ySlope, float gradient) {
  cSurface = surface;
  cType = type;
  cLocation = location;
  cXSlope = xSlope;
  cYSlope = ySlope;
  cGradient = gradient;
}

SurfaceCollisionEvent::SurfaceCollisionEvent(ICollidableWallSurface* surface, ICollisionData::CollisionType type, Vertex* location, float gradient) {
  cWallSurface = surface;
  cType = type;
  cLocation = location;
  cGradient = gradient;
}

IRollableSurface* SurfaceCollisionEvent::getSurface() {
  return cSurface;
}

ICollidableWallSurface* SurfaceCollisionEvent::getWallSurface() {
  return cWallSurface;
}

ICollisionData::CollisionType SurfaceCollisionEvent::getType() {
  return cType;  
}

Vertex* SurfaceCollisionEvent::getEventLocation() {
  return cLocation;
}

float SurfaceCollisionEvent::getXSlope() {
  return cXSlope;
}

float SurfaceCollisionEvent::getYSlope() {
  return cYSlope;
}

float SurfaceCollisionEvent::getGradient() {
  return cGradient;
}

