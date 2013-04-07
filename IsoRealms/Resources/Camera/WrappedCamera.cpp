#include "WrappedCamera.h"

WrappedCamera::WrappedCamera(ICamera* camera) {
  cCamera = camera;
}

void WrappedCamera::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  // Nothing to do
}

float WrappedCamera::getAngle() {
  return cCamera->getAngle();
}

float WrappedCamera::getTilt() {
  return cCamera->getTilt();
}

void WrappedCamera::update(int milliseconds) {
  cCamera->update(milliseconds);
}

void WrappedCamera::render() {
  cCamera->render();
}


