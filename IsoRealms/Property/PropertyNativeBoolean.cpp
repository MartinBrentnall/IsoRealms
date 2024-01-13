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
#include "PropertyNativeBoolean.h"

namespace IsoRealms {
  bool PropertyNativeBoolean::cTrue  = true;
  bool PropertyNativeBoolean::cFalse = false;
  
  PropertyNativeBoolean::PropertyNativeBoolean(IPropertyAppearance* appearance, const std::string& name, std::function<bool()> getter, std::function<void(bool)> setter, const std::string& labelFalse, const std::string& labelTrue) :
            PropertyAsset(appearance, name, getter() ? "#true" : "#false", std::vector<std::pair<std::string, std::string>>({}), std::vector<IAssetMenuItem*>({
              &cFalseOption,
              &cTrueOption
            }), [this](const std::string& id) {
              cSetter(id == "#true" ? true : false);
              return true;
            }),
            cGetter(getter),
            cSetter(setter),
            cFalseOption(this, []() {Utils::renderIconNone();}, labelFalse, "#false"),
            cTrueOption(this, []() {Utils::renderIconTick();}, labelTrue, "#true") {
  }

  bool PropertyNativeBoolean::renderIcon(IPropertyAppearance* appearance, const std::string& id) const {
    return false;
  }
  
  PropertyNativeBoolean::FixedBoolean::FixedBoolean(PropertyNativeBoolean* parent, std::function<void()> icon, const std::string& label, const std::string& value) :
            cParent(parent),
            cLabel(label),
            cValue(value),
            cIcon(icon) {
  }
  
  void PropertyNativeBoolean::FixedBoolean::render(IPropertyAppearance* appearance, float yOffset) const {
    glPushMatrix();
    appearance->transformForIcon(yOffset);
    cIcon();
    glPopMatrix();
    appearance->print(cLabel, yOffset, appearance->getIconWidth() + appearance->getIconTextSpacing());
  }
  
  bool PropertyNativeBoolean::FixedBoolean::executeAction() {
    cParent->cConfirmationCallback(cValue);
    return true;
  }
  
  float PropertyNativeBoolean::FixedBoolean::getWidth(IPropertyAppearance* appearance) const {
    return appearance->getWidth(cLabel) + appearance->getIconWidth() * 2.0f;
  }
  
  std::vector<std::unique_ptr<PropertyAsset::ISelection>> PropertyNativeBoolean::FixedBoolean::getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const {
    std::vector<std::unique_ptr<PropertyAsset::ISelection>> mSelection;
    if (id == cValue) {
      mSelection.emplace_back(std::make_unique<AssetMenu::Selection>(menu, item, 0.0f));
    }
    return mSelection;
  }  
}
