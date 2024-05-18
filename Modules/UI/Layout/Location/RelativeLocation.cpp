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
#include "RelativeLocation.h"

#include "Modules/UI/Layout/Layout.h"

namespace IsoRealms::UI {
  const std::string RelativeLocation::JSON_RELATIVE = "relative";
  const std::string RelativeLocation::JSON_TYPE     = "type";
  const std::string RelativeLocation::JSON_VALUE    = "value";

  const std::string RelativeLocation::TYPE_RELATIVE = "Relative";

  RelativeLocation::RelativeLocation(IProject* project, JSONObject object, Layout* layout, bool horizontal, float defaultValue) :
            cDefRelative(nullptr),
            cDefHorizontal(horizontal),
            cDefValue(object.getFloat(JSON_VALUE, defaultValue)) {
    project->init([this, object, layout](IAssets* assets) {
      cDefRelative = layout->getComponent(object.getString(JSON_RELATIVE));
    });
  }

  float RelativeLocation::getLocation(float aspectRatio) const {
    float mComponentStart = cDefHorizontal ? cDefRelative->getLeft(aspectRatio)  : cDefRelative->getBottom();
    float mComponentEnd   = cDefHorizontal ? cDefRelative->getRight(aspectRatio) : cDefRelative->getTop();
    return mComponentStart + ((mComponentEnd - mComponentStart) / 2.0f) * (cDefValue + 1.0f);
  }
  
  void RelativeLocation::save(JSONObject object, Layout* layout, float defaultValue) const {
    object.addString(JSON_TYPE, TYPE_RELATIVE);
    object.addFloat(JSON_VALUE, cDefValue, defaultValue);
    object.addString(JSON_RELATIVE, layout->getName(cDefRelative));
  }
}
