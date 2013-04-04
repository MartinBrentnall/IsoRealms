#include "ScreenEdgeTop.h"

float ScreenEdgeTop::getTabY(DockedDialog* dockedDialog) {
  float mY = 0.0f;
  if (dockedDialog == cExpandedDialog) {
    mY += dockedDialog->getHeight();
  }
  return mY;
}

void ScreenEdgeTop::renderTab(DockedDialog* dockedDialog, float x, float y) {
  // TODO: Implement this
}

