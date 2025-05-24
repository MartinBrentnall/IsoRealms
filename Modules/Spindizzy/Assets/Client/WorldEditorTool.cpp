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
#include "WorldEditorTool.h"

#include "IsoRealms/Editing/Property/IProperty.h"

#include "Modules/Spindizzy/Spindizzy.h"

namespace IsoRealms::Spindizzy {
  WorldEditorTool::WorldEditorTool(Spindizzy& spindizzy) : 
            Asset<IWorldEditorTool, Spindizzy>(spindizzy, spindizzy.createLiteralWorldEditorTool(this)) {
  }

  IWorldEditorTool* WorldEditorTool::createLiteralAsset(Spindizzy& spindizzy) {
    return spindizzy.createLiteralWorldEditorTool(this);
  }
  
  IWorldEditorTool* WorldEditorTool::getAsset(Spindizzy& spindizzy, JSONObject object) {
    return spindizzy.getWorldEditorTool(this, object);
  }
  
  IWorldEditorTool* WorldEditorTool::getAsset(Spindizzy& spindizzy, const std::string& id) {
    return spindizzy.getWorldEditorTool(this, id);
  }
  
  std::vector<std::string> WorldEditorTool::getAvailableProviders() const {
    return cManager.getAllWorldEditorTools();
  }  

  bool WorldEditorTool::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderWorldEditorToolIcon(id);
  }

  bool WorldEditorTool::hasConfiguration() const {
    return cManager.isWorldEditorToolConfigurable(getID());
  }  

  bool WorldEditorTool::isDefaultConfiguration() const {
    return true;
  }
}
