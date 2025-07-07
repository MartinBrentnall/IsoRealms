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
#include "PropertyEditor.h"

#include "IsoRealms/Assets/Type/IFont.h"
#include "IsoRealms/Editing/IUIStyle.h"
#include "IsoRealms/Utils.h"

#include "IPropertyManager.h"

namespace IsoRealms {
  PropertyEditor::PropertyEditor(const std::string& name, const std::string& tooltip, IEditable* editable) :
            Property(name, tooltip, nullptr),
            cEditable(editable) {
  }
      
  void PropertyEditor::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    glPushMatrix();
    glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    Utils::renderIconCustom();
    glPopMatrix();
    mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, "Edit...");
  }
  
  float PropertyEditor::getValueWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, "Edit...") + mFontSize * 2.25f;
  }
  
  void PropertyEditor::confirm(IPropertyManager& manager, float y) {
    manager.edit(cEditable);
  }
  
  bool PropertyEditor::hasConfiguration() const {
    return false;
  }
  
  void PropertyEditor::configure(IPropertyManager& manager) {
    // Nothing to do.
  }
}
