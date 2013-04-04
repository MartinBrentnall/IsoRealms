#include "PanelComponent.h"

PanelComponent::PanelComponent() {
  cRenderer = NULL;
}

void PanelComponent::setRenderer(IPanelRenderer* renderer) {
  cRenderer = renderer;
}

float PanelComponent::getWidth() {
  return 0.3f;
}

float PanelComponent::getHeight() {
  return 0.08f;
}

void PanelComponent::update(unsigned int milliseconds) {
  // Nothing to do
}

void PanelComponent::render() {
  glBindTexture(GL_TEXTURE_2D, 0);
  if (cRenderer != NULL) {
    cRenderer->render(this);
  }
  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();

  glBegin(GL_LINE_LOOP);
  glColor3f(0.45f, 0.0f, 0.9f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();
}

bool PanelComponent::input(SDL_Event& event) {
  return false;
}

