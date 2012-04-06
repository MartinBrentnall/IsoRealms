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
#include "SpindizzyCraftBallModel.h"

Texture* SpindizzyCraftBallModel::cTexture = NULL;
unsigned int SpindizzyCraftBallModel::cInstanceCount = 0;
const float SpindizzyCraftBallModel::CIRCLE_RESOLUTION            = 5.0f * (M_PI / 180.0);

SpindizzyCraftBallModel::SpindizzyCraftBallModel(Vertex* location, ICamera* camera) {
  cLocation = location;
  cCamera = camera;
  if (cTexture == NULL) {
    cTexture = new Texture();
    generateTextureBall();
  }
  cInstanceCount++;
}

void SpindizzyCraftBallModel::update(int milliseconds) {
  // Nothing to do
}

void SpindizzyCraftBallModel::render() {
  float mBallWidth = 0.33f;
  float mBallHeight = 0.33f;

  glTranslatef(cLocation->x, cLocation->y, cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT + mBallHeight);
  float mAngle = cCamera->getAngle();
  float mTilt = cCamera->getTilt();
  glRotatef(-mAngle + 90.0f, 0.0f, 0.0f, 1.0f);
  glRotatef(mTilt, 0.0f, 1.0f, 0.0f);
  glEnable(GL_ALPHA_TEST);
  cTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f + mBallHeight, 0.0f + mBallWidth, 0.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f - mBallHeight, 0.0f + mBallWidth, 0.0f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f - mBallHeight, 0.0f - mBallWidth, 0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f + mBallHeight, 0.0f - mBallWidth, 0.0f);
  glEnd();
  glDisable(GL_ALPHA_TEST);
}

void SpindizzyCraftBallModel::renderCircle(float radius, IColour* colour) {
  glBegin(GL_TRIANGLE_FAN);
  colour->set();
  glVertex2f(0.0f, 0.0f);
  float mStartAngle = 0.0f * (M_PI / 180.0f);
  float mEndAngle = 360.0f * (M_PI / 180.0f);
  for (float angle = mEndAngle; angle >= mStartAngle; angle -= CIRCLE_RESOLUTION) {
    glVertex2f(sin(angle) * radius, cos(angle) * radius);
  }
  glEnd();
}

// TODO: Nasty stuff below here.  Clean up!
void SpindizzyCraftBallModel::generateTextureBall() {
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glPopAttrib();

  cTexture->setRenderTarget();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  IColour* mBlack = new Colour(0.0f, 0.0f, 0.0f, 1.0f);
  IColour* mMagenta = new Colour(0.7f, 0.0f, 1.0f, 1.0f);
  IColour* mYellow = new Colour(1.0f, 1.0f, 0.0f, 1.0f);
  renderCircle(1.0f, mBlack);
  renderCircle(0.85f, mMagenta);
  renderCircle(0.7f, mYellow);
  glTranslatef(-0.1f, -0.1f, 0.0f);
  renderCircle(0.75f, mMagenta);
  glTranslatef(0.1f, 0.1f, 0.0f);
  glEnd();

  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  glViewport(0, 0, 1024, 768);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

SpindizzyCraftBallModel::~SpindizzyCraftBallModel() {
  if (!(--cInstanceCount)) {
    // TODO: Clean up the texture!
  }
}

