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

#include <functional>
#include <memory>
#include <vector>
#include <string>

#include "IsoRealms/IResourceData.h"

namespace IsoRealms {
  class IEditable;
  class IProperty;
  class IPropertyEditor;
  class IUIStyle;
  class IPropertyMaker;

  class IPropertyManager {
    public:
    virtual void addProperty(std::unique_ptr<IProperty> property) = 0;
    virtual void openProperties(IResourceData& owner, const std::string& name, std::function<void(IPropertyMaker&)> propertyFetcher) = 0;
    virtual void edit(std::unique_ptr<IPropertyEditor> editor) = 0;
    virtual void edit(IEditable* editor) = 0;
    virtual void refreshProperties() = 0;
    virtual IUIStyle& getPropertyStyle() = 0;
  };
}
