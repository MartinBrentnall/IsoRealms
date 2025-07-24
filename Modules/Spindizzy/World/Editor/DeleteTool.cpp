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
#include "DeleteTool.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  IWorldEditorToolInstance* DeleteTool::createToolInstance(WorldEditor& editor, IResourceData& owner) {
    return cEditingErasers.emplace_back(std::make_unique<Eraser>(*this, editor)).get();
  }

  bool DeleteTool::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void DeleteTool::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void DeleteTool::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool DeleteTool::isDefaultConfiguration() const {
    return true;
  }

  DeleteTool::Eraser::Eraser(DeleteTool& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor),
            cSelectedObject(0) {
  }

  bool DeleteTool::Eraser::inputTool(SignalInputID id, double yaw) {
    switch (id) {
      case SignalInputID::USE_TOOL:  removeSelectedObject();                                                                                  return true;
      case SignalInputID::TOOL_MODE: cSelectedObject++; if (cSelectedObject >= static_cast<int>(cHoverObjects.size())) {cSelectedObject = 0;} return true;
      default:                                                                                                                                break;
    }
    return false;
  }

  bool DeleteTool::Eraser::isCursorLocked() const {
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
      cEditor.getWorld().selectObjects(start, *end, [this](IWorldObject* object) {
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

    glTranslatef(cEditor.getCursorX(), cEditor.getCursorY(), cEditor.getCursorZ() * 0.5f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
    glEnd();
  }

  void DeleteTool::Eraser::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  bool DeleteTool::Eraser::renderIcon(float yaw) const {
    return cParent.renderAssetIcon();
  }

  void DeleteTool::Eraser::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }
}
