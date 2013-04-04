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
#include "SpindizzyCraftPyramidModel.h"

SpindizzyCraftPyramidModel::SpindizzyCraftPyramidModel(Vertex* location, Texture** textureTop, Texture** textureSide) {
  cTextureTop = textureTop;
  cTextureSide = textureSide;
  cLocation = location;
}

void SpindizzyCraftPyramidModel::update(unsigned int milliseconds) {
  // Nothing to do
}

void SpindizzyCraftPyramidModel::render() {
  float mBaseWidth = 0.03;
  float mTopWidth = 0.3;
  float mBaseHeight = IsoRealmsConstants::BLOCK_HEIGHT * 0.5;
  float mTopHeight = IsoRealmsConstants::BLOCK_HEIGHT * 1.50;

  glTranslatef(cLocation->x, cLocation->y, cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  glRotatef((cLocation->x + cLocation->y) * -90.0f, 0.0f, 0.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  (*cTextureSide)->set();
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.0, 0.0);   
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0 - mBaseWidth, 0.0 + mBaseHeight * 1.2);
  glVertex3f(0.0, 0.0 + mBaseWidth, 0.0 + mBaseHeight * 1.2);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0 + mBaseWidth, 0.0, 0.0 + mBaseHeight * 1.2);
  glVertex3f(0.0 - mBaseWidth, 0.0, 0.0 + mBaseHeight * 1.2);
  glColor3f(1.0, 1.0, 1.0);
  glEnd();

  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 + mTopWidth,  0.0 - mTopWidth,  0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 - mTopWidth,  0.0 - mTopWidth,  0.0 + mTopHeight);
  glTexCoord2f(0.5, 1.0); glVertex3f(0.0,              0.0,              0.0 + mBaseHeight);

  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 + mTopWidth,  0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 - mTopWidth,  0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.5, 1.0); glVertex3f(0.0,              0.0,             0.0 + mBaseHeight);

  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 + mTopWidth,  0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 + mTopWidth,  0.0 - mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.5, 1.0); glVertex3f(0.0,              0.0,             0.0 + mBaseHeight);

  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 - mTopWidth,  0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 - mTopWidth,  0.0 - mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.5, 1.0); glVertex3f(0.0,              0.0,             0.0 + mBaseHeight);
  glEnd();

  (*cTextureTop)->set();
  glBegin(GL_QUADS);
  glTexCoord2f(1.0, 1.0); glVertex3f(0.0 + mTopWidth, 0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.0, 1.0); glVertex3f(0.0 - mTopWidth, 0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 - mTopWidth, 0.0 - mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 + mTopWidth, 0.0 - mTopWidth, 0.0 + mTopHeight);
  glEnd();
  
  glEnable(GL_CULL_FACE);  
}
