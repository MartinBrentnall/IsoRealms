#include "ColourIcon.h"

ColourIcon::ColourIcon(IResourceBrowser<IColour>* browser, IColour* colour) : Icon<IColour>(browser, colour) {
}

void ColourIcon::renderIcon() {
  IColour* mColour = getResource();
  mColour->set();
  float mLeft   = getLeft();
  float mRight  = getRight();
  float mTop    = getTop();
  float mBottom = getBottom();
  glBegin(GL_QUADS);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glVertex2f(mLeft,  mTop);
  glEnd();
}

float ColourIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return 0.1f * mAspectRatio;
}

float ColourIcon::getHeight() {
  return 0.1f;
}
