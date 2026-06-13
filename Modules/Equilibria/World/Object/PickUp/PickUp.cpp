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
#include "PickUp.h"

#include "Modules/Equilibria/PickUpType/PickUpType.h"
#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/World.h"

namespace IsoRealms::Equilibria {
  PickUp::PickUp(Zone& zone, PickUpType& type, int x, int y, int z) :
            cZone(zone),
            cDefType(&type),
            cDefModel(cDefType->createModel()),
            cDefX(x),
            cDefY(y),
            cDefZ(z),
            cLuaBinding(zone.getWorld().getEquilibria().getProject().getLuaState(), this) {
    reset();
  }

  PickUp::PickUp(Zone& zone, PickUp& pickUp, int x, int y, int z) :
            cZone(zone),
            cDefType(pickUp.cDefType),
            cDefModel(cDefType->createModel()),
            cDefX(pickUp.cDefX + x),
            cDefY(pickUp.cDefY + y),
            cDefZ(pickUp.cDefZ + z),
            cLuaBinding(zone.getWorld().getEquilibria().getProject().getLuaState(), this) {
    reset();
  }

  PickUp::PickUp(Zone& zone, JSONObject object) :
            cZone(zone),
            cDefX(object.getInteger(JSON_X) + cZone.getStartX()),
            cDefY(object.getInteger(JSON_Y) + cZone.getStartY()),
            cDefZ(object.getInteger(JSON_Z) + cZone.getStartZ()),
            cLuaBinding(zone.getWorld().getEquilibria().getProject().getLuaState(), this) {
    cZone.getWorld().getEquilibria().getProject().init([this, object]() {
      cDefType = cZone.getWorld().getEquilibria().get<PickUpType>(nullptr, object.getString(JSON_TYPE));
      cDefModel = cDefType->createModel();
      reset();
    });
  }

  void PickUp::removed() {
    cZone.getWorld().unregisterBoundary(cDefType, this);
  }

  void PickUp::initialise() {
    cZone.getWorld().registerBoundary(cDefType, this, cDefX, cDefX, cDefY, cDefY);
  }

  void PickUp::reset() {
    cRuntimePresent = true;
  }

  void PickUp::save(JSONObject object, int x, int y, int z) const {
    object.addString(JSON_TYPE, cZone.getWorld().getEquilibria().getComponentID(cDefType));
    object.addInteger(JSON_X,   cDefX - x);
    object.addInteger(JSON_Y,   cDefY - y);
    object.addInteger(JSON_Z,   cDefZ - z);
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
//     return id == BIND_TO_ZONE ? cZone.getBinding(mSubID)
//          :                      nullptr;
    return nullptr;
  }
  
  void PickUp::bindValues() {
    cZone.bindValues();
  }

  void PickUp::unbindValues() {
    cZone.unbindValues();
  }

  bool PickUp::contains(const LiteralVertex& location) const {
    return location.x >= cDefX - 0.5f && location.x <= cDefX + 0.5f && location.y >= cDefY - 0.5f && location.y <= cDefY + 0.5f && location.z >= cDefZ * 0.5f && location.z <= (cDefZ + 2.0f) * 0.5f;
  }

  void PickUp::renderSelectionHighlight() const {
    glColor4f(0.5f, 0.0f, 1.0f, 0.5f);
    Utils::renderVolumeCuboid(cDefX - 0.5f, cDefX + 0.5f, cDefY - 0.5f, cDefY + 0.5f, cDefZ * 0.5f, (cDefZ + 2.0f) * 0.5f);
  }

  void PickUp::remove() {
    cZone.remove(this);
  }

  void PickUp::getProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  std::string PickUp::getTypeName() const {
    return "Pick Up";
  }

  Zone& PickUp::getObjectZone() {
    return cZone;
  }
}
