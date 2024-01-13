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
#include "PropertyScreen.h"

namespace IsoRealms {
  PropertyScreen::PropertyScreen(const std::string& label, IScreen** asset, IAssetUser<IScreen>* client, IAssetBrowser* browser) :
            PropertyAssetBase(label, asset, client, browser, browser->getAllVertices(), std::vector<IAssetMenuItem*>({&cOptionNone})),
            cOptionNone(this, []() {Utils::renderIconNone();}, "None", "#"){
  }
  
  bool PropertyScreen::renderIcon(IAssetBrowser* browser, const std::string& id) const {
    return browser->renderScreenIcon(id);
  }
  
  IScreen* PropertyScreen::getAsset(IAssetBrowser* browser, IAssetUser<IScreen>* client, const std::string& id) const {
    return browser->getScreen(client, id);
  }
}
