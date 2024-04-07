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
#include "ZoneViewTypeOverview.h"

#include "ZoneViewOverview.h"

#include "Modules/Spindizzy/ISpindizzyRegistry.h"
#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/WorldView/WorldView.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneViewTypeOverview::TAG_COLOUR = "Colour";
  
  ZoneViewTypeOverview::ZoneViewTypeOverview(IProject* project, WorldView* worldView, DOMNode& node) :
            cProject(project),
            cWorldView(worldView),
            cDefColour(project, 1.0f, 0.0f, 0.0f, 0.0f),
            cRuntimeParameterView1(project, nullptr, worldView->getSpindizzy()),
            cRuntimeParameterView2(project, nullptr, worldView->getSpindizzy()) {
    cDefColour.init(node, TAG_COLOUR);
  }

  void ZoneViewTypeOverview::bind1(ZoneViewOverview* zoneViewOverview) {
    cRuntimeParameterView1.setValue(zoneViewOverview);
  }

  void ZoneViewTypeOverview::bind2(ZoneViewOverview* zoneViewOverview) {
    cRuntimeParameterView2.setValue(zoneViewOverview);
  }

  const IColour* ZoneViewTypeOverview::getInitialColour() const {
    return &cDefColour;
  }
  
  std::unique_ptr<IZoneView> ZoneViewTypeOverview::createZoneView(Zone* zone) {
    return std::make_unique<ZoneViewOverview>(cProject, cWorldView, *this, zone);
  }

  void ZoneViewTypeOverview::registerAssets(ISpindizzyRegistry* registry) {
    registry->addZoneBinding(&cRuntimeParameterView1, &cRuntimeParameterView2, "");
  }

  bool ZoneViewTypeOverview::renderAssetIcon() const {
    return false;
  }

  void ZoneViewTypeOverview::saveAsset(DOMNodeWriter* node) const {
    cDefColour.save(node, TAG_COLOUR);
  }
}
