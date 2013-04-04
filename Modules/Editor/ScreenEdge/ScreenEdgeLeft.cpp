#include "ScreenEdgeLeft.h"

float ScreenEdgeLeft::getTabX(DockedDialog* dockedDialog) {
  float mX = 0.0f;
  if (dockedDialog == cExpandedDialog) {
    mX += cExpandedDialog->getWidth();
  }
  return mX;
}

void ScreenEdgeLeft::renderTab(DockedDialog* dockedDialog, float x, float y) {
  // TODO: Implement this
}
