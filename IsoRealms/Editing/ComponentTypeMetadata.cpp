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
#include "ComponentTypeMetadata.h"

#include "IsoRealms/IComponentDefiner.h"
#include "IsoRealms/Project/ComponentType.h"
#include "IsoRealms/Project/Module.h"
#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Persistence/JSONThing.h"
#include "PropertyData.h"

namespace IsoRealms {
  ComponentTypeMetadata::ComponentTypeMetadata(JSONObject object) :
            cSingular(   object.getString("singular")),
            cPlural(     object.getString("plural")),
            cCategory(   object.getString("category")),
            cDescription(object.getString("description")),
            cProperties( object.getObject("properties")) {
  }

  void ComponentTypeMetadata::scope(IComponentDefiner& definer, Module& module, const std::string& componentType, const std::string& category) {
    if (cCategory == category) {
      definer.scope(componentType, cDescription, [this, &module, componentType, category](IComponentDefiner& definer) {
        module.getComponentType(componentType)->define(definer);
      });
    }
  }
}
  