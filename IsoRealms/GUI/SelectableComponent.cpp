#include "SelectableComponent.h"

SelectableComponent::SelectableComponent() {
  cHandler = NULL;
  cSelected = false;
}

void SelectableComponent::setHandler(ISelector* handler) {
  cHandler = handler;
}

void SelectableComponent::update(unsigned int) {
}

void SelectableComponent::render() {
  float mLeft = getLeft();
  float mRight = getRight();
  float mTop = getTop();
  float mBottom = getBottom();
  glBindTexture(GL_TEXTURE_2D, 0);
  if (cHandler != NULL) {
    cHandler->render(this);
  }
  if (cSelected) {
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(mLeft,  mBottom);
    glVertex2f(mRight, mBottom);
    glVertex2f(mRight, mTop);
    glVertex2f(mLeft,  mTop);
    glEnd();
    glLineWidth(1.0f);
  }  
}

bool SelectableComponent::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      Configuration* mConfiguration = Configuration::getInstance();
      ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
      float mX = mScreen->getXLocation(event.button.x);
      float mY = mScreen->getYLocation(event.button.y);
      if (contains(mX, mY)) {
//	gainedFocus();
        return true;
      }
    }
  }
  return false;
}

void SelectableComponent::gainedFocus() {
  if (!cSelected && cHandler != NULL) {
    cSelected = true;
    cHandler->selected();
  }
}

void SelectableComponent::lostFocus() {
  if (cSelected && cHandler != NULL) {
    cSelected = false;
    cHandler->deselected();
  }
}

float SelectableComponent::getWidth() {
  return 0.3f;
}

float SelectableComponent::getHeight() {
  return 0.05f;
}

