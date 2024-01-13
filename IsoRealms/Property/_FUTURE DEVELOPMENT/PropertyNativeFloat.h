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

#include <cmath>
#include <functional>
#include <string>

#include "IsoRealms/Utils.h"

#include "PropertyInputField.h"

namespace IsoRealms {
  class PropertyNativeFloat : public PropertyInputField {
    private:
    bool cAllowNone;
    float* cFloatValue;
    std::function<void(const float)> cAddCallback;
    std::function<void()> cRemoveCallback;
    
    public:
    PropertyNativeFloat(const std::string& name, float* value, bool allowNone = false, std::function<void()> removeCallback = []() {});
    PropertyNativeFloat(const std::string& name, std::function<void(const float)> addCallback);

    protected:
    
    /********************************\
     * Implements IProperty *
    \********************************/
    bool isKeyAllowed(SDL_keysym& keysym) override;
    bool confirmValue() override;
  };
}
