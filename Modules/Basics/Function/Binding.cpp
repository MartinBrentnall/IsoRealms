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
#include "Binding.h"

#include "IsoRealms/Project/Options.h"

#include "Function.h"

namespace IsoRealms::Basics {
  Binding::Binding(Function& parent, IActionContext& owner, const std::string& name) :
            cParent(parent),
            cDefName(name),
            cDefValue(owner) { //, [this]() {
            //   std::string mNewBindingID = cDefValue.getID();
            //   std::size_t mLastSeparator = mNewBindingID.rfind('/');
            //   if (mLastSeparator != std::string::npos) {
            //     mNewBindingID = mNewBindingID.substr(mLastSeparator + 1);
            //   }
            //   std::string mStrippedID;
            //   for (unsigned int i = 0; i < mNewBindingID.length(); i++) {
            //     char mChar = std::toupper(mNewBindingID[i]);
            //     if ((mChar >= 'A' && mChar <= 'Z') || (mChar >= '0' && mChar <= '9')) {
            //       mStrippedID += (mStrippedID.empty() ? std::tolower(mNewBindingID[i]) : mNewBindingID[i]);
            //     }
            //   }
            //   cParent.setBindingName(*this, mStrippedID);
            // }) {
  }

  void Binding::setName(const std::string& name) {
    cDefName = name;
  }

  std::string Binding::getName() const {
    return cDefName;
  }

  std::string Binding::getType() const {
    return cDefValue.getType();
  }

  std::string Binding::getInitCode() const {
    return "_" + cDefName + " = {}\n";;
  }

  std::string Binding::getCode(unsigned int functionID, unsigned int arg) const {
    std::string mFunction = "function func" + Utils::toString(functionID) + "_arg" + Utils::toString(arg) + "(arg)\n";
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

  void Binding::save(JSONObject object, const std::string& attributeName, const std::string& attributeValueName) const {
    object.addString(attributeName, cDefName);
    cDefValue.save(object, attributeValueName);
  }

  void Binding::saveCall(JSONObject object, const std::string& attributeName) const {
    object.addString(attributeName, cDefName);
  }

  IBinding* Binding::getValue() const {
    return *cDefValue;
  }

  void Binding::release() {
    // TODO: Is this necessary?
//     if (cDefValue != nullptr) {
//       releaser->release(this, cDefValue);
//     }
  }
  
  void Binding::getProperties(IComponentDefiner& definer, bool init) {
    Options mHint;
    if (!init) {
      mHint.addOption(Options::PROPERTY_IMMEDIATE, "true");
    }
    definer.propertyResource(JSON_TO, cDefValue, mHint);
    definer.propertyString(JSON_VARIABLE, [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, "", [this](const std::string& value) {return cParent.isBindingNameAllowed(*this, value);});
  }
}
