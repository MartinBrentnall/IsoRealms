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
  cRed   = 0.0f;
  cGreen = 0.0f;
  cBlue  = 0.0f;
  cAlpha = 1.0f;
}

Colour::Colour(IDummyModule* module, DOMNodeWrapper* node, IResourceRegistry* resourceRegistry) {
}

Colour::Colour(const IColour& colour) {
  cRed   = colour.getRed();
  cGreen = colour.getGreen();
  cBlue  = colour.getBlue();
  cAlpha = colour.getAlpha();
}

void Colour::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  cRed   = node->getFloatAttribute("red");
  cGreen = node->getFloatAttribute("green");
  cBlue  = node->getFloatAttribute("blue");
  cAlpha = node->getFloatAttribute("alpha");
  resources->notifyChange(this);
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

int Colour::getIntRed() const {
  return (int) (cRed * 255.0);
}

int Colour::getIntGreen() const {
  return (int) (cGreen * 255.0);
}

int Colour::getIntBlue() const {
  return (int) (cBlue * 255.0);
}

int Colour::getIntAlpha() const {
  return (int) (cAlpha * 255.0);
}

Colour::Colour(IColour* a, IColour* b) {
  cRed   = (a->getRed()   + b->getRed())   / 2.0f;
  cGreen = (a->getGreen() + b->getGreen()) / 2.0f;
  cBlue  = (a->getBlue()  + b->getBlue())  / 2.0f;
  cAlpha = (a->getAlpha() + b->getAlpha()) / 2.0f;
}

Colour::Colour(IColour* a, IColour* b, float weight) {
  cRed   = a->getRed()   + (b->getRed()   - a->getRed())   * weight;
  cGreen = a->getGreen() + (b->getGreen() - a->getGreen()) * weight;
  cBlue  = a->getBlue()  + (b->getBlue()  - a->getBlue())  * weight;
  cAlpha = a->getAlpha() + (b->getAlpha() - a->getAlpha()) * weight;
}

Colour::Colour(IColour* colour, float intensity) {
  cRed   = colour->getRed()   * intensity;
  cGreen = colour->getGreen() * intensity;
  cBlue  = colour->getBlue()  * intensity;
  cAlpha = colour->getAlpha();
}

void Colour::set() {
  glColor3f(cRed, cGreen, cBlue);
}

void Colour::setBackground() {
  float fogColour[4] = {cRed, cGreen, cBlue, 0.0f};
  glClearColor(cRed, cGreen, cBlue, 0.0f);
  glFogfv(GL_FOG_COLOR, fogColour);
}

float Colour::getRed() const {
  return cRed;
}

float Colour::getGreen() const {
  return cGreen;
}

float Colour::getBlue() const {
  return cBlue;
}

float Colour::getAlpha() const {
  return cAlpha;
}

void Colour::setRed(float red) {
  cRed = red;
}

void Colour::setGreen(float green) {
  cGreen = green;
}

void Colour::setBlue(float blue) {
  cBlue = blue;
}

void Colour::setAlpha(float alpha) {
  cAlpha = alpha;
}

float Colour::luminance() {
  return cRed * 0.2126f + cGreen * 0.7152 + cBlue * 0.0722;
}

void Colour::debug() {
  std::cout << "  " << cRed << " " << cGreen << " " << cBlue << std::endl;
}

void Colour::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
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

void Colour::saveCache(DOMNodeWriter* cache) {
  // Nothing to do
}
