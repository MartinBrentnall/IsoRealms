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
#include <string>
#include <variant>

#include "IsoRealms/IResource.h"
#include "IsoRealms/Types.h"

#include "UISignalID.h"

namespace IsoRealms {
  class MenuItemResource {
    public:
    MenuItemResource(std::variant<IResource*, std::string> resource, std::function<void(IResource* resource)> action, std::function<bool(IResource* resource)> icon);

    IResource* getResource() const;
    float getWidth(IUIStyle& style) const;
    void render(IUIStyle& style, float y, float xRemoveOffset, float aspectRatio) const;
    bool input(UISignalID id);
    bool isResource() const;
    bool isAddResource() const;
    bool isPlaceHolder() const;
    std::string getTooltip() const;
    std::string getLabel() const;

    private:
    std::variant<IResource*, std::string> cResource;
    std::function<void(IResource*)> cAction;
    std::function<bool(IResource*)> cIcon;
  };
}

