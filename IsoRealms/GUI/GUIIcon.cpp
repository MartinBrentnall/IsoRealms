#include "GUIIcon.h"

GUIIcon::GUIIcon(I3DModel* iconModel) {
  cIconModel = iconModel;
}

void GUIIcon::update(unsigned int milliseconds) {
  cIconModel->update(milliseconds);
}

void GUIIcon::render() {
  float mTop    = getTop();
  float mBottom = getBottom();
  float mLeft   = getLeft();
  float mRight  = getRight();
  glTranslatef(mLeft + (mRight - mLeft) / 2.0f, mBottom + (mTop - mBottom) / 2.0f, 0.0f);
  glScalef(mRight - mLeft, mTop - mBottom, 1.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
  cIconModel->render();
  
}

bool GUIIcon::input(SDL_Event& event) {
  // Nothing to do
}

