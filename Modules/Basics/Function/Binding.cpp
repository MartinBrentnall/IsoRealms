/*
 * Copyright 2023 Martin Brentnall
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
#include "Binding.h"

#include "IsoRealms/IAssets.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  Binding::Binding(JSONObject object, const std::string& nameAttribute, const std::string& valueTag, IProject* project, IBindingRegistry* localArgs, bool init) :
            cDefValue(project, localArgs) {
    cDefName = object.getString(nameAttribute);
    if (object.hasMember(valueTag)) {
      if (init) {
        cDefValue.init(object, valueTag);
      } else {
        cDefValue.set(object, valueTag);
      }
    }
  }

  std::string Binding::getName() const {
    return cDefName;
  }

  std::string Binding::getInitCode() const {
    return "_" + cDefName + " = {}\n";;
  }

  std::string Binding::getCode(const std::string& function, unsigned int arg) const {
    std::string mFunction = "function " + function + "_arg" + Utils::toString(arg) + "(arg)\n";
    mFunction += "  table.insert(_" + cDefName + ", " + cDefName + ")\n";
    mFunction += "  " + cDefName + " = arg\n";
    mFunction += "end\n";
    mFunction += "\n";
    return mFunction;
  }

  std::string Binding::getCleanup() const {
    std::string mFunction = "  " + cDefName + " = _" + cDefName + "[#_" + cDefName + "]\n";
    mFunction += "  table.remove(_" + cDefName + ")\n";
    return mFunction;
  }

  void Binding::save(JSONObject object, const std::string& attributeName, IAssetIdentifier* identifier, const std::string& attributeValueName) const {
    object.addString(attributeName, cDefName);
    cDefValue.save(object, attributeValueName);
  }

  void Binding::saveCall(JSONObject object, const std::string& attributeName) const {
    object.addString(attributeName, cDefName);
  }

  IBinding* Binding::getValue() const {
    return *cDefValue;
  }

  void Binding::release(IAssets* releaser) {
    // TODO: Is this necessary?
//     if (cDefValue != nullptr) {
//       releaser->release(this, cDefValue);
//     }
  }
}
