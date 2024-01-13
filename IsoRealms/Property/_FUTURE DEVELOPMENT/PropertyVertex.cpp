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
#include "PropertyVertex.h"

namespace IsoRealms {
  PropertyVertex::PropertyVertex(const std::string& label, IVertex** asset, IAssetUser<IVertex>* client, IAssetBrowser* browser) :
            PropertyAssetBase(label, asset, client, browser, browser->getAllVertices(), std::vector<IAssetMenuItem*>({&cOptionNone})),
            cOptionNone(this, []() {Utils::renderIconNone();}, "None", "#") {
  }
  
  bool PropertyVertex::renderIcon(IAssetBrowser* browser, const std::string& id) const {
    return browser->renderVertexIcon(id);
  }
  
  IVertex* PropertyVertex::getAsset(IAssetBrowser* browser, IAssetUser<IVertex>* client, const std::string& id) const {
    return browser->getVertex(client, id);
  }
}
