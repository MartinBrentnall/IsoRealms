#include "ScriptableVertex.h"

IVertex* ScriptableVertex::getVertex() {
  return &cVertex;
}

double ScriptableVertex::getX() {
  return cVertex.x;
}

double ScriptableVertex::getY() {
  return cVertex.y;
}

double ScriptableVertex::getZ() {
  return cVertex.z;
}

void ScriptableVertex::setX(double x) {
  cVertex.x = x;
}

void ScriptableVertex::setY(double y) {
  cVertex.y = y;
}

void ScriptableVertex::setZ(double z) {
  cVertex.z = z;
}
