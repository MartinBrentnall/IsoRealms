#include "ComponentIconModel.h"

ComponentIconModel::ComponentIconModel(I3DModelType* modelType) {
  cModelType = modelType;
}

float ComponentIconModel::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.2f * mAspectRatio;
}

float ComponentIconModel::getHeight() {
  return 0.2f;
}

void ComponentIconModel::render() {
  glPushMatrix();
  float mScale = 0.12f; // TODO: Calculate this
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mXLocation = getLeft() + (getRight() - getLeft()) / 2.0f;
  float mYLocation = getBottom() + (getTop() - getBottom()) / 2.0f;
  glTranslatef(mXLocation, mYLocation, 0.0f);
  float mAspectRatio = mScreen->getAspectRatio();
  glScalef(mAspectRatio * mScale, mScale, mScale);
  glColor3f(1.0f, 1.0f, 1.0f);
  cModelType->renderIcon();
  glPopMatrix();
}

void ComponentIconModel::update(unsigned int milliseconds) {
  // Nothing to do
}

bool ComponentIconModel::input(SDL_Event& event) {
  return false;
}
