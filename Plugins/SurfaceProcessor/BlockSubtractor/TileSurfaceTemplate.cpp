#include "TileSurfaceTemplate.h"

TileSurfaceTemplate::TileSurfaceTemplate(int north, int east, int south, int west, Condition* condition) {
  cNorth = north;
  cEast = east;
  cSouth = south;
  cWest = west;
  cCondition = condition;
}

int TileSurfaceTemplate::getNorth() {
  return cNorth;
}

int TileSurfaceTemplate::getEast() {
  return cEast;
}

int TileSurfaceTemplate::getSouth() {
  return cSouth;
}

int TileSurfaceTemplate::getWest() {
  return cWest;
}

Condition* TileSurfaceTemplate::getCondition() {
  return cCondition;
}

bool TileSurfaceTemplate::alligned(int x, int y) {
  return y >= cSouth && y <= cNorth && x >= cWest && x <= cEast;
}

