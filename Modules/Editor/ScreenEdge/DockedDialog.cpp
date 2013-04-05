#include "DockedDialog.h"

DockedDialog::DockedDialog(Dialog* dialog, AbstractRectangularComponent* icon) {
  cDialog = dialog;
  cIcon = icon;
}

bool DockedDialog::input(SDL_Event& event) {
  return cDialog->input(event);
}

void DockedDialog::update(unsigned int milliseconds) {
  cDialog->update(milliseconds);
  cIcon->update(milliseconds);
}

void DockedDialog::render() {
  cDialog->render();
}

void DockedDialog::renderIcon() {
  cIcon->render();
}

bool DockedDialog::contains(float x, float y) {
  return cDialog->contains(x, y);
}

float DockedDialog::getHeight() {
  return 0.8f;
}

float DockedDialog::getWidth() {
  return 0.8f;
}

void DockedDialog::setSize(float left, float bottom, float right, float top) {
  cDialog->setSize(left, bottom, right, top);
}

