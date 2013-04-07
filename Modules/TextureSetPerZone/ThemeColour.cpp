#include "ThemeColour.h" 

void ThemeColour::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  // TODO
}

void ThemeColour::set(IColour* colour) {
  cColour = colour;
}

float ThemeColour::getRed() const {
  return cColour->getRed();
}

float ThemeColour::getGreen() const {
  return cColour->getGreen();
}

float ThemeColour::getBlue() const {
  return cColour->getBlue();
}

float ThemeColour::getAlpha() const {
  return cColour->getAlpha();
}

int ThemeColour::getIntRed() const {
  return cColour->getIntRed();
} 

int ThemeColour::getIntGreen() const {
  return cColour->getIntGreen();
}

int ThemeColour::getIntBlue() const {
  return cColour->getIntBlue();
}

int ThemeColour::getIntAlpha() const {
  return cColour->getIntAlpha();
}

float ThemeColour::luminance() {
  return cColour->luminance();
}

void ThemeColour::set() {
  cColour->set();
}

void ThemeColour::setBackground() {
  cColour->setBackground();
}


