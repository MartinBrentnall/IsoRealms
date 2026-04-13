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
#pragma once

#include <string>

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Basics;
  class Function;

  class ArgumentDefinition final {
    public:
    ArgumentDefinition(Function& parent, const std::string& name, const std::string& luaName);
    ArgumentDefinition(Function& parent, JSONObject object);
    void save(JSONObject object) const;
    void setName(const std::string& name);
    std::string getName() const;
    const BindingType* getType() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata, Function& parent);
    void saveCall(JSONObject object, const std::string& attributeName) const;
    std::string getInitCode() const;
    std::string getCode(unsigned int functionID, unsigned int arg) const;
    std::string getCleanup() const;

    private:
    Function& cParent;
    std::string cDefName;
    std::string cDefLuaName;
    BindingType cDefType;

    static const std::string JSON_LUA_NAME;
    static const std::string JSON_NAME;
    static const std::string JSON_TYPE;
  };
}
