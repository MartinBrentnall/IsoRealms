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
#include "InputHandler.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/IResourceData.h"

namespace IsoRealms {
  InputHandler::InputHandler(IResourceData& owner) :
            Asset<IInputHandler, IResourceData>(owner, owner.getAssetManager().createLiteralInputHandler(this, owner)) {
  }

  IInputHandler* InputHandler::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralInputHandler(this, owner);
  }
  
  IInputHandler* InputHandler::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getInputHandler(this, object, owner);
  }
  
  IInputHandler* InputHandler::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getInputHandler(this, id, owner);
  }
  
  std::vector<std::string> InputHandler::getAvailableProviders() const {
    return cManager.getAssetManager().getAllInputHandlers();
  }  

  bool InputHandler::renderOtherProviderIcon(const std::string& id) const {
    return cManager.getAssetManager().renderInputHandlerIcon(id);
  }

  bool InputHandler::hasConfiguration() const {
    return cManager.getAssetManager().isInputHandlerConfigurable(getID());
  }  

  bool InputHandler::isDefaultConfiguration() const {
    return true;
  }
}
