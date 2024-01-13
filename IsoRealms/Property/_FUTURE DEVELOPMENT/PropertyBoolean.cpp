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
#include "PropertyBoolean.h"

namespace IsoRealms {
  PropertyBoolean::PropertyBoolean(const std::string& label, IBoolean** asset, IAssetUser<IBoolean>* client, IAssetBrowser* browser, Type mode, const std::string& firstLabel, const std::string& secondLabel) :
            PropertyAssetBase(label, asset, client, browser, browser->getAllBooleans(), mode == Type::ONLY_FALSE ? std::vector<IAssetMenuItem*>({
                         &cFalseOption
            })                                                                       :  mode == Type::ONLY_TRUE  ? std::vector<IAssetMenuItem*>({
                         &cTrueOption
            })                                                                       :                             std::vector<IAssetMenuItem*>({
                         &cTrueOption,
                         &cFalseOption
            })),
            cFalseOption(this, []() {Utils::renderIconNone();}, firstLabel, "#false"),
            cTrueOption(this, []() {Utils::renderIconTick();}, mode == ONLY_TRUE ? firstLabel : secondLabel, "#true") {
  }

  IBoolean* PropertyBoolean::getAsset(IAssetBrowser* browser, IAssetUser<IBoolean>* client, const std::string& id) const {
    return browser->getBoolean(client, id);
  }
  
  bool PropertyBoolean::renderIcon(IAssetBrowser* browser, const std::string& id) const {
    return browser->renderBooleanIcon(id);
  }
}
