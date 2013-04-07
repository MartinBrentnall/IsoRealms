#include "BackgroundChanger.h"

BackgroundChanger::BackgroundChanger(IElementType* elementType) {
  cElementType = elementType;
  cProgressBackgroundColour = 1.0f;
  cPreviousBackgroundColour = new Colour(0.0f, 0.0f, 0.0f);
  cTargetBackgroundColour = new Colour(0.0f, 0.0f, 0.0f);
  cChangeSpeed = 0.0025f;
}

void BackgroundChanger::setChangeSpeed(float changeSpeed) {
  cChangeSpeed = changeSpeed;
}

void BackgroundChanger::setColour(IColour* colour) {
  float mRed   = cPreviousBackgroundColour->getRed()   + (cTargetBackgroundColour->getRed()   - cPreviousBackgroundColour->getRed())   * cProgressBackgroundColour;
  float mGreen = cPreviousBackgroundColour->getGreen() + (cTargetBackgroundColour->getGreen() - cPreviousBackgroundColour->getGreen()) * cProgressBackgroundColour;
  float mBlue  = cPreviousBackgroundColour->getBlue()  + (cTargetBackgroundColour->getBlue()  - cPreviousBackgroundColour->getBlue())  * cProgressBackgroundColour;
  cPreviousBackgroundColour = new Colour(mRed, mGreen, mBlue);
  cTargetBackgroundColour = colour;
  cProgressBackgroundColour = 0.0f;
}

IPlugin* BackgroundChanger::getElementSet() {
  return NULL;
}

IElementType* BackgroundChanger::getElementType() {
  return cElementType;
}

bool BackgroundChanger::initElement(unsigned int) {
  return true;
}

void BackgroundChanger::renderStatic() {
  // Nothing to do
}

void BackgroundChanger::renderRuntime() {
  float mRed   = cPreviousBackgroundColour->getRed()   + (cTargetBackgroundColour->getRed()   - cPreviousBackgroundColour->getRed())   * cProgressBackgroundColour;
  float mGreen = cPreviousBackgroundColour->getGreen() + (cTargetBackgroundColour->getGreen() - cPreviousBackgroundColour->getGreen()) * cProgressBackgroundColour;
  float mBlue  = cPreviousBackgroundColour->getBlue()  + (cTargetBackgroundColour->getBlue()  - cPreviousBackgroundColour->getBlue())  * cProgressBackgroundColour;
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_QUADS);
  glColor3f(mRed, mGreen, mBlue); glVertex3f(-1.0f, -1.0f, 0.9f);
  glColor3f(mRed, mGreen, mBlue); glVertex3f( 1.0f, -1.0f, 0.9f);
  glColor3f(mRed, mGreen, mBlue); glVertex3f( 1.0f,  1.0f, 0.9f);
  glColor3f(mRed, mGreen, mBlue); glVertex3f(-1.0f,  1.0f, 0.9f);
  glEnd();
}

void BackgroundChanger::renderEditing() {
  // Nothing to do
}

void BackgroundChanger::updateRuntime(unsigned int milliseconds) {
  if (cProgressBackgroundColour < 1.0f) {
    cProgressBackgroundColour += cChangeSpeed * milliseconds;
    if (cProgressBackgroundColour > 1.0f) {
      cProgressBackgroundColour = 1.0f;
    }
  }
}

void BackgroundChanger::updateEditing(unsigned int milliseconds) {
  // Nothing to do
}

void BackgroundChanger::input(SDL_Event&) {
  // Nothing to do
}

bool BackgroundChanger::isVisualRuntime() {
  return true;
}

bool BackgroundChanger::isVisualEditing() {
  return false;
}

bool BackgroundChanger::isDynamicRuntime() {
  return true;
}

bool BackgroundChanger::isDynamicEditing() {
  return false;
}

bool BackgroundChanger::isInteractive() {
  return false;
}

void BackgroundChanger::save(DOMNodeWriter*, IResourceLocator*, BlockLocation&) {
  // TODO
}

void BackgroundChanger::setDirty() {
  // Nothing to do
}
