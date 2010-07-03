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
unsigned int SpindizzyCraftGyroscopeModel::cInstanceCount = 0;

SpindizzyCraftGyroscopeModel::SpindizzyCraftGyroscopeModel(Vertex* location) {
  cLocation = location;
  if (cTextures.empty()) {
    cTextures[TEXTURE_DISC] = generateTextureDisc();
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

  glBindTexture(GL_TEXTURE_2D, cTextures[TEXTURE_DISC]);
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
GLuint SpindizzyCraftGyroscopeModel::generateTextureDisc() {
  Image* mImage = new Image(64, 64, true);
  Colour mTransparent(0.0, 0.0, 0.0, 0.0);
  Colour mBlack(0.0, 0.0, 0.0, 1.0);
  Colour mMagenta(0.7, 0.0, 1.0, 1.0);
  Colour mCyan(0.0, 1.0, 1.0, 1.0);
  Colour mYellow(1.0, 1.0, 0.0, 1.0);
  mImage->drawSquare(&mTransparent, 0, mImage->getWidth(), 0, mImage->getHeight());
  mImage->drawCircle(&mBlack, mImage->getWidth() / 2);
  mImage->drawCircle(&mTransparent, int(mImage->getWidth() / 2.3));
  mImage->drawQuarterCircle(&mCyan, int(mImage->getWidth() / 2.3), 1);
  mImage->drawQuarterCircle(&mMagenta, int(mImage->getWidth() / 2.3), 2);
  mImage->drawQuarterCircle(&mYellow, int(mImage->getWidth() / 2.3), 3);
  GLuint mTextureID = mImage->generateTexture();
  delete mImage;
  return mTextureID;
}

SpindizzyCraftGyroscopeModel::~SpindizzyCraftGyroscopeModel() {
  if (!(--cInstanceCount)) {
    for (std::map<TextureID, GLuint>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
      glDeleteTextures(1, &(i->second));
    }
  }
}