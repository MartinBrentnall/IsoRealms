#include "ScreenEdgeVertical.h"

float ScreenEdgeVertical::getTabHeight(DockedDialog* dockedDialog) {
  return TAB_SIZE;
}

float ScreenEdgeVertical::getTabWidth(DockedDialog* dockedDialog) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  return TAB_WIDTH * mAspectRatio;
}

float ScreenEdgeVertical::getTabY(DockedDialog* dockedDialog) {
  float mY = 1.0f;
  for (unsigned int i = 0; i < cDockedDialogs.size(); i++) {
    if (dockedDialog == cDockedDialogs[i]) {
      return mY;
    }
    mY -= getTabHeight(cDockedDialogs[i]);
  }
  return mY;
}  