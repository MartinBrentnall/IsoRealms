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
#include "SimpleVertex.h"

namespace IsoRealms::Basics {
  SimpleVertex::SimpleVertex(Basics& basics, IComponentData& data) :
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  void SimpleVertex::define(IComponentDefiner& definer) {
    definer.propertyFloat("x", cDefX);
    definer.propertyFloat("y", cDefY);
    definer.propertyFloat("z", cDefZ);
  }

  void SimpleVertex::publish(ResourcePublisher& publisher) {
    cStateNotifier = publisher.publish<IVertex>(this, "", "Simple Vertices");
    publisher.publish<IBinding>(&cLuaBinding, "", "Variables/Vertices");
  }

  void SimpleVertex::reset() {
    cRuntimeX = cDefX;
    cRuntimeY = cDefY;
    cRuntimeZ = cDefZ;
  }
  
  double SimpleVertex::getX() const {
    return cRuntimeX;
  }

  double SimpleVertex::getY() const {
    return cRuntimeY;
  }

  double SimpleVertex::getZ() const {
    return cRuntimeZ;
  }

  void SimpleVertex::setX(double x) {
    cRuntimeX = x;
    cStateNotifier->stateChanged();
  }

  void SimpleVertex::setY(double y) {
    cRuntimeY = y;
    cStateNotifier->stateChanged();
  }

  void SimpleVertex::setZ(double z) {
    cRuntimeZ = z;
    cStateNotifier->stateChanged();
  }
}
