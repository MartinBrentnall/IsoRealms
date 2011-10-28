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

TextureSetChooserComponent::TextureSetChooserComponent(IComponentContainer* componentContainer, IZoneTextureSetter* zoneTextureSetter, std::vector<ITextureSet*> texturePalette) : ResizableDialog(componentContainer, "Choose Texture Set", 0.18f, 0.68f, 0.8f, 0.3f) {
  cZoneTextureSetter = zoneTextureSetter;
  WrappingGridComponent* mWrappingComponent = new WrappingGridComponent();
  for (unsigned int i = 0; i < texturePalette.size(); i++) {
    ISizedComponent* mTextureIcon = new TextureIcon(this, texturePalette[i]);
    mWrappingComponent->addComponent(mTextureIcon);
  }

  EdgeRelation* mInsideDialog = new EdgeRelation(this, EdgeRelation::INSIDE);
  IComponentBoundsCalculator* mWrappingComponentLayout = new ComponentEdgeLayout(mInsideDialog, mInsideDialog, mInsideDialog, mInsideDialog, NULL);
  mWrappingComponent->setBoundsCalculator(mWrappingComponentLayout);
  addComponent(mWrappingComponent);
}

TextureSetChooserComponent::TextureIcon::TextureIcon(TextureSetChooserComponent* parent, ITextureSet* textureSet) {
  cParent = parent;
  cTextureSet = textureSet;
}

float TextureSetChooserComponent::TextureIcon::getWidth() {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreen->getAspectRatio();
  return 0.2f * mAspectRatio;
}

float TextureSetChooserComponent::TextureIcon::getHeight() {
  return 0.2f;
}

void TextureSetChooserComponent::TextureIcon::render() {
  float mLeft = getLeft();
  float mRight = getRight();
  float mTop = getTop();
  float mBottom = getBottom();
  glBindTexture(GL_TEXTURE_2D, 0);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  glVertex2f(mLeft, mTop);
  glVertex2f(mLeft, mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();

  glPushMatrix();
  float mScale = 0.12f;
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mXLocation = getLeft() + (getRight() - getLeft()) / 2.0f;
  float mYLocation = getBottom() + (getTop() - getBottom()) / 2.0f;
  glTranslatef(mXLocation, mYLocation, 0.0f);
  float mAspectRatio = mScreen->getAspectRatio();
  glScalef(mAspectRatio * mScale, mScale, mScale);
  glRotatef(-55.0f, 1.0f, 0.0f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // TODO: Must get this right; check with how the editor is doing it!
  glColor3f(1.0f, 1.0f, 1.0f);

  ITexture* mTexture = cTextureSet->getTexture("SwitchDiamondFilled");
  mTexture->set();
  glBegin(GL_QUADS);
  mTexture->texCoord2f(1.0f, 1.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 1.0f);  glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 0.0f);  glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(1.0f, 0.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  glEnd();

  mTexture = cTextureSet->getTexture("WallSouth");
  mTexture->set();
  glBegin(GL_QUADS);
  mTexture->texCoord2f(1.0f, 1.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 1.0f);  glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 0.0f);  glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(1.0f, 0.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  glEnd();

  mTexture = cTextureSet->getTexture("WallNorth");
  mTexture->set();
  glBegin(GL_QUADS);
  mTexture->texCoord2f(1.0f, 0.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 0.0f);  glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 1.0f);  glVertex3f(-IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(1.0f, 1.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  glEnd();

  mTexture = cTextureSet->getTexture("WallEast");
  mTexture->set();
  glBegin(GL_QUADS);
  mTexture->texCoord2f(1.0f, 0.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 0.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 1.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(1.0f, 1.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  glEnd();

  mTexture = cTextureSet->getTexture("WallWest");
  mTexture->set();
  glBegin(GL_QUADS);
  mTexture->texCoord2f(1.0f, 1.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 1.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(0.0f, 0.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS, -IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  mTexture->texCoord2f(1.0f, 0.0f);  glVertex3f( IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_RADIUS,  IsoRealmsConstants::BLOCK_HEIGHT / 2.0);
  glEnd();

  glPopMatrix();
}

void TextureSetChooserComponent::TextureIcon::update(int ticks) {
  // Nothing to do
}

bool TextureSetChooserComponent::TextureIcon::mouseButtonDown(SDL_Event& event) {
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
  float mX = mScreen->getXLocation(event.button.x);
  float mY = mScreen->getYLocation(event.button.y);
  if (contains(mX, mY)) {
    cParent->cZoneTextureSetter->setTextureSet(cTextureSet);
    cParent->relinquishFocus();
    return true;
  }
  return false;
}

bool TextureSetChooserComponent::TextureIcon::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      return mouseButtonDown(event);
    }
  }
  return false;
}

void TextureSetChooserComponent::renderResizableDialogContent() {
  // Nothing to do.
}

void TextureSetChooserComponent::updateResizableDialogContent(int) {
  // Nothing to do.
}

bool TextureSetChooserComponent::inputResizableDialogContent(SDL_Event& event) {
  return false;
}

