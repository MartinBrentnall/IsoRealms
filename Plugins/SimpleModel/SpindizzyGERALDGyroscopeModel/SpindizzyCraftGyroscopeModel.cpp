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

std::map<SpindizzyCraftGyroscopeModel::TextureID, GLuint> SpindizzyCraftGyroscopeModel::cTextures = std::map<SpindizzyCraftGyroscopeModel::TextureID, GLuint>();

SpindizzyCraftGyroscopeModel::SpindizzyCraftGyroscopeModel() {
  if (cTextures.empty()) {
    cTextures[TEXTURE_DISC] = registerTexture(generateTextureDisc());
  }
}

void SpindizzyCraftGyroscopeModel::update(int milliseconds) {
  // Nothing to do
}

void SpindizzyCraftGyroscopeModel::render() {
  float mBaseWidth = 0.03;
  float mTopWidth = 0.33;
  float mBaseHeight = IsoRealmsConstants::BLOCK_HEIGHT * 0.9;
  float mTopHeight = IsoRealmsConstants::BLOCK_HEIGHT * 1.80;

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_CULL_FACE);
  glBegin(GL_TRIANGLES);
  glColor3f(0.0, 0.0, 0.0);
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
  glColor3f(1.0, 1.0, 1.0);
  glEnd();

  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, cTextures[TEXTURE_DISC]);
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
GLuint SpindizzyCraftGyroscopeModel::registerTexture(Image* image) {
  GLuint mTextureID;
  int mDepth = image->getDepth();
  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, mDepth, image->sizeX, image->sizeY, 0, mDepth, GL_UNSIGNED_BYTE, image->data);
  return mTextureID;
}

Image* SpindizzyCraftGyroscopeModel::generateTextureDisc() {
  Image* mImage = new Image(64, 64, true);
  Colour mTransparent(0.0, 0.0, 0.0, 0.0);
  Colour mBlack(0.0, 0.0, 0.0, 1.0);
  Colour mMagenta(0.7, 0.0, 1.0, 1.0);
  Colour mCyan(0.0, 1.0, 1.0, 1.0);
  Colour mYellow(1.0, 1.0, 0.0, 1.0);
  mImage->drawSquare(&mTransparent, 0, mImage->sizeX, 0, mImage->sizeY);
  mImage->drawCircle(&mBlack, mImage->sizeX / 2);
  mImage->drawCircle(&mTransparent, int(mImage->sizeX / 2.3));
  mImage->drawQuarterCircle(&mCyan, int(mImage->sizeX / 2.3), 1);
  mImage->drawQuarterCircle(&mMagenta, int(mImage->sizeX / 2.3), 2);
  mImage->drawQuarterCircle(&mYellow, int(mImage->sizeX / 2.3), 3);
  return mImage;
}
