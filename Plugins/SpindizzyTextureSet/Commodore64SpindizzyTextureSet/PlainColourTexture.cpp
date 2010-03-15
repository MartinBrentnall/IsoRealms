#include "PlainColourTexture.h"

PlainColourTexture::PlainColourTexture(Colour* colour) {
  cColour = colour;
}

void PlainColourTexture::set() {
  glColor3f(cColour->getRed(), cColour->getGreen(), cColour->getBlue());
}

void PlainColourTexture::texCoord2f(float, float) {
  // Nothing to do
}

Colour* PlainColourTexture::getColour(float, float) {
  return cColour;
}

ISpindizzyTexture::Mapping PlainColourTexture::getMapping() {
  return PLAIN_COLOUR;
}
