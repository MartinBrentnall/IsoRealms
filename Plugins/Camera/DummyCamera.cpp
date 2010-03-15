#include "DummyCamera.h"

float DummyCamera::getAngle() {
  return 0.0f;
}

float DummyCamera::getTilt() {
  return 0.0f;
}

extern "C" IPlugin* create() {
  return new DummyCamera();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
