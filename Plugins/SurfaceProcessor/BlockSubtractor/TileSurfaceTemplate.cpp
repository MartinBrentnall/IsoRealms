#include "TileSurfaceTemplate.h"

TileSurfaceTemplate::TileSurfaceTemplate(int north, int east, int south, int west) {
  cNorth = north;
  cEast = east;
  cSouth = south;
  cWest = west;
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

bool TileSurfaceTemplate::alligned(int x, int y) {
  return y >= cSouth && y <= cNorth && x >= cWest && x <= cEast;
}

