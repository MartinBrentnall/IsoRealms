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
#include <iostream>
#include <string>

#include <GL/glew.h>

#include "PropertyInputField.h"

namespace IsoRealms {
  class PropertyNativeString : public PropertyInputField {
    public:
    PropertyNativeString(const PropertyData& data, IResourceAccessManager& resourceAccessManager, std::function<std::string()> getter, std::function<void(const std::string&)> setter, std::function<bool(const std::string&)> validityChecker, std::function<void()> removeFunction = nullptr, std::function<void(std::function<void()>, std::function<void()>)> confirmCustom = nullptr);
      
    protected:
    
    /*********************************\
     * Implements PropertyInputField *
    \*********************************/
    bool isKeyAllowed(char character, unsigned int caret) override;
    bool confirmValue() override;

    private:
    std::function<bool(const std::string&)> cValidityChecker;
    std::function<void(const std::string&)> cSetter;
    std::function<void(std::function<void()>, std::function<void()>)> cConfirmCustom;
  };
}
