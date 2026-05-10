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

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Function;

  class Binding {
    public:
    Binding(Function& parent, IActionContext& owner, const std::string& name);
    Binding(Function& parent, IActionContext& owner, bool init, JSONObject object);
    void setName(const std::string& name);
    std::string getName() const;
    std::string getType() const;
    std::string getInitCode() const;
    std::string getCode(unsigned int functionID, unsigned int arg) const;
    std::string getCleanup() const;
    void save(JSONObject object, const std::string& attributeName, const std::string& attributeValueName) const;
    void saveCall(JSONObject object, const std::string& attributeName) const;
    IBinding* getValue() const;
    void release();
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);

    private:

    static const std::string JSON_TO;
    static const std::string JSON_VARIABLE;

    // External interfaces.
    Function& cParent;

    // Definition data.
    std::string cDefName;         /// Name of this binding.
    IsoRealms::Binding cDefValue; /// Value bound.
  };
}
