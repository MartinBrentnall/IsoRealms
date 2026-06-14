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
#include "SimpleFloat.h"

namespace IsoRealms::Basics {
  SimpleFloat::SimpleFloat(Basics& basics, IComponentData& data) :
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  void SimpleFloat::define(IComponentDefiner& definer) {
    definer.propertyFloat("value", [this]() {return cDefValue;}, [this](float value) {cDefValue = value;});
  }

  void SimpleFloat::publish(ResourcePublisher& publisher) {
    cStateNotifier = publisher.publish<IFloat>(this, "", "Simple Floats");
    publisher.publish<IBinding>(&cLuaBinding, "", "Variables/Floats");
  }
  
  void SimpleFloat::reset() {
    cRuntimeValue = cDefValue;
  }

  float SimpleFloat::getValue() const {
    return cRuntimeValue;
  }

  void SimpleFloat::setValue(float value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged();
  }
}
