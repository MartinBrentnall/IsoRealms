/*
 * Copyright 2009 Martin Brentnall
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
#include "SpindizzyCraftGyroscopeModel.h"

Texture* SpindizzyCraftGyroscopeModel::cTexture = NULL;
unsigned int SpindizzyCraftGyroscopeModel::cInstanceCount = 0;
const float SpindizzyCraftGyroscopeModel::CIRCLE_RESOLUTION = 5.0f * (M_PI / 180.0);

SpindizzyCraftGyroscopeModel::SpindizzyCraftGyroscopeModel(Vertex* location) {
  cLocation = location;
  if (cTexture == NULL) {
    cTexture = new Texture();
    generateTextureDisc();
  }
  cInstanceCount++;
}

void SpindizzyCraftGyroscopeModel::update(int milliseconds) {
  // Nothing to do
}

void SpindizzyCraftGyroscopeModel::render() {
  glTranslatef(cLocation->x, cLocation->y, cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  glRotatef((cLocation->x + cLocation->y) * -180.0f, 0.0f, 0.0f, 1.0f);
  float mBaseWidth = 0.03;
  float mTopWidth = 0.33;
  float mBaseHeight = IsoRealmsConstants::BLOCK_HEIGHT * 0.9;
  float mTopHeight = IsoRealmsConstants::BLOCK_HEIGHT * 1.80;

  cTexture->set();
  glDisable(GL_CULL_FACE);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(1.0, 0.5);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0 - mBaseWidth, 0.0 + mBaseHeight);
  glVertex3f(0.0, 0.0 + mBaseWidth, 0.0 + mBaseHeight);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0 + mBaseWidth, 0.0, 0.0 + mBaseHeight);
  glVertex3f(0.0 - mBaseWidth, 0.0, 0.0 + mBaseHeight);

  glVertex3f(0.0, 0.0, mTopHeight);
  glVertex3f(0.0, 0.0 - mBaseWidth, 0.0 + mBaseHeight);
  glVertex3f(0.0, 0.0 + mBaseWidth, 0.0 + mBaseHeight);
  glVertex3f(0.0, 0.0, mTopHeight);
  glVertex3f(0.0 + mBaseWidth, 0.0, 0.0 + mBaseHeight);
  glVertex3f(0.0 - mBaseWidth, 0.0, 0.0 + mBaseHeight);
  glEnd();

  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0, 1.0); glVertex3f(0.0 + mTopWidth, 0.0 + mTopWidth, 0.0 + mBaseHeight);
  glTexCoord2f(0.0, 1.0); glVertex3f(0.0 - mTopWidth, 0.0 + mTopWidth, 0.0 + mBaseHeight);
  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 - mTopWidth, 0.0 - mTopWidth, 0.0 + mBaseHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 + mTopWidth, 0.0 - mTopWidth, 0.0 + mBaseHeight);
  glEnd();
  glDisable(GL_ALPHA_TEST);
  
  glEnable(GL_CULL_FACE);  
}

// TODO: Nasty stuff below here.  Clean up!
void SpindizzyCraftGyroscopeModel::renderCircle(float radius, float startAngle, float endAngle, IColour* colour) {
  glBegin(GL_TRIANGLE_FAN);
  colour->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = startAngle * (M_PI / 180.0f);
  float mEndAngle = endAngle * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * radius, cos(angle) * radius);
  }
  glEnd();
}

void SpindizzyCraftGyroscopeModel::renderCircle(float outerRadius, float innerRadius, IColour* colour) {
  glBegin(GL_TRIANGLE_STRIP);
  colour->set();
  float mStartAngle = 0.0f * (M_PI / 180.0f);
  float mEndAngle = 360.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * innerRadius, cos(angle) * innerRadius);
    glVertex2f(sin(angle) * outerRadius, cos(angle) * outerRadius);
  }
  glEnd();
}

void SpindizzyCraftGyroscopeModel::generateTextureDisc() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  cTexture->setRenderTarget();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  Colour mBlack(0.0, 0.0, 0.0, 1.0);
  Colour mMagenta(0.7, 0.0, 1.0, 1.0);
  Colour mCyan(0.0, 1.0, 1.0, 1.0);
  Colour mYellow(1.0, 1.0, 0.0, 1.0);

  renderCircle(1.0f, 0.88f, &mBlack);
  renderCircle(0.88f, 0.0f, 90.0f, &mCyan);
  renderCircle(0.88f, 90.0f, 180.0f, &mMagenta);
  renderCircle(0.88f, 180.0f, 270.0f, &mYellow);
    
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  glViewport(0, 0, 1024, 768);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

SpindizzyCraftGyroscopeModel::~SpindizzyCraftGyroscopeModel() {
  if (!(--cInstanceCount)) {
    // TODO: Clean up texture!
  }
}