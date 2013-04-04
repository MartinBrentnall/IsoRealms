#include "ScreenEdgeRight.h"

float ScreenEdgeRight::getTabX(DockedDialog* dockedDialog) {
  float mX = 1.0f - getTabWidth(dockedDialog);
  if (dockedDialog == cExpandedDialog) {
    mX = sine(mX, mX - cExpandedDialog->getWidth(), cAnimation);
  }
  for (std::map<DockedDialog*, float>::iterator i = cCollapsingDialogs.begin(); i != cCollapsingDialogs.end(); i++) {
    if (dockedDialog == i->first) {
      mX = sine(mX - cExpandedDialog->getWidth(), mX, i->second);
    }
  }
  return mX;
}

void ScreenEdgeRight::renderTab(DockedDialog* dockedDialog, float x, float y) {
  float mEndY = y - getTabHeight(dockedDialog);
  float mEndX = x + getTabWidth(dockedDialog);
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();
  glDisable(GL_DEPTH_TEST);

  glBindTexture(GL_TEXTURE_2D, 0);
  glLoadIdentity();
  
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(0.05f, 0.0f, 0.1f, 0.85f);
  glVertex2f(x,     y);
  glVertex2f(x,     mEndY);
  glVertex2f(mEndX, mEndY);
  glVertex2f(mEndX, y);    
  glEnd();
  glDisable(GL_BLEND);

  glBegin(GL_LINE_LOOP);
  glColor3f(0.8f, 0.6f, 1.0f);
  glVertex2f(x,     y);
  glVertex2f(x,     mEndY);
  glVertex2f(mEndX, mEndY);
  glVertex2f(mEndX, y);    
  glEnd();
  
  glLoadIdentity();  
  glEnable(GL_DEPTH_TEST);

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();  
}
