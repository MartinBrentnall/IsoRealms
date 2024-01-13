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
#include "PropertyModel.h"

namespace IsoRealms {
  PropertyModel::PropertyModel(const std::string& label, I3DModelType** asset, IAssetUser<I3DModelType>* client, IAssetBrowser* browser, std::function<void()> updateCallback, std::function<void()> removeCallback) :
            PropertyAssetBase(label, asset, client, browser, browser->getAllModelTypes(), std::vector<IAssetMenuItem*>({&cOptionNone}), updateCallback, removeCallback, "#"),
            cOptionNone(this, []() {Utils::renderIconNone();}, "None", "#") {
  }
  
  PropertyModel::PropertyModel(const std::string& label, IAssetUser<I3DModelType>* client, IAssetBrowser* browser, std::function<void(I3DModelType* asset)> addCallback) :
            PropertyAssetBase(label, client, browser, browser->getAllModelTypes(), std::vector<IAssetMenuItem*>({&cOptionNone}), addCallback),
            cOptionNone(this, []() {Utils::renderIconNone();}, "None", "#") {
  }
  
  bool PropertyModel::renderIcon(IAssetBrowser* browser, const std::string& id) const {
    return browser->renderModelIcon(id);
  }
  
  I3DModelType* PropertyModel::getAsset(IAssetBrowser* browser, IAssetUser<I3DModelType>* client, const std::string& id) const {
    return browser->getModelType(client, id);
  }
}
