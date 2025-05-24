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
#include "PropertiesTool.h"

#include "IsoRealms/Editing/UIManager.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Editor/WorldEditor.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const LiteralColour PropertiesTool::SELECTION_COLOUR(1.0f, 0.0f, 0.2f, 1.0f);
  const LiteralColour PropertiesTool::LOCKED_COLOUR(0.0f, 0.0f, 0.0f, 1.0f);

  IWorldEditorToolInstance* PropertiesTool::createToolInstance(WorldEditor& editor) {
    return cEditingModifiers.emplace_back(std::make_unique<Modifier>(*this, editor)).get();
  }

  bool PropertiesTool::renderAssetIcon() const {
    Utils::renderIconCustom();
    return true;
  }

  void PropertiesTool::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> PropertiesTool::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool PropertiesTool::isDefaultConfiguration() const {
    return true;
  }

  PropertiesTool::Modifier::Modifier(PropertiesTool& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor),
            cSelectedObject(0),
            cEditingProperties(false),
            cPropertiesUI(editor.getWorld().getSpindizzy().getProject(), *this, [this]() { // TODO: Am I passing the right project here????
              cEditingProperties = false;
            }, [](IEditable* editor) {
              std::cout << "WARNING: PropertiesTool::Modifier::Modifier: This UI does not support editables." << std::endl;
            }) {
  }

  bool PropertiesTool::Modifier::inputTool(SignalInputID id, double yaw) {
    if (cEditingProperties) {
      switch (id) {
        case SignalInputID::MOVE_CURSOR_FORWARD:  cPropertiesUI.input(UISignalID::MOVE_UP);    break;
        case SignalInputID::MOVE_CURSOR_BACKWARD: cPropertiesUI.input(UISignalID::MOVE_DOWN);  break;
        case SignalInputID::MOVE_CURSOR_LEFT:     cPropertiesUI.input(UISignalID::MOVE_LEFT);  break;
        case SignalInputID::MOVE_CURSOR_RIGHT:    cPropertiesUI.input(UISignalID::MOVE_RIGHT); break;
        case SignalInputID::USE_TOOL:             cPropertiesUI.input(UISignalID::CONFIRM);    break;
        case SignalInputID::CANCEL:               cPropertiesUI.input(UISignalID::CANCEL);     break;
        default:                                                                               break;
      }
      return true;
    }

    switch (id) {
      case SignalInputID::USE_TOOL:  showProperties();                                                                                        return true;
      case SignalInputID::TOOL_MODE: cSelectedObject++; if (cSelectedObject >= static_cast<int>(cHoverObjects.size())) {cSelectedObject = 0;} return true;
      default:                                                                                                                                break;
    }
    return false;
  }

  bool PropertiesTool::Modifier::isCursorLocked() const {
    return cEditingProperties;
  }

  void PropertiesTool::Modifier::showProperties() {
    if (!cHoverObjects.empty() && !cEditingProperties) {
      cPropertiesUI.openUI(std::make_unique<PropertiesMenu>(cPropertiesUI, *this, [this]() {
        return cHoverObjects[cSelectedObject]->getProperties();
      }, cHoverObjects[cSelectedObject]->getTypeName() + " Configuration:", 1.0f, 1.0f, 1.0f));
      cEditingProperties = true;
    }
  }

  void PropertiesTool::Modifier::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    if (end != nullptr) {
      cEditor.getWorld().selectObjects(start, *end, [this](IWorldObject* object) {
        return true;
      }, [this](IWorldObject* object) {
        if (!object->getProperties().empty()) {
          cHoverObjects.push_back(object);
          cSelectedObject = cHoverObjects.size() - 1;
        }
      });
    } else {
      cHoverObjects.clear();
      cSelectedObject = 0;
    }

    cHoverObjects.erase(std::remove_if(cHoverObjects.begin(), cHoverObjects.end(), [end](IWorldObject* object) {
      return !object->contains(*end);
    }), cHoverObjects.end());

    if (cSelectedObject >= static_cast<int>(cHoverObjects.size())) {
      cSelectedObject = cHoverObjects.size() - 1;
    }
  }

  bool PropertiesTool::Modifier::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  double PropertiesTool::Modifier::getSnapInterval() const {
    return 1.0;
  }

  void PropertiesTool::Modifier::renderEditingPreview() const {
    if (!cHoverObjects.empty()) {
      glEnable(GL_BLEND);
      glEnable(GL_ALPHA_TEST);
      cHoverObjects[cSelectedObject]->renderSelectionHighlight();
      glDisable(GL_ALPHA_TEST);
      glDisable(GL_BLEND);
    }

    glTranslatef(cEditor.getCursorX(), cEditor.getCursorY(), cEditor.getCursorZ() * 0.5f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.5f);
    Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
    glEnd();
  }

  void PropertiesTool::Modifier::renderUI(float aspectRatio) const {
    cPropertiesUI.render(aspectRatio);
  }

  bool PropertiesTool::Modifier::renderIcon(float yaw) const {
    return cParent.renderAssetIcon();
  }

  void PropertiesTool::Modifier::updateUI(unsigned int milliseconds) {
    cPropertiesUI.update(milliseconds);
  }

  IFont* PropertiesTool::Modifier::getFont() const {
    return cEditor.getFont();
  }

  float PropertiesTool::Modifier::getFontSize() const {
    return cEditor.getFontSize();
  }

  IFont* PropertiesTool::Modifier::getCodeFont() const {
    return nullptr;
  }

  float PropertiesTool::Modifier::getCodeFontSize() const {
    return 0.2f;
  }

  IProject& PropertiesTool::Modifier::getProject() const {
    return cEditor.getWorld().getSpindizzy().getProject();
  }
  
  // const IColour* PropertiesTool::Modifier::getSelectionColour() const {
  //   return &SELECTION_COLOUR;
  // }
  //
  // const IColour* PropertiesTool::Modifier::getLockedColour() const {
  //   return &LOCKED_COLOUR;
  // }
}
