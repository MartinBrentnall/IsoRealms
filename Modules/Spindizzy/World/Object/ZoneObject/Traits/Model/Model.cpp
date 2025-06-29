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
#include "Model.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Zone/Zone.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/ZoneObject.h"
#include "Modules/Spindizzy/World/World.h"
#include "Modules/Spindizzy/Assets/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitModel.h"

namespace IsoRealms::Spindizzy {
  Model::Model(ZoneObject& object, ZoneObjectTypeTraitModel& type) :
            cDefType(type),
            cLuaBinding(object.getZone().getWorld().getSpindizzy().getProject(), this) {
    object.getZone().getWorld().getSpindizzy().getProject().init([this, &object](IAssets& assets) {
      std::string mLocationID = cDefType.getLocationID();
      cDefLocation = object.getLocation(mLocationID);
      cDefModel = cDefType.createModel();
    });
  }
  
  void Model::setEnabled(bool enabled) {
    cRuntimeEnabled = enabled;
  }
  
  bool Model::isEnabled() const {
    return cRuntimeEnabled;
  }
  
  void Model::registerAssets(ITraitRegistry& registry) {
    registry.registerRenderer(this);
    registry.registerProcessor(this);
  }
  
  void Model::save(JSONObject object) const {
    // Nothing to do.
  }

  bool Model::hasConfiguration() const {
    return false;
  }    
  
  void Model::reset() {
    cRuntimeEnabled = cDefType.isInitiallyEnabled();
  }
    
  IBinding* Model::getTraitBinding(const std::string& id) {
    return id.empty() ? &cLuaBinding : nullptr;
  }
  
  void Model::render() const {
    if (cRuntimeEnabled) {
      glPushMatrix();
      glTranslatef(cDefLocation->getX(), cDefLocation->getY(), cDefLocation->getZ() * 0.5f);
      cDefModel->render();
      glPopMatrix();
    }
  }  
  
  void Model::update(unsigned int milliseconds) {
    if (cRuntimeEnabled) {
      cDefModel->update(milliseconds);
    }
  }
}
