/*
 * Copyright 2009 Martin Brentnall
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
#include "Colour.h"

Colour::Colour() {
  cRed = 0.0;
  cGreen = 0.0;
  cBlue = 0.0;
  cAlpha = 0.0;
}

Colour::Colour(DOMNodeWrapper* node) {
  
  cRed = node->getFloatAttribute("red");
  cGreen = node->getFloatAttribute("green");
  cBlue = node->getFloatAttribute("blue");
  cAlpha = node->getFloatAttribute("alpha");
}

Colour::Colour(float r, float g, float b, float a) {
  set(r, g, b, a);
}

Colour::Colour(float r, float g, float b) {
  set(r, g, b, 0.0);
}

void Colour::set(float r, float g, float b, float a) {
  cRed = r;
  cGreen = g;
  cBlue = b;
  cAlpha = a;
}

void Colour::change(float r, float g, float b, float a) {
  cRed += r;
  cGreen += g;
  cBlue += b;
  cAlpha += a;

  cRed =   restrict(0.0, 1.0, cRed);
  cGreen = restrict(0.0, 1.0, cGreen);
  cBlue =  restrict(0.0, 1.0, cBlue);
  cAlpha = restrict(0.0, 1.0, cAlpha);
}

int Colour::getAsInt(Channel channel) {
  switch (channel) {
    case RED:   return (int) (cRed * 255.0);
    case GREEN: return (int) (cGreen * 255.0);
    case BLUE:  return (int) (cBlue * 255.0);
    case ALPHA: return (int) (cAlpha * 255.0);
  }
  throw new IllegalStateException();
}

Colour::Colour(Colour a, Colour b) {
  cRed = (a.cRed + b.cRed) / 2;
  cGreen = (a.cGreen + b.cGreen) / 2;
  cBlue = (a.cBlue + b.cBlue) / 2;
  cAlpha = (a.cAlpha + b.cAlpha) / 2;
}

Colour Colour::brightness(float intensity) {
  return Colour(cRed * intensity, cGreen * intensity, cBlue * intensity, cAlpha);
}

void Colour::set() {
  glColor3f(cRed, cGreen, cBlue);
}

void Colour::setBackground() {
  float fogColour[4] = {cRed, cGreen, cBlue, 0.0f};
  glClearColor(cRed, cGreen, cBlue, 0.0f);
  glFogfv(GL_FOG_COLOR, fogColour);
}

float Colour::getRed() {
  return cRed;
}

float Colour::getGreen() {
  return cGreen;
}

float Colour::getBlue() {
  return cBlue;
}

void Colour::debug() {
  std::cout << "  " << cRed << " " << cGreen << " " << cBlue << std::endl;
}

void Colour::save(DOMNodeWriter* node) {
  // TODO: Skip zero values
  std::string mRedString = Utils::toString(cRed);
  std::string mGreenString = Utils::toString(cGreen);
  std::string mBlueString = Utils::toString(cBlue);
  std::string mAlphaString = Utils::toString(cAlpha);
  node->addAttribute("red", mRedString);
  node->addAttribute("green", mGreenString);
  node->addAttribute("blue", mBlueString);
  node->addAttribute("alpha", mAlphaString);
}
