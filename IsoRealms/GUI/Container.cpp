/*
 * Copyright 2015 Martin Brentnall
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
#include "Container.h"

Container::Container() {
  cComponent = NULL;
}

void Container::render() {
  if (cComponent != NULL) {
    cComponent->render();
  }
}

void Container::update(unsigned int milliseconds) {
  if (cComponent != NULL) {
    cComponent->update(milliseconds);
  }
}

bool Container::input(SDL_Event& event) {
  return cComponent != NULL && cComponent->input(event);
}

float Container::getWidth() {
  return getRight() - getLeft();
}

float Container::getHeight() {
  return getTop() - getBottom();
}
  
void Container::addComponent(const std::string& name, ISizedComponent* component) {
  cComponent = component;
  cComponent->setBoundsCalculator(new ContainerBounds(this));
}
 
void Container::removeComponent(ISizedComponent* component) {
}

Container::ContainerBounds::ContainerBounds(Container* container) {
  cParent = container;
}

float Container::ContainerBounds::getBottom() {
  return cParent->getBottom();
}

float Container::ContainerBounds::getLeft() {
  return cParent->getLeft();
}

float Container::ContainerBounds::getRight() {
  return cParent->getRight();
}

float Container::ContainerBounds::getTop() {
  return cParent->getTop();
}


