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

std::map<SpindizzyCraftBallModel::TextureID, GLuint> SpindizzyCraftBallModel::cTextures = std::map<SpindizzyCraftBallModel::TextureID, GLuint>();
unsigned int SpindizzyCraftBallModel::cInstanceCount = 0;

SpindizzyCraftBallModel::SpindizzyCraftBallModel() {
  if (cTextures.empty()) {
    cTextures[TEXTURE_BALL] = generateTextureBall();
  }
  cInstanceCount++;
}

void SpindizzyCraftBallModel::update(int milliseconds) {
  // Nothing to do
}

void SpindizzyCraftBallModel::render() {
  float mBallWidth = 0.33;
  float mBallHeight = 0.33;

  // TODO: Camera awareness!
/*  glRotatef(-rotation + 90.0, 0.0, 0.0, 1.0);
  glRotatef(tilt, 0.0, 1.0, 0.0);*/
  glColor3f(1.0, 1.0, 1.0);
  glEnable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, cTextures[TEXTURE_BALL]);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0, 1.0); glVertex3f(0.0 + mBallHeight, 0.0 + mBallWidth, 0.0);
  glTexCoord2f(0.0, 1.0); glVertex3f(0.0 - mBallHeight, 0.0 + mBallWidth, 0.0);
  glTexCoord2f(0.0, 0.0); glVertex3f(0.0 - mBallHeight, 0.0 - mBallWidth, 0.0);
  glTexCoord2f(1.0, 0.0); glVertex3f(0.0 + mBallHeight, 0.0 - mBallWidth, 0.0);
  glEnd();
  glDisable(GL_ALPHA_TEST);
  
}

// TODO: Nasty stuff below here.  Clean up!
GLuint SpindizzyCraftBallModel::generateTextureBall() {
  Image* mImage = new Image(64, 64, false);
  Colour mTransparent(0.0, 0.0, 0.0, 0.0);
  Colour mBlack(0.0, 0.0, 0.0, 1.0);
  Colour mMagenta(0.7, 0.0, 1.0, 1.0);
  Colour mYellow(1.0, 1.0, 0.0, 1.0);
  mImage->drawSquare(&mTransparent, 0, mImage->getWidth(), 0, mImage->getHeight());
  mImage->drawCircle(&mBlack, mImage->getWidth() / 2);
  mImage->drawCircle(&mMagenta, int(mImage->getWidth() / 2.3));
  mImage->drawCircle(&mYellow, int(mImage->getWidth() / 3.5));
  mImage->drawOffsetCircle(&mMagenta, int(mImage->getWidth() / 3.0));
  GLuint mTextureID = mImage->generateTexture();
  delete mImage;
  return mTextureID;
}

SpindizzyCraftBallModel::~SpindizzyCraftBallModel() {
  if (!(--cInstanceCount)) {
    for (std::map<TextureID, GLuint>::iterator i = cTextures.begin(); i != cTextures.end(); i++) {
      glDeleteTextures(1, &(i->second));
    }
  }
}

