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

#include "IsoRealms/Editing/Configurator.h"
#include "IsoRealms/Editing/PropertiesMenu.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Editor/WorldEditor.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const LiteralColour PropertiesTool::SELECTION_COLOUR(1.0f, 0.0f, 0.2f, 1.0f);
  const LiteralColour PropertiesTool::LOCKED_COLOUR(0.0f, 0.0f, 0.0f, 1.0f);

  IWorldEditorToolInstance* PropertiesTool::createToolInstance(WorldEditor* editor) {
    return cEditingModifiers.emplace_back(std::make_unique<Modifier>(*this, editor)).get();
  }

  PropertiesTool::Modifier::Modifier(PropertiesTool& parent, WorldEditor* editor) :
            cParent(parent),
            cEditor(editor),
            cSelectedObject(0),
            cConfigurator(this),
            cPropertiesMenu(&cConfigurator, editor->getPropertyAppearance()) {
  }

  bool PropertiesTool::Modifier::inputEdit(sf::Event& event) {
    if (cEditingProperties) {
      cConfigurator.input(event);
      return true;
    }

    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Space: showProperties();                                                                                        return true;
          case sf::Keyboard::Tab:   cSelectedObject++; if (cSelectedObject >= static_cast<int>(cHoverObjects.size())) {cSelectedObject = 0;} return true;
          default:                                                                                                                           break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: showProperties(); return true;
        }
        break;
      }

      default: break;
    }
    return false;
  }

  void PropertiesTool::Modifier::showProperties() {
    if (!cHoverObjects.empty() && !cEditingProperties) {
      cProperties = cHoverObjects[cSelectedObject]->getProperties(cEditor->getPropertyAppearance());
      cPropertiesMenu.clear();
      cPropertiesMenu.setTitle(cHoverObjects[cSelectedObject]->getTypeName() + " Configuration:");
      for (unsigned int i = 0; i < cProperties.size(); i++) {
        cPropertiesMenu.addItem(cProperties[i].get());
      }
      cConfigurator.open(&cPropertiesMenu);
      cEditingProperties = true;
    }
  }

  void PropertiesTool::Modifier::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    if (end != nullptr) {
      cEditor->getWorld()->selectObjects(start, *end, [this](IWorldObject* object) {
        return true;
      }, [this](IWorldObject* object) {
        if (!object->getProperties(cEditor->getPropertyAppearance()).empty()) {
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

    glTranslatef(cEditor->getCursorX(), cEditor->getCursorY(), cEditor->getCursorZ() * 0.5f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.5f);
    Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
    glEnd();
  }

  void PropertiesTool::Modifier::renderUI() const {
    cConfigurator.render();
  }

  bool PropertiesTool::Modifier::renderIcon(float yaw) const {
    Utils::renderIconLeaf();
    return true;
  }

  void PropertiesTool::Modifier::updateUI(unsigned int milliseconds) {
    cConfigurator.update(milliseconds);
  }

  IFont* PropertiesTool::Modifier::getFont() const {
    return cConfigurator.getFont();
  }

  const IColour* PropertiesTool::Modifier::getSelectionColour() const {
    return &SELECTION_COLOUR;
  }

  const IColour* PropertiesTool::Modifier::getLockedColour() const {
    return &LOCKED_COLOUR;
  }

  void PropertiesTool::Modifier::exit() {
    cEditingProperties = false;
  }
}
