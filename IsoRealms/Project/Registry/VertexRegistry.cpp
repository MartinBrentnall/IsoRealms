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
#include "VertexRegistry.h" 

namespace IsoRealms {
  VertexRegistry::VertexRegistry() :
            ResourceClientManager(&cLiteral, "Literal", "Literal") {
  }

  VertexRegistry::Literal::Instance::Instance() {
  }

  VertexRegistry::Literal::Instance::Instance(double xPos, double yPos, double zPos) :
            x(xPos),
            y(yPos),
            z(zPos) {
  }

  double VertexRegistry::Literal::Instance::getX() const {
    return x;
  }

  double VertexRegistry::Literal::Instance::getY() const {
    return y;
  }

  double VertexRegistry::Literal::Instance::getZ() const {
    return z;
  }

  void VertexRegistry::Literal::Instance::saveResource(JSONObject object) const {
    object.addFloat(JSON_X, x);
    object.addFloat(JSON_Y, y);
    object.addFloat(JSON_Z, z);
  }

  bool VertexRegistry::Literal::Instance::isDefaultConfiguration() const {
    return x == 0.0f && y == 0.0f && z == 0.0f;
  }
}
