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
#include "BlockLocation.h"

BlockLocation::BlockLocation() {
  set(0, 0, 0);
}

BlockLocation::BlockLocation(int xPos, int yPos, int zPos) {
  set(xPos, yPos, zPos);
}

void BlockLocation::set(int xPos, int yPos, int zPos) {
  x = xPos;
  y = yPos;
  z = zPos;
}

void BlockLocation::set(DOMNodeWrapper* node) {
  x = node->getIntegerAttribute("x");
  y = node->getIntegerAttribute("y");
  z = node->getIntegerAttribute("z");
}

void BlockLocation::setRelative(DOMNodeWrapper* node, BlockLocation& relative) {
  x = node->getIntegerAttribute("x") + relative.x;
  y = node->getIntegerAttribute("y") + relative.y;
  z = node->getIntegerAttribute("z") + relative.z;
}

void BlockLocation::set(BlockLocation destination) {
  x = destination.x;
  y = destination.y;
  z = destination.z;
}

bool BlockLocation::operator==(BlockLocation& point) {
  return x == point.x && y == point.y && z == point.z;
}

void BlockLocation::save(DOMNodeWriter* node) {
  std::string mXString = Utils::toString(x);
  std::string mYString = Utils::toString(y);
  std::string mZString = Utils::toString(z);
  node->addAttribute("x", mXString);
  node->addAttribute("y", mYString);
  node->addAttribute("z", mZString);
}

void BlockLocation::saveRelative(DOMNodeWriter* node, BlockLocation& relative) {
  std::string mXString = Utils::toString(x - relative.x);
  std::string mYString = Utils::toString(y - relative.y);
  std::string mZString = Utils::toString(z - relative.z);
  node->addAttribute("x", mXString);
  node->addAttribute("y", mYString);
  node->addAttribute("z", mZString);
}

void BlockLocation::debug() {
  std::cout << x << "," << y << "," << z << std::endl;
}
