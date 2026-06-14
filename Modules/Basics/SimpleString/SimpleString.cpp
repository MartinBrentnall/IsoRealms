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
#include "SimpleString.h"

namespace IsoRealms::Basics {
  SimpleString::SimpleString(Basics& basics, IComponentData& data) :
            cLuaBinding(data.getProject().getLuaState(), this) {
  }

  void SimpleString::define(IComponentDefiner& definer) {
    definer.propertyString("value", [this]() {return cDefValue;}, [this](const std::string& value) {cDefValue = value;});
  }

  void SimpleString::publish(ResourcePublisher& publisher) {
    cStateNotifier = publisher.publish<IString>(this, "", "Simple Strings");
    publisher.publish<IBinding>(&cLuaBinding, "", "Variables/Strings");
  }

  void SimpleString::reset() {
    cRuntimeValue = cDefValue;
  }
  
  std::string SimpleString::getValue() const {
    return cRuntimeValue;
  }

  std::string SimpleString::getConversionPath() const {
    return "";
  }

  bool SimpleString::isConfigurable() const {
    return false;
  }

  void SimpleString::setValue(const std::string& value) {
    cRuntimeValue = value;
    cStateNotifier->stateChanged();
  }
}
