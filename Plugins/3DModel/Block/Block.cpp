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
  glScalef(0.75f, 0.75f, 0.75f);
  
  float xs = -IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  float ys = -IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  float xe =  IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  float ye =  IsoRealmsConstants::BLOCK_RADIUS; // TODO: Rename this
  
  ISpindizzyTexture* mTexture = cProperties->getTopSurfaceTexture();
  mTexture->set();
  glBegin(GL_QUADS);
  mTexture->texCoord2f(1.0f, 1.0f); glVertex3f(xe, ys, 0.0f);
  mTexture->texCoord2f(1.0f, 0.0f); glVertex3f(xe, ye, 0.0f);
  mTexture->texCoord2f(0.0f, 0.0f); glVertex3f(xs, ye, 0.0f);
  mTexture->texCoord2f(0.0f, 1.0f); glVertex3f(xs, ys, 0.0f);
  glEnd();

  mTexture = cProperties->getBottomSurfaceTexture();
  mTexture->set();
  glBegin(GL_QUADS);
  mTexture->texCoord2f(0.0f, 1.0f); glVertex3f(xs, ye, 0.0f);
  mTexture->texCoord2f(1.0f, 1.0f); glVertex3f(xe, ye, 0.0f);
  mTexture->texCoord2f(1.0f, 0.0f); glVertex3f(xe, ys, 0.0f);
  mTexture->texCoord2f(0.0f, 0.0f); glVertex3f(xs, ys, 0.0f);
  glEnd();

  float mBlockRadius = IsoRealmsConstants::BLOCK_RADIUS;
  float mBlockHeight = IsoRealmsConstants::BLOCK_HEIGHT;
  float mCapHeight   = mBlockHeight * 0.5;

  mTexture = cProperties->getNorthSurfaceTexture();
  mTexture->set();
  glBegin(GL_QUADS);
  switch (mTexture->getMapping()) {
    case ISpindizzyTexture::TILED: {
      mTexture->texCoord2f(0.0f, 1.0f); glVertex3f(-mBlockRadius,  mBlockRadius, mBlockHeight);
      mTexture->texCoord2f(1.0f, 1.0f); glVertex3f( mBlockRadius,  mBlockRadius, mBlockHeight);
      mTexture->texCoord2f(1.0f, 0.0f); glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(0.0f, 0.0f); glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight);
      break;
    }
    
    case ISpindizzyTexture::COLUMN_CAPPED: {
      mTexture->texCoord2f(0.0f, 0.25f); glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight + mCapHeight);
      mTexture->texCoord2f(1.0f, 0.25f); glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight + mCapHeight);
      mTexture->texCoord2f(1.0f, 0.0f);  glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(0.0f, 0.0f);  glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(0.0f, 1.0f);  glVertex3f(-mBlockRadius,  mBlockRadius,  0.0f);
      mTexture->texCoord2f(1.0f, 1.0f);  glVertex3f( mBlockRadius,  mBlockRadius,  0.0f);
      mTexture->texCoord2f(1.0f, 0.75f); glVertex3f( mBlockRadius,  mBlockRadius, -mCapHeight);
      mTexture->texCoord2f(0.0f, 0.75f); glVertex3f(-mBlockRadius,  mBlockRadius, -mCapHeight);
      break;
    }

    default: {
      // TODO: Throw Unsupported map type exception
      break;
    }
  }
  glEnd();

  mTexture = cProperties->getEastSurfaceTexture();
  mTexture->set();
  glBegin(GL_QUADS);
  switch (mTexture->getMapping()) {
    case ISpindizzyTexture::TILED: {
      mTexture->texCoord2f(0.0f, 0.0f); glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(1.0f, 0.0f); glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(1.0f, 1.0f); glVertex3f( mBlockRadius,  mBlockRadius,  0.0f);
      mTexture->texCoord2f(0.0f, 1.0f); glVertex3f( mBlockRadius, -mBlockRadius,  0.0f);
      break;
    }
    
    case ISpindizzyTexture::COLUMN_CAPPED: {
      mTexture->texCoord2f(0.0f, 0.0f);  glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(1.0f, 0.0f);  glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(1.0f, 0.25f); glVertex3f( mBlockRadius,  mBlockRadius, -mBlockHeight + mCapHeight);
      mTexture->texCoord2f(0.0f, 0.25f); glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight + mCapHeight);
      mTexture->texCoord2f(0.0f, 0.75f); glVertex3f( mBlockRadius, -mBlockRadius, -mCapHeight);
      mTexture->texCoord2f(1.0f, 0.75f); glVertex3f( mBlockRadius,  mBlockRadius, -mCapHeight);
      mTexture->texCoord2f(1.0f, 1.0f);  glVertex3f( mBlockRadius,  mBlockRadius,  0.0f);
      mTexture->texCoord2f(0.0f, 1.0f);  glVertex3f( mBlockRadius, -mBlockRadius,  0.0f);
      break;
    }

    default: {
      // TODO: Throw Unsupported map type exception
      break;
    }
  }
  glEnd();
    
  mTexture = cProperties->getSouthSurfaceTexture();
  mTexture->set();
  glBegin(GL_QUADS);
  switch (mTexture->getMapping()) {
    case ISpindizzyTexture::TILED: {
      mTexture->texCoord2f(0.0f, 0.0f); glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(1.0f, 0.0f); glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(1.0f, 1.0f); glVertex3f( mBlockRadius, -mBlockRadius,  0.0f);
      mTexture->texCoord2f(0.0f, 1.0f); glVertex3f(-mBlockRadius, -mBlockRadius,  0.0f);
      break;
    }
    
    case ISpindizzyTexture::COLUMN_CAPPED: {
      mTexture->texCoord2f(0.0f, 0.0f);  glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(1.0f, 0.0f);  glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(1.0f, 0.25f); glVertex3f( mBlockRadius, -mBlockRadius, -mBlockHeight + mCapHeight);
      mTexture->texCoord2f(0.0f, 0.25f); glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight + mCapHeight);
      mTexture->texCoord2f(0.0f, 0.75f); glVertex3f(-mBlockRadius, -mBlockRadius, -mCapHeight);
      mTexture->texCoord2f(1.0f, 0.75f); glVertex3f( mBlockRadius, -mBlockRadius, -mCapHeight);
      mTexture->texCoord2f(1.0f, 1.0f);  glVertex3f( mBlockRadius, -mBlockRadius,  0.0f);
      mTexture->texCoord2f(0.0f, 1.0f);  glVertex3f(-mBlockRadius, -mBlockRadius,  0.0f);
      break;
    }

    default: {
      // TODO: Throw Unsupported map type exception
      break;
    }
  }
  glEnd();
    
  mTexture = cProperties->getWestSurfaceTexture();
  mTexture->set();
  glBegin(GL_QUADS);
  switch (mTexture->getMapping()) {
    case ISpindizzyTexture::TILED: {
      mTexture->texCoord2f(0.0f, 1.0f); glVertex3f(-mBlockRadius, -mBlockRadius,  0.0f);
      mTexture->texCoord2f(1.0f, 1.0f); glVertex3f(-mBlockRadius,  mBlockRadius,  0.0f);
      mTexture->texCoord2f(1.0f, 0.0f); glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(0.0f, 0.0f); glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight);
      break;
    }
    
    case ISpindizzyTexture::COLUMN_CAPPED: {
      mTexture->texCoord2f(0.0f, 0.25f); glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight + mCapHeight);
      mTexture->texCoord2f(1.0f, 0.25f); glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight + mCapHeight);
      mTexture->texCoord2f(1.0f, 0.0f);  glVertex3f(-mBlockRadius,  mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(0.0f, 0.0f);  glVertex3f(-mBlockRadius, -mBlockRadius, -mBlockHeight);
      mTexture->texCoord2f(0.0f, 1.0f);  glVertex3f(-mBlockRadius, -mBlockRadius,  0.0f);
      mTexture->texCoord2f(1.0f, 1.0f);  glVertex3f(-mBlockRadius,  mBlockRadius,  0.0f);
      mTexture->texCoord2f(1.0f, 0.75f); glVertex3f(-mBlockRadius,  mBlockRadius, -mCapHeight);
      mTexture->texCoord2f(0.0f, 0.75f); glVertex3f(-mBlockRadius, -mBlockRadius, -mCapHeight);
      break;
    }

    default: {
      // TODO: Throw Unsupported map type exception
      break;
    }
  }
  glEnd();
}
