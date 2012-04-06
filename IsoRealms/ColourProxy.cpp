#include "ColourProxy.h"

void ColourProxy::setImplementation(IColour* colour) {
  cColour = colour;
}

float ColourProxy::getRed() const {
  return cColour->getRed();
}

float ColourProxy::getGreen() const {
  return cColour->getGreen();
}

float ColourProxy::getBlue() const {
  return cColour->getBlue();
}

float ColourProxy::getAlpha() const {
  return cColour->getAlpha();
}

int ColourProxy::getIntRed() const {
  return cColour->getIntRed();
}

int ColourProxy::getIntGreen() const {
  return cColour->getIntGreen();
}

int ColourProxy::getIntBlue() const {
  return cColour->getIntBlue();
}

int ColourProxy::getIntAlpha() const {
  return cColour->getIntAlpha();
}

float ColourProxy::luminance() {
  return cColour->luminance();
}

void ColourProxy::set() {
  cColour->set();
}

void ColourProxy::save(DOMNodeWriter* node, const std::string& name) {
  cColour->save(node, name);
}
