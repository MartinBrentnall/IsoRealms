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

#include <limits>
#include <functional>
#include <string>

#include <GL/glew.h>

#include "IsoRealms/Utils.h"

#include "IPropertyManager.h"
#include "Property.h"

namespace IsoRealms {
  class IPropertyManager;
  
  class PropertyStruct : public Property {
    public:
    PropertyStruct(const std::string& name, const std::string& tooltip, const std::string& value, std::function<std::vector<std::unique_ptr<IProperty>>()> subProperties, std::function<void()> removeFunction = nullptr);
    
    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override;
    float getValueWidth(IUIStyle& style) const override;
    void confirm(IPropertyManager& manager, float y) override;
    bool hasConfiguration() const override;
    void configure(IPropertyManager& manager) override;

    private:
    std::function<std::vector<std::unique_ptr<IProperty>>()> cSubProperties;
    std::string cValue;
  };
}
