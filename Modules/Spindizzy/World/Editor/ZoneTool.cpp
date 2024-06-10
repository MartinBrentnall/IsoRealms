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
#include "ZoneTool.h"

#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  IWorldEditorToolInstance* ZoneTool::createToolInstance(WorldEditor* editor) {
    return cInstances.emplace_back(std::make_unique<Instance>(*this, editor)).get();
  }

  bool ZoneTool::renderAssetIcon() const {
    return false;
  }

  void ZoneTool::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  ZoneTool::Instance::Instance(ZoneTool& parent, WorldEditor* editor) :
            cParent(parent),
            cEditor(editor),
            cHoverZone(nullptr),
            cSelectedZone(nullptr) {
  }

  bool ZoneTool::Instance::inputTool(SignalInputID id, double yaw) {
    switch (id) {
      case SignalInputID::USE_TOOL: {
        if (cHoverZone != nullptr) {
          cSelectedZone = cHoverZone;
        } else if (cSelectedZone != nullptr) {
          cEditor->getWorld()->copy(cSelectedZone, cEditor->getCursorCell(), cEditor);
        }
        return true;
      }

      case SignalInputID::TOOL_MODE: {
        return true;
      }

      default: {
        break;
      }
    }
    return false;
  }

  bool ZoneTool::Instance::isCursorLocked() const {
    return false;
  }

  void ZoneTool::Instance::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
    if (end != nullptr) {
      cHoverZone = cEditor->getWorld()->getZone(end);
    } else {
      cHoverZone = nullptr;
      cSelectedZone = nullptr;
    }
  }

  bool ZoneTool::Instance::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  double ZoneTool::Instance::getSnapInterval() const {
    return 1.0;
  }

  void ZoneTool::Instance::renderEditingPreview() const {
    if (cSelectedZone != nullptr) {
      glEnable(GL_BLEND);
      glAlphaFunc(GL_GREATER, 0.1f);
      glEnable(GL_ALPHA_TEST);
      cSelectedZone->renderSelectionHighlight();
    }

    glTranslatef(cEditor->getCursorX(), cEditor->getCursorY(), cEditor->getCursorZ() * 0.5f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
    glEnd();
  }

  void ZoneTool::Instance::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  bool ZoneTool::Instance::renderIcon(float yaw) const {
    glTranslatef(0.0f, 0.3f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(Spindizzy::DEFAULT_VIEW_ANGLE_YAW, 0.0f, 0.0f, 1.0f);
    // TODO: Scale the icon
    glScalef(1.3f, 1.3f, 1.3f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);
    return true;
  }

  void ZoneTool::Instance::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }
}
