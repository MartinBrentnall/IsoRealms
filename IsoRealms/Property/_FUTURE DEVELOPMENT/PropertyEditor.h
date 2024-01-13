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

#include <GL/glew.h>

#include "IsoRealms/Property/Property.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms::Spindizzy {
  class PropertyEditor : public Property {
    private:
    IEditable* cEditable;
    
    public:
    PropertyEditor(const std::string& name, IEditable* editable);
      
    /************************\
     * Implements IProperty *
    \************************/
    bool updateEditing(unsigned int milliseconds) override;
    void render(IPropertyAppearance* appearance) const override;
    void renderEditing(IPropertyAppearance* appearance) const override;
    IEditable* edit(IAssetRegistry* assets) override;
    void close() override;
    float getWidth(IPropertyAppearance* appearance) const override;
    bool input(sf::Event& event) override;
  };
}
