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

#include <string>

#include <SFML/Window.hpp>

#include "IsoRealms/Assets/Type/IFont.h"
#include "IsoRealms/Editing/IUIStyle.h"
#include "IsoRealms/Editing/UISignalID.h"

namespace IsoRealms {
  class IPropertyEditor {
    public:
    virtual void render(IUIStyle& style, float y, float x, float aspectRatio) const = 0;
    virtual bool update(unsigned int milliseconds) = 0;
    virtual bool input(UISignalID id, IUIStyle& style) = 0;
    virtual bool input(sf::Event& event, IUIStyle& style) = 0;

    virtual ~IPropertyEditor() {}
  };
}

