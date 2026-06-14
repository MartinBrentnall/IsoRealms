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
#include "Spinner.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/Object/Zone/Zone.h"
#include "Modules/Equilibria/World/Object/ZoneObject/ZoneObject.h"
#include "Modules/Equilibria/World/World.h"
#include "Modules/Equilibria/Resources/Fixed/ZoneObjectTypeTrait/ZoneObjectTypeTraitSpinner.h"

namespace IsoRealms::Equilibria {
  Spinner::Spinner(ZoneObject& object, ZoneObjectTypeTraitSpinner& type) :
            cDefType(type),
            cLuaBinding(object.getZone().getWorld().getEquilibria().getProject().getLuaState(), this) {
    object.getZone().getWorld().getEquilibria().getProject().init([this, &object]() {
      std::string mLocationID = cDefType.getLocationID();
      cDefLocation = object.getLocation(mLocationID);
      cDefModel = cDefType.createModel();
    });
  }
  
  void Spinner::publish(ITraitRegistry& registry) {
    registry.registerRenderer(this);
    registry.registerProcessor(this);
  }
  
  void Spinner::setEnabled(bool enabled) {
    cRuntimeEnabled = enabled;
  }
  
  bool Spinner::isEnabled() const {
    return cRuntimeEnabled;
  }
  
  void Spinner::save(JSONObject object) const {
    // Nothing to do.
  }

  bool Spinner::hasConfiguration() const {
    return false;
  }    
  
  void Spinner::reset() {
    cRuntimeEnabled = true; // TODO cDefType.isEnabled();
  }
    
  IBinding* Spinner::getTraitBinding(const std::string& id) {
    return nullptr;
  }
  
  void Spinner::render() const {
    if (cRuntimeEnabled) {
      glPushMatrix();
      glTranslatef(cDefLocation->getX(), cDefLocation->getY(), cDefLocation->getZ() * 0.5f);
      glRotatef((cDefLocation->getX() + cDefLocation->getY()) * cDefType.getSpinSpeed(), 0.0f, 0.0f, 1.0f);
      cDefModel->render();
      glPopMatrix();
    }
  }
  
  void Spinner::update(unsigned int milliseconds) {
    if (cRuntimeEnabled) {
      cDefModel->update(milliseconds);
    }
  }
}
