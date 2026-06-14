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
#include "SimpleBoolean.h"

namespace IsoRealms::Basics {
  SimpleBoolean::SimpleBoolean(Basics& basics, IComponentData& data) :
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  void SimpleBoolean::define(IComponentDefiner& definer) {
    definer.propertyBoolean("value", [this]() {return cDefValue;}, [this](bool value) {cDefValue = value;});
  }

  void SimpleBoolean::publish(ResourcePublisher& publisher) {
    cStateNotifier = publisher.publish<IBoolean>(this, "", "Simple Booleans");
    publisher.publish<IBinding>(&cLuaBinding, "", "Simple Booleans");
  }

  void SimpleBoolean::reset() {
    cRuntimeValue = cDefValue;
  }
    
  bool SimpleBoolean::getValue() const {
    return cRuntimeValue;
  }

  void SimpleBoolean::setValue(bool value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged();
  }
}
