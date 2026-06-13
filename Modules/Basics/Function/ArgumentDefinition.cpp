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
#include "ArgumentDefinition.h"

#include "Function.h"

namespace IsoRealms::Basics {
  ArgumentDefinition::ArgumentDefinition(Function& parent, const std::string& name, const std::string& luaName) :
            cParent(parent),
            cDefName(name),
            cDefLuaName(luaName),
            cDefType(parent.getComponentData(), [this]() {
              std::string mNewBindingID = cDefType.getTreeItemInfo().cID;
              std::size_t mLastSeparator = mNewBindingID.rfind('/');
              if (mLastSeparator != std::string::npos) {
                mNewBindingID = mNewBindingID.substr(mLastSeparator + 1);
              }
              std::string mStrippedID;
              for (unsigned int i = 0; i < mNewBindingID.length(); i++) {
                char mChar = std::toupper(mNewBindingID[i]);
                if ((mChar >= 'A' && mChar <= 'Z') || (mChar >= '0' && mChar <= '9')) {
                  mStrippedID += (mStrippedID.empty() ? std::tolower(mNewBindingID[i]) : mNewBindingID[i]);
                }
              }
              cParent.setArgumentDefinitionName(*this, mStrippedID);
            }) {
  }

  ArgumentDefinition::ArgumentDefinition(Function& parent, JSONObject object) :
            ArgumentDefinition(parent, object.getString(JSON_NAME), object.getString(JSON_LUA_NAME)) {
    cDefType.init(object, JSON_TYPE);
  }

  void ArgumentDefinition::save(JSONObject object) const {
    object.addString(JSON_NAME, cDefName);
    object.addString(JSON_LUA_NAME, cDefLuaName);
    cDefType.save(object, JSON_TYPE);
  }

  void ArgumentDefinition::setName(const std::string& name) {
    cDefName = name;
  }

  std::string ArgumentDefinition::getName() const {
    return cDefName;
  }

  const BindingType* ArgumentDefinition::getType() const {
    return &cDefType;
  }

  void ArgumentDefinition::getProperties(IPropertyMaker& owner, const Metadata& metadata, Function& parent) {
    owner.createPropertyNativeString(JSON_NAME,     [this]() {return cDefName;}, [this](const std::string& value) {cDefName = value;}, "", [this, &parent](const std::string& value) {return parent.isArgumentDefinitionNameAllowed(*this, value);});
    owner.createPropertyTreeSelector(JSON_TYPE,     cDefType);
    owner.createPropertyNativeString(JSON_LUA_NAME, [this]() {return cDefLuaName;}, [this](const std::string& value) {cDefLuaName = value;});
  }

  void ArgumentDefinition::saveCall(JSONObject object, const std::string& attributeName) const {
    object.addString(attributeName, cDefName);
  }

  std::string ArgumentDefinition::getInitCode() const {
    return "_" + cDefLuaName + " = {}\n";;
  }

  std::string ArgumentDefinition::getCode(unsigned int functionID, unsigned int arg) const {
    std::string mFunction = "function func" + Utils::toString(functionID) + "_arg" + Utils::toString(arg) + "(arg)\n";
    mFunction += "  table.insert(_" + cDefLuaName + ", " + cDefLuaName + ")\n";
    mFunction += "  " + cDefLuaName + " = arg\n";
    mFunction += "end\n";
    mFunction += "\n";
    return mFunction;
  }

  std::string ArgumentDefinition::getCleanup() const {
    std::string mFunction = "  " + cDefLuaName + " = _" + cDefLuaName + "[#_" + cDefLuaName + "]\n";
    mFunction += "  table.remove(_" + cDefLuaName + ")\n";
    return mFunction;
  }
}

