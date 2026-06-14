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
#include "ZoneViewTypeOverview.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/WorldView/WorldView.h"

#include "ZoneViewOverview.h"

namespace IsoRealms::Equilibria {
  ZoneViewTypeOverview::ZoneViewTypeOverview(const Metadata& metadata, WorldView& worldView) :
            cWorldView(worldView),
            cDefColour(worldView.getComponentData(), 1.0f, 0.0f, 0.0f, 0.0f),
            cRuntimeParameterView1(worldView.getEquilibria().getProject().getLuaState(), nullptr, nullptr, true),
            cRuntimeParameterView2(worldView.getEquilibria().getProject().getLuaState(), nullptr, nullptr, true) {
  }

  ZoneViewTypeOverview::ZoneViewTypeOverview(const Metadata& metadata, WorldView& worldView, JSONObject object) :
            ZoneViewTypeOverview(metadata, worldView) {
    cDefColour.init(object, JSON_COLOUR);
  }

  void ZoneViewTypeOverview::publish(Equilibria& equilibria, const std::string& parentID) {
    equilibria.addZoneBinding(&cRuntimeParameterView1, &cRuntimeParameterView2, parentID);
  }

  void ZoneViewTypeOverview::bind1(ZoneViewOverview* zoneViewOverview) {
    cRuntimeParameterView1.setValue(zoneViewOverview);
  }

  void ZoneViewTypeOverview::bind2(ZoneViewOverview* zoneViewOverview) {
    cRuntimeParameterView2.setValue(zoneViewOverview);
  }

  const IColour* ZoneViewTypeOverview::getInitialColour() const {
    return *cDefColour;
  }
  
  std::unique_ptr<IZoneView> ZoneViewTypeOverview::createZoneView(Zone* zone) {
    return std::make_unique<ZoneViewOverview>(cWorldView, *this, zone);
  }

  ZoneViewTypeOverview::ZoneViewTypeOverview(const Metadata& metadata, WorldView& worldView, JSONObject object) :
            ZoneViewTypeOverview(metadata, worldView) {
    cDefColour.init(object, JSON_COLOUR);
  }

}
