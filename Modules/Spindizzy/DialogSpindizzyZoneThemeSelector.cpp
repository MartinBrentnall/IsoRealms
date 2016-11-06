/*
 * Copyright 2015 Martin Brentnall
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
#include "DialogSpindizzyZoneThemeSelector.h"

DialogSpindizzyZoneThemeSelector::DialogSpindizzyZoneThemeSelector(IComponentContainer* container, IResourceAccessor* resources, ISpindizzyZoneThemeAccessor* spindizzyZoneThemeAccessor) : Dialog(container, "Modules/Spindizzy/DialogSpindizzyZoneThemeSelector", resources) {
  cSpindizzyZoneThemeBrowser = new ComponentCustomResourceBrowser<SpindizzyZoneTheme, SpindizzyZoneThemeIcon>(resources, nullptr, this, 0.0f);
  addComponent("zoneThemes", cSpindizzyZoneThemeBrowser);
  std::map<std::string, SpindizzyZoneTheme*> mZoneThemes = spindizzyZoneThemeAccessor->getSpindizzyZoneThemes();
  for (std::pair<std::string, SpindizzyZoneTheme*> mZoneTheme : mZoneThemes) {
    cSpindizzyZoneThemeBrowser->resourceAdded(mZoneTheme.second);
  }
  cSpindizzyZoneThemeBrowser->addResourceSelectionListener(this);
  cSpindizzyZoneThemeAccessor = spindizzyZoneThemeAccessor;
}

std::string DialogSpindizzyZoneThemeSelector::getPath(IResource* resource) {
  std::map<std::string, SpindizzyZoneTheme*> mZoneThemes = cSpindizzyZoneThemeAccessor->getSpindizzyZoneThemes();
  for (std::pair<std::string, SpindizzyZoneTheme*> mZoneTheme : mZoneThemes) {
    if (resource == mZoneTheme.second) {
      return mZoneTheme.first;
    }
  }
  return "<Theme name not found>";
}

void DialogSpindizzyZoneThemeSelector::resourceSelected(SpindizzyZoneTheme* spindizzyZoneTheme) {
  cSpindizzyZoneThemeAccessor->spindizzyZoneThemeSelected(spindizzyZoneTheme);
}



