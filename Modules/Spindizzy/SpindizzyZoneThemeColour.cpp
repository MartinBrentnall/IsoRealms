/*
 * Copyright 2015 Martin Brentnall
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
#include "SpindizzyZoneThemeColour.h" 

void SpindizzyZoneThemeColour::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cColour = nullptr;
}

void SpindizzyZoneThemeColour::set(IColour* colour) {
  cColour = colour;
}

float SpindizzyZoneThemeColour::getRed() const {
  return cColour != nullptr ? cColour->getRed() : 0.0f;
}

float SpindizzyZoneThemeColour::getGreen() const {
  return cColour != nullptr ? cColour->getGreen() : 0.0f;
}

float SpindizzyZoneThemeColour::getBlue() const {
  return cColour != nullptr ? cColour->getBlue() : 0.0f;
}

float SpindizzyZoneThemeColour::getAlpha() const {
  return cColour != nullptr ? cColour->getAlpha() : 0.0f;
}

int SpindizzyZoneThemeColour::getIntRed() const {
  return cColour != nullptr ? cColour->getIntRed() : 0.0f;
} 

int SpindizzyZoneThemeColour::getIntGreen() const {
  return cColour != nullptr ? cColour->getIntGreen() : 0.0f;
}

int SpindizzyZoneThemeColour::getIntBlue() const {
  return cColour != nullptr ? cColour->getIntBlue() : 0.0f;
}

int SpindizzyZoneThemeColour::getIntAlpha() const {
  return cColour != nullptr ? cColour->getIntAlpha() : 0.0f;
}

float SpindizzyZoneThemeColour::luminance() {
  return cColour != nullptr ? cColour->luminance() : 0.0f;
}

void SpindizzyZoneThemeColour::set() {
  if (cColour != nullptr) {
    cColour->set();
  } else {
    glColor3f(0.0f, 0.0f, 0.0f);
  }
}

void SpindizzyZoneThemeColour::setBackground() {
  if (cColour != nullptr) {
    cColour->setBackground();
  }
}


