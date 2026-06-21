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
#include "PropertyRemover.h"

#include "IsoRealms/Resources/Type/IFont.h"
#include "IsoRealms/Editing/IUIStyle.h"

#include "IPropertyManager.h"

namespace IsoRealms {
  PropertyRemover::PropertyRemover(const PropertyData& data, IComponentAccessManager& resourceAccessManager, const std::string& value, std::function<void()> removeFunction) :
            Property(data, resourceAccessManager, removeFunction),
            cValue(value) {
  }

  void PropertyRemover::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(x, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cValue);
  }

  float PropertyRemover::getValueWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cValue);
  }

  void PropertyRemover::confirm(IPropertyManager& manager, float y) {
    // Nothing to do.  Removal is triggered only via the remove icon.
  }

  bool PropertyRemover::hasConfiguration() const {
    return false;
  }

  void PropertyRemover::configure(IPropertyManager& manager) {
    // Nothing to do.
  }
}
