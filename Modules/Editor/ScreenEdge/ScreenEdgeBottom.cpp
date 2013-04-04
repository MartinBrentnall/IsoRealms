#include "ScreenEdgeBottom.h"

float ScreenEdgeBottom::getTabY(DockedDialog* dockedDialog) {
  float mY = 1.0f - getTabHeight(dockedDialog);
  if (dockedDialog == cExpandedDialog) {
    mY -= cExpandedDialog->getHeight();
  }
  return mY;
}

void ScreenEdgeBottom::renderTab(DockedDialog* dockedDialog, float x, float y) {
  // TODO: Implement this
}
