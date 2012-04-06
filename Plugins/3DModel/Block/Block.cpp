/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "Block.h"

Block::Block(Vertex* location, BlockProperties* properties) {
  cLocation = location;
  cProperties = properties;
}

void Block::update(int milliseconds) {
  // Nothing to do.
}

void Block::render() {
  glTranslatef(cLocation->x, cLocation->y, cLocation->z * IsoRealmsConstants::BLOCK_HEIGHT);
  glScalef(0.65f, 0.65f, 0.65f);
  
  float xs = -IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  float ys = -IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  float xe =  IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  float ye =  IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  
  ITexture* mTexture = cProperties->getTopSurfaceTexture();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(xe, ys, 0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(xe, ye, 0.0f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(xs, ye, 0.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(xs, ys, 0.0f);
  glEnd();

  mTexture = cProperties->getBottomSurfaceTexture();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(xs, ye, 0.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(xe, ye, 0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(xe, ys, 0.0f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(xs, ys, 0.0f);
  glEnd();

  float mBlockRadius = IsoRealmsConstants::BLOCK_RADIUS;
  float mBlockHeight = IsoRealmsConstants::BLOCK_HEIGHT;
  float mCapHeight   = mBlockHeight * 0.5;

  mTexture = cProperties->getNorthSurfaceTextureBottom();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight + mCapHeight);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight + mCapHeight);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight);
  glEnd();
  
  mTexture = cProperties->getNorthSurfaceTextureTop();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-mBlockRadius,  mBlockRadius,  0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( mBlockRadius,  mBlockRadius,  0.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( mBlockRadius,  mBlockRadius, -mCapHeight);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-mBlockRadius,  mBlockRadius, -mCapHeight);
  glEnd();

  mTexture = cProperties->getEastSurfaceTextureBottom();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight + mCapHeight);
  glTexCoord2f(0.0f, 1.0f); glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight + mCapHeight);
  glEnd();
  
  mTexture = cProperties->getEastSurfaceTextureTop();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f); glVertex3f( mBlockRadius, -mBlockRadius, -mCapHeight);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( mBlockRadius,  mBlockRadius, -mCapHeight);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( mBlockRadius,  mBlockRadius,  0.0f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f( mBlockRadius, -mBlockRadius,  0.0f);
  glEnd();
    
  mTexture = cProperties->getSouthSurfaceTextureBottom();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight + mCapHeight);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight + mCapHeight);
  glEnd();
  
  mTexture = cProperties->getSouthSurfaceTextureTop();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-mBlockRadius, -mBlockRadius, -mCapHeight);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( mBlockRadius, -mBlockRadius, -mCapHeight);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( mBlockRadius, -mBlockRadius,  0.0f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-mBlockRadius, -mBlockRadius,  0.0f);
  glEnd();
    
  mTexture = cProperties->getWestSurfaceTextureBottom();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight + mCapHeight);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight + mCapHeight);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight);
  glEnd();
  
  mTexture = cProperties->getWestSurfaceTextureTop();
  mTexture->set();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-mBlockRadius, -mBlockRadius,  0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-mBlockRadius,  mBlockRadius,  0.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-mBlockRadius,  mBlockRadius, -mCapHeight);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-mBlockRadius, -mBlockRadius, -mCapHeight);
  glEnd();
}
