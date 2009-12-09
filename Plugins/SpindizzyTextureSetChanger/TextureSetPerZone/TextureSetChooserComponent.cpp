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
#include "TextureSetChooserComponent.h"

TextureSetChooserComponent::TextureSetChooserComponent(IComponentContainer* componentContainer, std::vector<ISpindizzyTextureSet*> texturePalette) : RectangleComponent(componentContainer, 0.18f, 0.68f, 0.8f, 0.3f) {
  cTexturePalette = texturePalette;
  cSpacingSize = 0.01f;
  cPreviewSize = 0.1f;
}

void TextureSetChooserComponent::renderContent() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  float mLeft = getLeft();
  float mTop = getTop();
  for (unsigned int i = 0; i < cTexturePalette.size(); i++) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(mLeft + cSpacingSize * mAspectRatio,                         mTop - cSpacingSize);
    glVertex2f(mLeft + (cSpacingSize + cPreviewSize * 2.0f) * mAspectRatio, mTop - cSpacingSize);
    glVertex2f(mLeft + (cSpacingSize + cPreviewSize * 2.0f) * mAspectRatio, mTop - (cSpacingSize + cPreviewSize * 2.0f));
    glVertex2f(mLeft + cSpacingSize * mAspectRatio,                         mTop - (cSpacingSize + cPreviewSize * 2.0f));
    glEnd();

    glPushMatrix();
    glTranslatef(mLeft + (cSpacingSize + cPreviewSize) * mAspectRatio, mTop - (cSpacingSize + cPreviewSize), 0.0f);
    glScalef(cPreviewSize * mAspectRatio, cPreviewSize, cPreviewSize);
/*    glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);*/
    // TODO: Scale the icon
//  glScalef(1.3f, 1.3f, 1.3f);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f( 1.0f,  1.0f);
    glVertex2f(-1.0f,  1.0f);
    glEnd();
    glPopMatrix();
  }
  
}

void TextureSetChooserComponent::updateContent(int) {
}

bool TextureSetChooserComponent::inputContent(SDL_Event& event) {
  return false;
}

