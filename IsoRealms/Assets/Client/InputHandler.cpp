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

namespace IsoRealms {
  InputHandler::InputHandler(IProject& project) : 
            Asset<IInputHandler, IProject>(project, project.createLiteralInputHandler(this)) {
  }

  IInputHandler* InputHandler::createLiteralAsset(IProject& project) {
    return project.createLiteralInputHandler(this);
  }
  
  IInputHandler* InputHandler::getAsset(IProject& project, JSONObject object) {
    return project.getInputHandler(this, object);
  }
  
  IInputHandler* InputHandler::getAsset(IProject& project, const std::string& id) {
    return project.getInputHandler(this, id);
  }
  
  std::vector<std::string> InputHandler::getAvailableProviders() const {
    return cManager.getAllInputHandlers();
  }  

  bool InputHandler::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderInputHandlerIcon(id);
  }

  bool InputHandler::hasConfiguration() const {
    return cManager.isInputHandlerConfigurable(getID());
  }  

  bool InputHandler::isDefaultConfiguration() const {
    return true;
  }
}
