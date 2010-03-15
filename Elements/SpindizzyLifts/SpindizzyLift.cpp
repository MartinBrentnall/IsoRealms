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
#include "SpindizzyLift.h"

SpindizzyLift::SpindizzyLift(IElementFactory* elementFactory, BlockLocation* location, ISpindizzyTexture* texture, SpindizzyLiftProperties* properties, int bottom, int top) : IElement(elementFactory) {
  cTopDelay    = properties->getTopDelay();
  cBottomDelay = properties->getBottomDelay();
  cUpSpeed     = properties->getUpSpeed();
  cDownSpeed   = properties->getDownSpeed();
  cTexture     = texture;
  cLocation    = BlockLocation(*location);
  cBottom      = bottom;
  cTop         = top;
}

void SpindizzyLift::setTexture(ISpindizzyTexture* texture) {
  cTexture = texture;
}

void SpindizzyLift::renderStatic() {
  // Nothing to do.
}

void SpindizzyLift::renderEditingArrow() {
  float mLineRadius = IsoRealmsConstants::BLOCK_RADIUS * 0.5;
  float mArrowOffset = IsoRealmsConstants::BLOCK_HEIGHT * 0.5;
  glBegin(GL_LINES);
  glVertex3f(cLocation.x, cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x, cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT);

  // Top point
  glVertex3f(cLocation.x,               cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x + mLineRadius, cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT - mArrowOffset);
  glVertex3f(cLocation.x,               cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x - mLineRadius, cLocation.y, cTop * IsoRealmsConstants::BLOCK_HEIGHT - mArrowOffset);

  // Bottom point
  glVertex3f(cLocation.x,               cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x + mLineRadius, cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT + mArrowOffset);
  glVertex3f(cLocation.x,               cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT);
  glVertex3f(cLocation.x - mLineRadius, cLocation.y, cBottom * IsoRealmsConstants::BLOCK_HEIGHT + mArrowOffset);
  glEnd();
}

void SpindizzyLift::renderStaticEditing() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glColor3f(1.0, 1.0, 0.0);
  glLineWidth(3.0);
  glLineStipple(1, 255);
  glEnable(GL_LINE_STIPPLE);
  renderEditingArrow();
  glColor3f(1.0, 0.0, 0.0);
  glDisable(GL_LINE_STIPPLE);
  glLineWidth(5.0);
  renderEditingArrow();
  glColor3f(1.0, 1.0, 1.0);
  glLineWidth(1.0);
  glPopMatrix();
}

std::vector<IVisualElement*> SpindizzyLift::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> SpindizzyLift::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IInteractiveElement*> SpindizzyLift::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  return mInteractiveElements;
}

void SpindizzyLift::update(int milliseconds) {
  // TODO: Make the lift move!
}

void SpindizzyLift::render() {
  glPushMatrix();
  glTranslatef(cLocation.x, cLocation.y, cLocation.z * IsoRealmsConstants::BLOCK_HEIGHT + (IsoRealmsConstants::BLOCK_HEIGHT * 0.05));
  cTexture->set();
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);
  glDisable(GL_CULL_FACE);
  glBegin(GL_QUADS);
  float mScaleFactor = 1.45;
  cTexture->texCoord2f(1.0, 1.0); glVertex3f(    IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0 - IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0.0);
  cTexture->texCoord2f(1.0, 0.0); glVertex3f(    IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor,     IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0.0);
  cTexture->texCoord2f(0.0, 0.0); glVertex3f(0 - IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor,     IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0.0);
  cTexture->texCoord2f(0.0, 1.0); glVertex3f(0 - IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0 - IsoRealmsConstants::BLOCK_RADIUS * mScaleFactor, 0.0);
  glEnd();
  glEnable(GL_CULL_FACE);
  glDisable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopMatrix();
}

void SpindizzyLift::save(DOMNodeWriter* node, BlockLocation& relative) {
  DOMNodeWriter* mLocation = node->addBranch("Location");
  cLocation.saveRelative(mLocation, relative);
  DOMNodeWriter* mLiftRange = node->addBranch("LiftMovement");
  mLiftRange->addAttribute("top", cTop - relative.z);
  mLiftRange->addAttribute("bottom", cBottom - relative.z);
  DOMNodeWriter* mLiftProperties = node->addBranch("LiftProperties");
  mLiftProperties->addAttribute("upSpeed", cUpSpeed);
  mLiftProperties->addAttribute("downSpeed", cDownSpeed);
  mLiftProperties->addAttribute("topDelay", cTopDelay);
  mLiftProperties->addAttribute("bottomDelay", cBottomDelay);
}

