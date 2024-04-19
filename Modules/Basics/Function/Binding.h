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

#include "IsoRealms/IProject.h"
#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms::Basics {
  class Binding {
    public:
    Binding(DOMNode& node, const std::string& nameAttribute, const std::string& valueTag, IProject* project, IBindingRegistry* localArgs, bool init);
    std::string getName() const;
    std::string getInitCode() const;
    std::string getCode(const std::string& function, unsigned int arg) const;
    std::string getCleanup() const;
    void save(DOMNodeWriter* node, const std::string& attributeName, IAssetIdentifier* identifier, bool local, const std::string& attributeValueName) const;
    void saveCall(DOMNodeWriter* node, const std::string& attributeName) const;
    IBinding* getValue() const;
    void release(IAssets* releaser);

    private:
    
    // Definition data.
    std::string cDefName;         /// Name of this binding.
    IsoRealms::Binding cDefValue; /// Value bound.
  };
}
