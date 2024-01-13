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
#include "PropertyInputHandler.h"

namespace IsoRealms {
  PropertyInputHandler::PropertyInputHandler(const std::string& label, IInputHandler** asset, IAssetUser<IInputHandler>* client, IAssetBrowser* browser, std::function<void()> removeCallback) :
            PropertyAssetBase(label, asset, client, browser, browser->getAllInputHandlers(), std::vector<IAssetMenuItem*>({&cOptionNone}), removeCallback),
            cOptionNone(this, []() {Utils::renderIconNone();}, "None", "#") {
  }
  
  PropertyInputHandler::PropertyInputHandler(const std::string& label, IAssetUser<IInputHandler>* client, IAssetBrowser* browser, std::function<void(IInputHandler* asset)> addCallback) :
            PropertyAssetBase(label, client, browser, browser->getAllInputHandlers(), std::vector<IAssetMenuItem*>({&cOptionNone}), addCallback),
            cOptionNone(this, []() {Utils::renderIconNone();}, "None", "#") {
  }
  
  bool PropertyInputHandler::renderIcon(IAssetBrowser* browser, const std::string& id) const {
    return browser->renderInputHandlerIcon(id);
  }
  
  IInputHandler* PropertyInputHandler::getAsset(IAssetBrowser* browser, IAssetUser<IInputHandler>* client, const std::string& id) const {
    return browser->getInputHandler(client, id);
  }
}
