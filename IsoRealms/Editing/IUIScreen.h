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

#include <string>

#include "UISignalID.h"

namespace IsoRealms {
  class IUIScreen {
    public:
    virtual void renderBackground(float aspectRatio, float minimumWidth) = 0;
    virtual void render(float aspectRatio) = 0;
    virtual std::string getBreadCrumb() const = 0;
    virtual void setBreadCrumbColour() const = 0;
    virtual float getSelectionHighlightLeft(float aspectRatio) const = 0;
    virtual float getSelectionHighlightRight(float aspectRatio) const = 0;
    virtual float getSelectionHighlightTop() const = 0;
    virtual float getSelectionHighlightBottom() const = 0;
    virtual void update(unsigned int milliseconds) = 0;
    virtual bool input(UISignalID id) = 0;
    virtual bool input(sf::Event& event) = 0;
    virtual void refresh() = 0;
    virtual std::string getTooltip() const = 0;
    virtual float getTooltipXPosition() const = 0;

    virtual ~IUIScreen() {
    }
  };
}
