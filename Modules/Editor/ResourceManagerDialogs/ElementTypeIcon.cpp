#include "ElementTypeIcon.h"

ElementTypeIcon::ElementTypeIcon(IResourceBrowser<IElementType>* browser, IElementType* elementType) : Icon<IElementType>(browser, elementType) {
  cElementType = elementType;
}

void ElementTypeIcon::renderIcon() {
  glPushMatrix();
  float mScale = 0.06f;
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mXLocation = getLeft() + (getRight() - getLeft()) / 2.0f;
  float mYLocation = getBottom() + (getTop() - getBottom()) / 2.0f;
  glTranslatef(mXLocation, mYLocation, 0.0f);
  float mAspectRatio = mScreen->getAspectRatio();
  glScalef(mAspectRatio * mScale, mScale, mScale);
  glColor3f(1.0f, 1.0f, 1.0f);

  cElementType->renderIcon();
  glPopMatrix();
}

void ElementTypeIcon::updateIcon(unsigned int milliseconds) {
  cElementType->updateIcon(milliseconds);
}

float ElementTypeIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float ElementTypeIcon::getHeight() {
  return 0.1f;
}
