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
#include "PropertyEditor.h"

namespace IsoRealms::Spindizzy {
  PropertyEditor::PropertyEditor(const std::string& name, IEditable* editable) :
            Property(name),
            cEditable(editable) {
  }
      
  bool PropertyEditor::updateEditing(unsigned int milliseconds) {
    return true;
  }
  
  void PropertyEditor::render(IPropertyAppearance* appearance) const {
    print("Edit", 0.0f, appearance->getIconWidth() + appearance->getIconTextSpacing());
    glPushMatrix();
    transformForIcon(0.0f);
    Utils::renderIconCustom();
    glPopMatrix();
  }
  
  void PropertyEditor::renderEditing(IPropertyAppearance* appearance) const {
    render(appearance);
  }
  
  IEditable* PropertyEditor::edit(IAssetRegistry* assets) {
    return cEditable;
  }
  
  void PropertyEditor::close() {
    // Nothing to do.
  }
  
  float PropertyEditor::getWidth(IPropertyAppearance* appearance) const {
    return appearance->getIconWidth() + appearance->getIconTextSpacing() + appearance->getTextWidth("Edit");
  }
  
  bool PropertyEditor::input(sf::Event& event) {
    return true;
  }
}
