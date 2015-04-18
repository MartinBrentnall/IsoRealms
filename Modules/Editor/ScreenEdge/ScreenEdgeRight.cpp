/*
 * Copyright 2015 Martin Brentnall
 *
 * This file is part of Iso-Realms.
 *
 * Iso-Realms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Iso-Realms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
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
}
