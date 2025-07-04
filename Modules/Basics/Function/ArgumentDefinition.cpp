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
#include "ArgumentDefinition.h"

#include "IsoRealms/Project.h"

#include "Function.h"

namespace IsoRealms::Basics {
  ArgumentDefinition::ArgumentDefinition(IProject& project, Function& parent, const std::string& name) :
            cParent(parent),
            cDefName(name),
            cDefType(parent.getResourceData(), [this]() {
              std::string mNewBindingID = cDefType.getID();
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

  ArgumentDefinition::ArgumentDefinition(IProject& project, Function& parent, JSONObject object) :
            ArgumentDefinition(project, parent, object.getString(JSON_NAME)) {
    cDefType.init(object, JSON_TYPE);
  }

  void ArgumentDefinition::save(JSONObject object) const {
    object.addString(JSON_NAME, cDefName);
    cDefType.save(object, JSON_TYPE);
  }

  void ArgumentDefinition::setName(const std::string& name) {
    cDefName = name;
  }

  std::string ArgumentDefinition::getName() const {
    return cDefName;
  }

  std::string ArgumentDefinition::getType() const {
    return cDefType->getBindingTypeID();
  }

  std::vector<std::unique_ptr<IProperty>> ArgumentDefinition::getProperties(Function& parent) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<BindingType>>("Type", "TODO", cDefType));
    mProperties.emplace_back(std::make_unique<PropertyNativeString>(      "Name", "TODO", [this]() {return cDefName;}, [this, &parent](const std::string& value) {return parent.setArgumentDefinitionName(*this, value);}));
    return mProperties;
  }

  void ArgumentDefinition::saveCall(JSONObject object, const std::string& attributeName) const {
    object.addString(attributeName, cDefName);
  }

  std::string ArgumentDefinition::getInitCode() const {
    return "_" + cDefName + " = {}\n";;
  }

  std::string ArgumentDefinition::getCode(const std::string& function, unsigned int arg) const {
    std::string mFunction = "function " + function + "_arg" + Utils::toString(arg) + "(arg)\n";
    mFunction += "  table.insert(_" + cDefName + ", " + cDefName + ")\n";
    mFunction += "  " + cDefName + " = arg\n";
    mFunction += "end\n";
    mFunction += "\n";
    return mFunction;
  }

  std::string ArgumentDefinition::getCleanup() const {
    std::string mFunction = "  " + cDefName + " = _" + cDefName + "[#_" + cDefName + "]\n";
    mFunction += "  table.remove(_" + cDefName + ")\n";
    return mFunction;
  }

  const std::string ArgumentDefinition::JSON_NAME = "name";
  const std::string ArgumentDefinition::JSON_TYPE = "type";
}

