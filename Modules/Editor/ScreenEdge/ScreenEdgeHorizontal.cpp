#include "ScreenEdgeHorizontal.h"

float ScreenEdgeHorizontal::getTabWidth(DockedDialog* dockedDialog) {
  return TAB_SIZE;
}

float ScreenEdgeHorizontal::getTabHeight(DockedDialog* dockedDialog) {
  return TAB_WIDTH;
}

float ScreenEdgeHorizontal::getTabX(DockedDialog* dockedDialog) {
  float mX = 0.0f;
  for (unsigned int i = 0; i < cDockedDialogs.size(); i++) {
    if (dockedDialog == cDockedDialogs[i]) {
      return mX;
    }
    mX += getTabWidth(cDockedDialogs[i]);
  }
  return mX;
}  