/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "LiteralVertex.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  LiteralVertex::LiteralVertex() {
    set(0.0, 0.0, 0.0);
  }

  void LiteralVertex::set(double xPos, double yPos, double zPos) {
    x = xPos;
    y = yPos;
    z = zPos;
  }

  void LiteralVertex::set(LiteralVertex destination) {
    x = destination.x;
    y = destination.y;
    z = destination.z;
  }

  void LiteralVertex::set(IVertex* destination) {
    x = destination->getX();
    y = destination->getY();
    z = destination->getZ();
  }

  LiteralVertex::LiteralVertex(double xPos, double yPos, double zPos) {
    set(xPos, yPos, zPos);
  }

  void LiteralVertex::translate() {
    glTranslatef(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
  }

  double LiteralVertex::getX() const {
    return x;
  }

  double LiteralVertex::getY() const {
    return y;
  }

  double LiteralVertex::getZ() const {
    return z;
  }

  bool LiteralVertex::renderAssetIcon() const {
    return false;
  }
  
  void LiteralVertex::debug() {
    std::cout << "Vertex is: " << x << ", " << y << ", " << z << std::endl;
  }

  bool LiteralVertex::isEqual(LiteralVertex* vertex) {
    return x == vertex->getX()
        && y == vertex->getY()
        && z == vertex->getZ();
  }

  void LiteralVertex::saveAsset(JSONObject object) const {
    object.addFloat(JSON_X, x);
    object.addFloat(JSON_Y, y);
    object.addFloat(JSON_Z, z);
  }

  std::vector<std::unique_ptr<IProperty>> LiteralVertex::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool LiteralVertex::isDefaultConfiguration() const {
    return x == 0.0f && y == 0.0f && z == 0.0f;
  }

  const std::string LiteralVertex::JSON_X = "x";
  const std::string LiteralVertex::JSON_Y = "y";
  const std::string LiteralVertex::JSON_Z = "z";
}
