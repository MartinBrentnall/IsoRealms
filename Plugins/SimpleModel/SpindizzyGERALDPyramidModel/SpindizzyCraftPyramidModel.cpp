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

std::map<SpindizzyCraftPyramidModel::TextureID, GLuint> SpindizzyCraftPyramidModel::cTextures = std::map<SpindizzyCraftPyramidModel::TextureID, GLuint>();
unsigned int SpindizzyCraftPyramidModel::cInstanceCount = 0;

SpindizzyCraftPyramidModel::SpindizzyCraftPyramidModel() {
  if (cTextures.empty()) {
    cTextures[TEXTURE_TOP] = registerTexture(generateTextureTop());
    cTextures[TEXTURE_SIDE] = registerTexture(generateTextureSide());
  }
  cInstanceCount++;
}

void SpindizzyCraftPyramidModel::update(int milliseconds) {
  // Nothing to do
}

// TODO: Where to do this rotation?
//       1. In here with a "location aware" plug-in?
//       2. Configurable in the craft itself?
/*  float mHeight = cLocation.z * IsoRealmsConstants::BLOCK_HEIGHT;
  glTranslatef(cLocation.x, cLocation.y, mHeight);
  glRotatef((-cLocation.x + cLocation.y) * 90.0, 0.0, 0.0, 1.0);*/

void SpindizzyCraftPyramidModel::render() {
  float mBaseWidth = 0.03;
  float mTopWidth = 0.3;
  float mBaseHeight = IsoRealmsConstants::BLOCK_HEIGHT * 0.5;
  float mTopHeight = IsoRealmsConstants::BLOCK_HEIGHT * 1.50;

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

  glBindTexture(GL_TEXTURE_2D, cTextures[TEXTURE_SIDE]);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 + mTopWidth,  0.0 - mTopWidth,  0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 - mTopWidth,  0.0 - mTopWidth,  0.0 + mTopHeight);
  glTexCoord2f(0.5, 0.5); glVertex3f(0.0,              0.0,              0.0 + mBaseHeight);

  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 + mTopWidth,  0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 - mTopWidth,  0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.5, 0.5); glVertex3f(0.0,              0.0,             0.0 + mBaseHeight);

  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 + mTopWidth,  0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 + mTopWidth,  0.0 - mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.5, 0.5); glVertex3f(0.0,              0.0,             0.0 + mBaseHeight);

  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 - mTopWidth,  0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 - mTopWidth,  0.0 - mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.5, 0.5); glVertex3f(0.0,              0.0,             0.0 + mBaseHeight);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, cTextures[TEXTURE_TOP]);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0, 1.0); glVertex3f(0.0 + mTopWidth, 0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.0, 1.0); glVertex3f(0.0 - mTopWidth, 0.0 + mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 - mTopWidth, 0.0 - mTopWidth, 0.0 + mTopHeight);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 + mTopWidth, 0.0 - mTopWidth, 0.0 + mTopHeight);
  glEnd();
  
  glEnable(GL_CULL_FACE);  
}

// TODO: Nasty stuff below here.  Clean up!
GLuint SpindizzyCraftPyramidModel::registerTexture(Image* image) {
  GLuint mTextureID;
  int mDepth = image->getDepth();
  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, mDepth, GL_UNSIGNED_BYTE, image->data);
  return mTextureID;
}

Image* SpindizzyCraftPyramidModel::generateTextureTop() {
  Image* mImage = new Image(64, 64, false);
  int mEdgeWidth = mImage->sizeX / 12;
  Colour mEdge(0.0, 0.0, 0.0);
  Colour mTop(0.0, 1.0, 1.0);
  mImage->drawSquare(&mEdge, 0, mImage->sizeX, 0, mImage->sizeY);
  mImage->drawSquare(&mTop, mEdgeWidth, mImage->sizeX - mEdgeWidth, mEdgeWidth, mImage->sizeY - mEdgeWidth);
  return mImage;
}

Image* SpindizzyCraftPyramidModel::generateTextureSide() {
  Image* mImage = new Image(64, 64, false);
  int mEdgeWidth = mImage->sizeX / 12;
  Colour mEdge(0.0, 0.0, 0.0);
  Colour mSide(0.7, 0.0, 1.0);
  mImage->drawSquare(&mEdge, 0, mImage->sizeX, 0, mImage->sizeY);

  // TODO: 'magic' numbers 5 and 30 should be calculated from image size
  mImage->drawTriangle(&mSide, mEdgeWidth + 5, mEdgeWidth, mImage->sizeX - (mEdgeWidth + 5), mEdgeWidth, mImage->sizeX / 2, mImage->sizeY - (mEdgeWidth + 32));
  return mImage;
}

SpindizzyCraftPyramidModel::~SpindizzyCraftPyramidModel() {
  if (!(--cInstanceCount)) {
    for (std::map<TextureID, GLuint>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
      glDeleteTextures(1, &(i->second));
    }
  }
  // TODO: See if we must clear up the Image object too.
}

