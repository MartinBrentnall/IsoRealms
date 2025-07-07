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
    ArgumentDefinition(IProject& project, Function& parent, const std::string& name);
    ArgumentDefinition(IProject& project, Function& parent, JSONObject object);
    void save(JSONObject object) const;
    void setName(const std::string& name);
    std::string getName() const;
    std::string getType() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(Function& parent);
    void saveCall(JSONObject object, const std::string& attributeName) const;
    std::string getInitCode() const;
    std::string getCode(const std::string& function, unsigned int arg) const;
    std::string getCleanup() const;

    private:
    Function& cParent;
    std::string cDefName;
    BindingType cDefType;

    static const std::string JSON_NAME;
    static const std::string JSON_TYPE;
  };
}
