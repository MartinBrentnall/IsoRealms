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

#include <memory>
#include <vector>
#include <string>

#include "IsoRealms/IProject.h"

namespace IsoRealms {
  class IPropertyEditor;
  class IUIStyle;

  class IPropertyManager {
    public:
    virtual void addProperty(std::unique_ptr<IProperty> property) = 0;
    virtual void openProperties(const std::string& name, std::function<std::vector<std::unique_ptr<IProperty>>()> propertyFetcher) = 0;
    virtual void edit(std::unique_ptr<IPropertyEditor> editor) = 0;
    virtual void edit(IEditable* editor) = 0;
    virtual void refreshProperties() = 0;
    virtual IUIStyle& getPropertyStyle() = 0;
  };
}
