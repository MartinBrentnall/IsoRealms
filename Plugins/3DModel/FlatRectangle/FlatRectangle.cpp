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
#include "FlatRectangle.h"

FlatRectangle::FlatRectangle(Vertex* location, ISpindizzyTexture** texture, float* size) {
  cLocation = location;
  cTexture = texture;
  cSize = size;
}

void FlatRectangle::update(int milliseconds) {
  // Nothing to do.
}

void FlatRectangle::render() {
  glPushMatrix();
  glTranslatef(cLocation->x, cLocation->y, (cLocation->z + 0.05f) * IsoRealmsConstants::BLOCK_HEIGHT);
  (*cTexture)->set();
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);
  glBegin(GL_QUADS);
  float mRadius = IsoRealmsConstants::BLOCK_RADIUS * *cSize;
  (*cTexture)->texCoord2f(1.0, 1.0); glVertex3f( mRadius, -mRadius, 0.0);
  (*cTexture)->texCoord2f(1.0, 0.0); glVertex3f( mRadius,  mRadius, 0.0);
  (*cTexture)->texCoord2f(0.0, 0.0); glVertex3f(-mRadius,  mRadius, 0.0);
  (*cTexture)->texCoord2f(0.0, 1.0); glVertex3f(-mRadius, -mRadius, 0.0);
  glEnd();
  glEnable(GL_CULL_FACE);
  glDisable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopMatrix();
}
