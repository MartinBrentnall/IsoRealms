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

Texture* SpindizzyCraftPyramidModel::cTextureTop = NULL;
Texture* SpindizzyCraftPyramidModel::cTextureSide = NULL;
unsigned int SpindizzyCraftPyramidModel::cInstanceCount = 0;
const float SpindizzyCraftPyramidModel::OUTLINE = 0.82f;

SpindizzyCraftPyramidModel::SpindizzyCraftPyramidModel(Vertex* location) {
  cLocation = location;
  if (cTextureTop == NULL) {
    cTextureTop = new Texture();
    cTextureSide = new Texture();
    
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glPopAttrib();

    cTextureTop->setRenderTarget();  generateTextureTop();
    cTextureSide->setRenderTarget(); generateTextureSide();
    
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    glViewport(0, 0, 1024, 768);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
  }
  cInstanceCount++;
}

void SpindizzyCraftPyramidModel::update(int milliseconds) {
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
  glDisable(GL_CULL_FACE);
  glBegin(GL_TRIANGLES);
  glColor3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0 - mBaseWidth, 0.0 + mBaseHeight * 1.2);
  glVertex3f(0.0, 0.0 + mBaseWidth, 0.0 + mBaseHeight * 1.2);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0 + mBaseWidth, 0.0, 0.0 + mBaseHeight * 1.2);
  glVertex3f(0.0 - mBaseWidth, 0.0, 0.0 + mBaseHeight * 1.2);
  glColor3f(1.0, 1.0, 1.0);
  glEnd();

  cTextureSide->set();
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

  cTextureTop->set();
  glBegin(GL_QUADS);
  glTexCoord2f(1.0, 1.0); glVertex3f(0.0 + mTopWidth, 0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.0, 1.0); glVertex3f(0.0 - mTopWidth, 0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 - mTopWidth, 0.0 - mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 + mTopWidth, 0.0 - mTopWidth, 0.0 + mTopHeight);
  glEnd();
  
  glEnable(GL_CULL_FACE);  
}

// TODO: Nasty stuff below here.  Clean up!
void SpindizzyCraftPyramidModel::generateTextureTop() {
  Colour mTop(0.0, 1.0, 1.0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_QUADS);
  mTop.set();
  glVertex2f(-OUTLINE, -OUTLINE);
  glVertex2f( OUTLINE, -OUTLINE);
  glVertex2f( OUTLINE,  OUTLINE);
  glVertex2f(-OUTLINE,  OUTLINE);
  glEnd();
}

void SpindizzyCraftPyramidModel::generateTextureSide() {
  Colour mSide(0.7, 0.0, 1.0);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
  mSide.set();
  // TODO: 'magic' numbers 0.74f and 0.62f should be calculated properly
  glVertex2f(-0.74, -OUTLINE);
  glVertex2f( 0.74, -OUTLINE);
  glVertex2f( 0.0f,  0.62f);
  glEnd();
}

SpindizzyCraftPyramidModel::~SpindizzyCraftPyramidModel() {
  if (!(--cInstanceCount)) {
    // TODO: Delete textures!
  }
}

