/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ZoneObjectTypeTraitCellLocation.h"

#include <cmath>

#include "Modules/Spindizzy/World/Editor/WorldEditor.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/CellLocation/CellLocation.h"
#include "Modules/Spindizzy/ZoneObjectType/ZoneObjectType.h"

namespace IsoRealms::Spindizzy {
  ZoneObjectTypeTraitCellLocation::ZoneObjectTypeTraitCellLocation(IProject& project, ZoneObjectType& type) :
            cDefType(type) {
    cDefType.registerEditor(this);
  }

  ZoneObjectTypeTraitCellLocation::ZoneObjectTypeTraitCellLocation(IProject& project, ZoneObjectType& type, JSONObject object) :
            ZoneObjectTypeTraitCellLocation(project, type) {
  }

  void ZoneObjectTypeTraitCellLocation::save(JSONObject object) const {
    // Nothing to do.
  }

  std::unique_ptr<IZoneObjectTrait> ZoneObjectTypeTraitCellLocation::createTrait(ZoneObject& object) {
    return std::make_unique<CellLocation>(object, cEditingPinnedX, cEditingPinnedY, cEditingPinnedZ);
  }  
  
  void ZoneObjectTypeTraitCellLocation::registerAssets(ISpindizzyRegistry* registry) {
    // Nothing to do.
  }
  
  bool ZoneObjectTypeTraitCellLocation::renderAssetIcon() const {
    return false;
  }

  void ZoneObjectTypeTraitCellLocation::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> ZoneObjectTypeTraitCellLocation::getAssetProperties(IPropertyOwner& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ZoneObjectTypeTraitCellLocation::isDefaultConfiguration() const {
    return true;
  }

  IZoneObjectTraitEditor::InputEditResult ZoneObjectTypeTraitCellLocation::inputEdit(sf::Event& event, WorldEditor& editor) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Space: {
            cEditingPinnedX = editor.getCursorCell().cDefX;
            cEditingPinnedY = editor.getCursorCell().cDefY;
            cEditingPinnedZ = editor.getCursorCell().cDefZ;
            return InputEditResult::COMPLETED;
          }
          
          case sf::Keyboard::Escape: {
            return InputEditResult::CANCELLED;
          }

          default: {
            // Nothing to do.
            break;
          }
        }
        break;
      }
      
      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: {
            cEditingPinnedX = editor.getCursorCell().cDefX;
            cEditingPinnedY = editor.getCursorCell().cDefY;
            cEditingPinnedZ = editor.getCursorCell().cDefZ;
            return InputEditResult::COMPLETED;
          }

          case 1: {
            return InputEditResult::CANCELLED;
          }
        }
        break;
      }

      default: break;
    }
    return InputEditResult::NONE;
  }
  
  void ZoneObjectTypeTraitCellLocation::processCursorMovement(LiteralVertex& cursor) {
    // Nothing to do.
  }
  
  double ZoneObjectTypeTraitCellLocation::getSnapInterval() const {
    return 1.0;
  }  
}
