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
#include "PickUp.h"

#include "Modules/Spindizzy/PickUpType/PickUpType.h"
#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/World.h"

namespace IsoRealms::Spindizzy {
  const std::string PickUp::ATTRIBUTE_TYPE = "type";
  const std::string PickUp::ATTRIBUTE_X    = "x";
  const std::string PickUp::ATTRIBUTE_Y    = "y";
  const std::string PickUp::ATTRIBUTE_Z    = "z";

  const std::string PickUp::BIND_TO_ZONE = "Zone";

  PickUp::PickUp(Zone& zone, PickUpType* type, int x, int y, int z) :
            cDefZone(zone),
            cDefType(type),
            cDefModel(cDefType->createModel()),
            cDefX(x),
            cDefY(y),
            cDefZ(z),
            cLuaBinding(zone.getWorld()->getSpindizzy()->getProject(), this) {
    reset();
  }

  PickUp::PickUp(Zone& zone, DOMNode& node) :
            cDefZone(zone),
            cDefType(nullptr),
            cDefModel(nullptr),
            cDefX(node.getIntegerAttribute(ATTRIBUTE_X) + cDefZone.getStartX()),
            cDefY(node.getIntegerAttribute(ATTRIBUTE_Y) + cDefZone.getStartY()),
            cDefZ(node.getIntegerAttribute(ATTRIBUTE_Z) + cDefZone.getStartZ()),
            cLuaBinding(zone.getWorld()->getSpindizzy()->getProject(), this) {
    cDefZone.getWorld()->getSpindizzy()->getProject()->init([this, node](IAssets* assets) {
      cDefType = cDefZone.getWorld()->getSpindizzy()->getPickUpType(node.getAttribute(ATTRIBUTE_TYPE));
      cDefModel = cDefType->createModel();
      reset();
    });
  }

  void PickUp::initialise() {
    cDefZone.getWorld()->registerBoundary(cDefType, this, cDefX, cDefX, cDefY, cDefY);
  }

  void PickUp::reset() {
    cRuntimePresent = true;
  }

  void PickUp::save(DOMNodeWriter* node, int x, int y, int z) const {
    node->addAttribute(ATTRIBUTE_TYPE, cDefZone.getWorld()->getSpindizzy()->getID(cDefType));
    node->addAttribute(ATTRIBUTE_X,    cDefX - x);
    node->addAttribute(ATTRIBUTE_Y,    cDefY - y);
    node->addAttribute(ATTRIBUTE_Z,    cDefZ - z);
  }

  bool PickUp::isType(const PickUpType* const type) const {
    return cDefType == type;
  }

  void PickUp::updateRuntime(unsigned int milliseconds) {
    if (cRuntimePresent) {
      cDefModel->update(milliseconds);
    }
  }

  void PickUp::updateEditing(unsigned int milliseconds) {
    cDefModel->update(milliseconds);
  }

  void PickUp::renderRuntime() const {
    if (cRuntimePresent) {
      renderEditing();
    }
  }

  void PickUp::renderEditing() const {
    glPushMatrix();
    glTranslatef(cDefX, cDefY, cDefZ * 0.5f);
    cDefModel->render();
    glPopMatrix();
  }

  bool PickUp::isCollected() const {
    return !cRuntimePresent;
  }

  void PickUp::pickUp() {
    cRuntimePresent = false;
  }

  bool PickUp::contains(LiteralVertex& location) const {
    if (cRuntimePresent) {
      if (CollisionUtils::contains(location, cDefX - 0.5f, cDefX + 0.5f, cDefY - 0.5f, cDefY + 0.5f, cDefZ - 0.25f, cDefZ + 1.0f)) {
        // TODO: Collection should not be triggered as a side-effect of this function!  Actually this function should be const.
        cRuntimePresent = false;
        return true;
      }
    }
    return false;
  }

  bool PickUp::isEntered(LiteralVertex& start, LiteralVertex& end) const {
    if (cRuntimePresent) {
      if (CollisionUtils::hasCrossed(start, end, cDefX - 0.5f, cDefX + 0.5f, cDefY - 0.5f, cDefY + 0.5f, cDefZ - 0.25f, cDefZ + 1.0f, true)) {
        // TODO: Collection should not be triggered as a side-effect of this function!  Actually this function should be const.
        cRuntimePresent = false;
        return true;
      }
    }
    return false;
  }

  bool PickUp::isExited(LiteralVertex& start, LiteralVertex& end) const {
    return false; // Not supported for pick ups
  }

  IBinding* PickUp::getBinding(const std::string& id) const {
    // TODO
//     if (id.empty()) {
//       return &cLuaBinding;
//     }
//     std::size_t mSplit = id.find('/');
//     std::string mBindTo = mSplit == std::string::npos ? id : id.substr(0, mSplit);
//     std::string mSubID = mSplit == std::string::npos ? "" : id.substr(mSplit + 1);
//     return id == BIND_TO_ZONE ? cDefZone.getBinding(mSubID)
//          :                      nullptr;
    return nullptr;
  }
  
  void PickUp::bindValues() {
    cDefZone.bindValues();
  }

  void PickUp::unbindValues() {
    cDefZone.unbindValues();
  }

  bool PickUp::contains(const LiteralVertex& location) const {
    return location.x >= cDefX - 0.5f && location.x <= cDefX + 0.5f && location.y >= cDefY - 0.5f && location.y <= cDefY + 0.5f && location.z >= cDefZ * 0.5f && location.z <= (cDefZ + 2.0f) * 0.5f;
  }

  void PickUp::renderSelectionHighlight() const {
    glColor4f(0.5f, 0.0f, 1.0f, 0.5f);
    Utils::renderVolumeCuboid(cDefX - 0.5f, cDefX + 0.5f, cDefY - 0.5f, cDefY + 0.5f, cDefZ * 0.5f, (cDefZ + 2.0f) * 0.5f);
  }

  void PickUp::remove() {
    cDefZone.remove(this);
  }

  std::vector<std::unique_ptr<IProperty>> PickUp::getProperties(IPropertyAppearance* appearance) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  std::string PickUp::getTypeName() const {
    return "Pick Up";
  }

  Zone& PickUp::getObjectZone() {
    return cDefZone;
  }
}
