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
#include "ModelSpindizzyCraftGyroscope.h"

ModelSpindizzyCraftGyroscope::ModelSpindizzyCraftGyroscope(Vertex* location, Texture** texture) {
  cLocation = location;
  cTexture = texture;
}

void ModelSpindizzyCraftGyroscope::update(unsigned int milliseconds) {
  // Nothing to do
}

void ModelSpindizzyCraftGyroscope::render() {
  glTranslatef(cLocation->x, cLocation->y, cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  glRotatef((cLocation->x + cLocation->y) * -180.0f, 0.0f, 0.0f, 1.0f);
  float mBaseWidth = 0.03;
  float mTopWidth = 0.33;
  float mBaseHeight = IsoRealmsConstants::BLOCK_HEIGHT * 0.9;
  float mTopHeight = IsoRealmsConstants::BLOCK_HEIGHT * 1.80;

  (*cTexture)->set();
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
