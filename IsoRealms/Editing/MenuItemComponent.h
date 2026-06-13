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

#include "IsoRealms/IComponent.h"
#include "IsoRealms/Types.h"

#include "UISignalID.h"

namespace IsoRealms {
  class MenuItemComponent {
    public:
    MenuItemComponent(std::variant<IComponent*, std::string> component, std::function<void(IComponent* component)> action, std::function<bool(IComponent* component)> icon);

    IComponent* getComponent() const;
    float getWidth(IUIStyle& style) const;
    void render(IUIStyle& style, float y, float xRemoveOffset, float aspectRatio) const;
    bool input(UISignalID id);
    bool isComponent() const;
    bool isAddComponent() const;
    bool isPlaceHolder() const;
    std::string getTooltip() const;
    std::string getLabel() const;

    private:
    std::variant<IComponent*, std::string> cComponent;
    std::function<void(IComponent*)> cAction;
    std::function<bool(IComponent*)> cIcon;
  };
}

