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
#include "AbstractRectangularComponent.h"

AbstractRectangularComponent::AbstractRectangularComponent() {
  setBoundsCalculator(NULL);
}

AbstractRectangularComponent::AbstractRectangularComponent(IComponentBoundsCalculator* componentBoundsCalculator) {
  setBoundsCalculator(componentBoundsCalculator);
}

void AbstractRectangularComponent::setBoundsCalculator(IComponentBoundsCalculator* componentBoundsCalculator) {
  cComponentBoundsCalculator = componentBoundsCalculator;
}

float AbstractRectangularComponent::getLeft() {
  return cComponentBoundsCalculator != NULL ? cComponentBoundsCalculator->getLeft() : 0.0f;
}

float AbstractRectangularComponent::getTop() {
  return cComponentBoundsCalculator != NULL ? cComponentBoundsCalculator->getTop() : 0.0f;
}

float AbstractRectangularComponent::getRight() {
  return cComponentBoundsCalculator != NULL ? cComponentBoundsCalculator->getRight() : 0.0f;
}

float AbstractRectangularComponent::getBottom() {
  return cComponentBoundsCalculator != NULL ? cComponentBoundsCalculator->getBottom() : 0.0f;
}

bool AbstractRectangularComponent::contains(float x, float y) {
  return x >= getLeft() && x <= getRight() && y >= getBottom() && y <= getTop();
}

void AbstractRectangularComponent::gainedFocus() {
  // Nothing to do
}

void AbstractRectangularComponent::lostFocus() {
  // Nothing to do
}

AbstractRectangularComponent::~AbstractRectangularComponent() {
  delete cComponentBoundsCalculator;
}
