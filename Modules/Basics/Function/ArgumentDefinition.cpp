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
            cDefType(parent.getComponentData()) {
  }

  void ArgumentDefinition::define(IComponentDefiner& definer, Function& parent) {
    definer.propertyString(  "name",    cDefName, "", [this, &parent](const std::string& value) {return parent.isArgumentDefinitionNameAllowed(*this, value);});
    definer.propertyResource("type",    cDefType);
    definer.propertyString(  "luaName", cDefLuaName);
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
