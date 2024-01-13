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

#include "IsoRealms/IAssetBrowser.h"
#include "IsoRealms/IAssetIdentifier.h"
#include "IsoRealms/Types.h"

namespace IsoRealms {
  class IEditableScreen;
  class IPropertyAppearance;
  
  class IProperty {
    public:
    virtual std::string getPropertyName() const = 0;
    virtual bool updateEditing(unsigned int milliseconds) = 0;
    virtual void render(IPropertyAppearance* appearance) const = 0;
    virtual void renderEditing(IPropertyAppearance* appearance) const = 0;
    virtual IEditable* edit(IAssetRegistry* assets) = 0;
    virtual void close() = 0;
    virtual float getWidth(IPropertyAppearance* appearance) const = 0;
    virtual bool input(sf::Event& event) = 0;
    
    virtual ~IProperty() {}
  };
}
