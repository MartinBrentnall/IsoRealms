/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "ModelSprite.h"

ModelSprite::ModelSprite(Vertex* location, IModelTypeSprite* modelType, float* size, bool flip) {
  cLocation = location;
  cModelType = modelType;
  cSize = size;
  cFlip = flip;
}

void ModelSprite::update(unsigned int milliseconds) {
  // Nothing to do.
}

void ModelSprite::render() {
  glPushMatrix();
  glTranslatef(cLocation->x, cLocation->y, (cLocation->z + 0.05f) * IsoRealmsConstants::BLOCK_HEIGHT); // TODO: That 0.05f looks like a hack
  cModelType->setTexture();
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);
  glBegin(GL_QUADS);
  float mRadius = IsoRealmsConstants::BLOCK_RADIUS * *cSize;
  float mTextureCoord = cFlip ? -1.0f : 1.0f;
  glTexCoord2f(mTextureCoord, mTextureCoord); glVertex3f( mRadius, -mRadius, 0.0f);
  glTexCoord2f(mTextureCoord, 0.0f);          glVertex3f( mRadius,  mRadius, 0.0f);
  glTexCoord2f(0.0f,          0.0f);          glVertex3f(-mRadius,  mRadius, 0.0f);
  glTexCoord2f(0.0f,          mTextureCoord); glVertex3f(-mRadius, -mRadius, 0.0f);
  glEnd();
  glEnable(GL_CULL_FACE);
  glDisable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopMatrix();
}
