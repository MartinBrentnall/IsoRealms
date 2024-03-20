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
#include "DeleteTool.h"

#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  IWorldEditorToolInstance* DeleteTool::createToolInstance(WorldEditor* editor) {
    return cEditingErasers.emplace_back(std::make_unique<Eraser>(*this, editor)).get();
  }

  DeleteTool::Eraser::Eraser(DeleteTool& parent, WorldEditor* editor) :
            cParent(parent),
            cEditor(editor),
            cSelectedObject(0) {
  }

  bool DeleteTool::Eraser::inputEdit(sf::Event& event) {
    switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::Space: removeSelectedObject();                                                                                  return true;
          case sf::Keyboard::Tab:   cSelectedObject++; if (cSelectedObject >= static_cast<int>(cHoverObjects.size())) {cSelectedObject = 0;} return true;
          default:                                                                                                                           break;
        }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        switch (event.joystickButton.button) {
          case 0: removeSelectedObject();                                                                                  return true;
          case 2: cSelectedObject++; if (cSelectedObject >= static_cast<int>(cHoverObjects.size())) {cSelectedObject = 0;} return true;
        }
        break;
      }

      default: break;
    }
    return false;
  }

  void DeleteTool::Eraser::removeSelectedObject() {
    if (!cHoverObjects.empty()) {
      cHoverObjects[cSelectedObject]->remove();
      cHoverObjects.erase(cHoverObjects.begin() + cSelectedObject);
      if (cSelectedObject >= static_cast<int>(cHoverObjects.size())) {
        cSelectedObject = cHoverObjects.size() - 1;
      }
    }
  }

  void DeleteTool::Eraser::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    if (end != nullptr) {
      cEditor->getWorld()->selectObjects(start, *end, [this](IWorldObject* object) {
        return true;
      }, [this](IWorldObject* object) {
        cHoverObjects.push_back(object);
        cSelectedObject = cHoverObjects.size() - 1;
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

  bool DeleteTool::Eraser::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  double DeleteTool::Eraser::getSnapInterval() const {
    return 1.0;
  }

  void DeleteTool::Eraser::renderEditingPreview() const {
    if (!cHoverObjects.empty()) {
      glEnable(GL_BLEND);
      glAlphaFunc(GL_GREATER, 0.1f);
      glEnable(GL_ALPHA_TEST);
      cHoverObjects[cSelectedObject]->renderSelectionHighlight();
    }

    glTranslatef(cEditor->getCursorX(), cEditor->getCursorY(), cEditor->getCursorZ() * 0.5f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
    glEnd();
  }

  void DeleteTool::Eraser::renderUI() const {
    // Nothing to do.
  }

  bool DeleteTool::Eraser::renderIcon(float yaw) const {
    Utils::renderIconNone();
    return true;
  }

  void DeleteTool::Eraser::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }
}
