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
#include "ComponentEdgeLayout.h"

ComponentEdgeLayout::ComponentEdgeLayout(EdgeRelation* top, EdgeRelation* left, EdgeRelation* bottom, EdgeRelation* right, IComponentSizeCalculator* sizeCalculator) {
  if ((top == NULL && bottom == NULL) || (left == NULL && right == NULL) || (sizeCalculator == NULL && (top == NULL || bottom == NULL || left == NULL || right == NULL))) {
    // TODO: Throw
    std::cout << "Constructor arguments describe ambiguous edge relations." << std::endl;
    exit(1);
  }
  cTop = top;
  cLeft = left;
  cBottom = bottom;
  cRight = right;
  cSizeCalculator = sizeCalculator;
}

float ComponentEdgeLayout::getLeft() {
  return cLeft != NULL ? cLeft->getLeft() : cRight->getRight() - cSizeCalculator->getWidth();
}

float ComponentEdgeLayout::getRight() {
  return cRight != NULL ? cRight->getRight() : cLeft->getLeft() + cSizeCalculator->getWidth();
}

float ComponentEdgeLayout::getTop() {
  return cTop != NULL ? cTop->getTop() : cBottom->getBottom() + cSizeCalculator->getHeight();
}

float ComponentEdgeLayout::getBottom() {
  return cBottom != NULL ? cBottom->getBottom() : cTop->getTop() - cSizeCalculator->getHeight();
}

ComponentEdgeLayout::~ComponentEdgeLayout() {
  // TODO: Who is responsible for deleting all thes edge relations?
}
