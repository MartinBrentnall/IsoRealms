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
  // TODO
}

void SpindizzyZoneThemeColour::set(IColour* colour) {
  cColour = colour;
}

float SpindizzyZoneThemeColour::getRed() const {
  return cColour->getRed();
}

float SpindizzyZoneThemeColour::getGreen() const {
  return cColour->getGreen();
}

float SpindizzyZoneThemeColour::getBlue() const {
  return cColour->getBlue();
}

float SpindizzyZoneThemeColour::getAlpha() const {
  return cColour->getAlpha();
}

int SpindizzyZoneThemeColour::getIntRed() const {
  return cColour->getIntRed();
} 

int SpindizzyZoneThemeColour::getIntGreen() const {
  return cColour->getIntGreen();
}

int SpindizzyZoneThemeColour::getIntBlue() const {
  return cColour->getIntBlue();
}

int SpindizzyZoneThemeColour::getIntAlpha() const {
  return cColour->getIntAlpha();
}

float SpindizzyZoneThemeColour::luminance() {
  return cColour->luminance();
}

void SpindizzyZoneThemeColour::set() {
  cColour->set();
}

void SpindizzyZoneThemeColour::setBackground() {
  cColour->setBackground();
}


