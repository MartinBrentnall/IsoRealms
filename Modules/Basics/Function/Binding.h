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
#pragma once

#include "IsoRealms/Editing.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms::Basics {
  class Function;

  class Binding {
    public:
    Binding(Function& parent, IBindingRegistry* localArgs, const std::string& name);
    Binding(Function& parent, IBindingRegistry* localArgs, bool init, JSONObject object);
    void setName(const std::string& name);
    std::string getName() const;
    std::string getType() const;
    std::string getInitCode() const;
    std::string getCode(const std::string& function, unsigned int arg) const;
    std::string getCleanup() const;
    void save(JSONObject object, const std::string& attributeName, IAssetIdentifier& identifier, const std::string& attributeValueName) const;
    void saveCall(JSONObject object, const std::string& attributeName) const;
    IBinding* getValue() const;
    void release(IAssets& releaser);
    std::vector<std::unique_ptr<IProperty>> getProperties();

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
