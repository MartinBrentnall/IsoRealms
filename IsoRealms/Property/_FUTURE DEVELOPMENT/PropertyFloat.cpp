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
#include "PropertyFloat.h"

namespace IsoRealms {
  PropertyFloat::PropertyFloat(const std::string& label, IFloat** asset, IAssetUser<IFloat>* client, IAssetBrowser* browser) :
            PropertyAssetBase(label, asset, client, browser, browser->getAllFloats(), std::vector<IAssetMenuItem*>({&cFixedOption})),
            cFixedOption(this) {
  }
  
  bool PropertyFloat::renderIcon(IAssetBrowser* browser, const std::string& id) const {
    return browser->renderFloatIcon(id);
  }
  
  IFloat* PropertyFloat::getAsset(IAssetBrowser* browser, IAssetUser<IFloat>* client, const std::string& id) const {
    return browser->getFloat(client, id);
  }
  
  PropertyFloat::FixedFloat::FixedFloat(PropertyFloat* parent) :
            cParent(parent),
            cValue("Enter value...") {
  }
  
  void PropertyFloat::FixedFloat::render(IPropertyAppearance* appearance, float yOffset) const {
    glPushMatrix();
    appearance->transformForIcon(yOffset);
    Utils::renderIconCustom();
    glPopMatrix();
    appearance->print(cValue, yOffset, appearance->getIconWidth() + appearance->getIconTextSpacing());
  }
  
  bool PropertyFloat::FixedFloat::executeAction() {
    // FIXME:IsoRealmsEditor: Implement this
    return false;
  }
  
  float PropertyFloat::FixedFloat::getWidth(IPropertyAppearance* appearance) const {
    return appearance->getTextWidth(cValue) + appearance->getIconWidth() + appearance->getIconWidth();
  }
  
  std::vector<std::unique_ptr<PropertyAsset::ISelection>> PropertyFloat::FixedFloat::getSelection(const std::string& id, const AssetMenu* menu, unsigned int item) const {
    std::vector<std::unique_ptr<PropertyAsset::ISelection>> mSelection;
    if (id[0] == '#') {
      mSelection.emplace_back(std::make_unique<AssetMenu::Selection>(menu, item, 0.0f));
      cValue = id.substr(1);
    }
    return mSelection;
  }  
}
