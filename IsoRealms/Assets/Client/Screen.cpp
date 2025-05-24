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
#include "Screen.h"

#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms {
  Screen::Screen(IProject& project) : 
            Asset<IScreen, IProject>(project, project.createLiteralScreen(this)) {
  }

  Screen::Screen(const Screen& screen) :
            Asset<IScreen, IProject>(screen.cManager, screen.cManager.createLiteralScreen(this)) {
    setID(screen.getID());
  }

  IScreen* Screen::createLiteralAsset(IProject& project) {
    return project.createLiteralScreen(this);
  }
  
  IScreen* Screen::getAsset(IProject& project, JSONObject object) {
    return project.getScreen(this, object);
  }
  
  IScreen* Screen::getAsset(IProject& project, const std::string& id) {
    return project.getScreen(this, id);
  }
  
  std::vector<std::string> Screen::getAvailableProviders() const {
    return cManager.getAllScreens();
  }  

  bool Screen::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderScreenIcon(id);
  }

  bool Screen::hasConfiguration() const {
    return cManager.isScreenConfigurable(getID());
  }  

  bool Screen::isDefaultConfiguration() const {
    return true;
  }
}
