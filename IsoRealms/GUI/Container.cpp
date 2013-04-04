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


