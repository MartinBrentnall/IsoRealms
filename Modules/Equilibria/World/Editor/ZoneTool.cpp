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
#include "ZoneTool.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/World.h"

namespace IsoRealms::Equilibria {
  ZoneTool::ZoneTool(Type type) :
          cDefType(type) {
  }

  IWorldEditorToolInstance* ZoneTool::createToolInstance(WorldEditor& editor, IComponentData& owner) {
    return cInstances.emplace_back(std::make_unique<Instance>(*this, editor)).get();
  }

  bool ZoneTool::renderAssetIcon() const {
    return renderEditingIcon(Type::MOVE);
  }

  void ZoneTool::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void ZoneTool::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool ZoneTool::isDefaultConfiguration() const {
    return true;
  }

  ZoneTool::Instance::Instance(ZoneTool& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor) {
  }

  bool ZoneTool::Instance::inputTool(sf::Event& event) {
    return false;
  }

  bool ZoneTool::Instance::inputTool(SignalInputID id, double yaw) {
    switch (id) {
      case SignalInputID::USE_TOOL: {
        if (cHoverZone != nullptr) {
          if (cParent.cDefType == Type::DELETE) {
            cEditor.getWorld().remove(cHoverZone);
            cHoverZone = nullptr;
          } else {
            cSelectedZone = cHoverZone;
          }
        } else if (cSelectedZone != nullptr) {
          if (cParent.cDefType != Type::DELETE) {
            cEditor.getWorld().copy(cSelectedZone, cEditor.getCursorCell(), cEditor);
            if (cParent.cDefType == Type::MOVE) {
              cEditor.getWorld().remove(cSelectedZone);
              cSelectedZone = nullptr;
            }
          }
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
      cHoverZone = cEditor.getWorld().getZone(end);
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
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_ALPHA_TEST);
    if (cSelectedZone != nullptr) {
      glColor4f(0.0f, 1.0f, 1.0f, 0.5f);
      cSelectedZone->renderSelectionHighlight();
    }
    if (cHoverZone != nullptr && cHoverZone != cSelectedZone) {
      if (cParent.cDefType == Type::DELETE) {
        glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
      } else {
        glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
      }
      cHoverZone->renderSelectionHighlight();
    }

    glTranslatef(cEditor.getCursorX(), cEditor.getCursorY(), cEditor.getCursorZ() * 0.5f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
    glEnd();
  }

  void ZoneTool::Instance::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  bool ZoneTool::Instance::renderIcon(float yaw) const {
    return cParent.renderEditingIcon(cParent.cDefType);
  }

  void ZoneTool::Instance::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  bool ZoneTool::renderEditingIcon(Type type) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glTranslatef(0.0f, 0.3f, 0.0f);
    glRotatef(Equilibria::DEFAULT_VIEW_ANGLE_PITCH, 1.0f, 0.0f, 0.0f);
    glRotatef(Equilibria::DEFAULT_VIEW_ANGLE_YAW, 0.0f, 0.0f, 1.0f);
    // TODO: Scale the icon
    glColor3f(0.0f, 1.0f, 0.0f);
    if (type == Type::DELETE) {
      glScalef(1.3f, 1.3f, 1.3f);
      glBegin(GL_LINES);
      Utils::renderVolumeLines(-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.0f);
      glEnd();
    } else {
      glScalef(0.7f, 0.7f, 0.7f);
      glBegin(GL_LINES);
      Utils::renderVolumeLines(-1.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.0f);
      Utils::renderVolumeLines( 0.5f,  1.5f, -0.5f, 0.5f, -1.0f, -0.5f);
      glEnd();
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    if (type != Type::DELETE) {
      glBegin(GL_LINES);
      glVertex2f(-0.1f,  0.1f);
      glVertex2f( 0.1f, -0.1f);
      glEnd();

      glBegin(GL_TRIANGLES);
      glVertex2f( 0.1f, -0.1f);
      glVertex2f( 0.1f,  0.1f);
      glVertex2f(-0.1f, -0.1f);
      glEnd();
    }

    if (type == Type::DELETE) {
      glScalef(0.8f, 0.8f, 0.8f);
      Utils::renderIconNone();
    } else if (type == Type::MOVE) {
      glScalef(0.4f, 0.4f, 0.4f);
      glTranslatef(-1.2f, 1.0f, 0.0f);
      Utils::renderIconNone();
    }
    return true;
  }
}
