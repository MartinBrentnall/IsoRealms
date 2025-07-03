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
#include "IsoRealms/IProject.h"
#include "IsoRealms/IResourceData.h"

namespace IsoRealms {
  Screen::Screen(IResourceData& owner) : 
            Asset<Screen, IScreen, IResourceData>(owner, owner.getAssetManager().createLiteralScreen(this, owner)) {
  }

  void Screen::render(float scale, float aspectRatio) const {
    cManager.getProject().screenPreRender(cAsset);
    cAsset->renderScreen(scale, aspectRatio);
    cManager.getProject().screenPostRender(cAsset);
  }

  IScreen* Screen::createLiteralAsset(IResourceData& owner) {
    return owner.getAssetManager().createLiteralScreen(this, owner);
  }
  
  IScreen* Screen::getAsset(IResourceData& owner, JSONObject object) {
    return owner.getAssetManager().getScreen(this, object, owner);
  }
  
  IScreen* Screen::getAsset(IResourceData& owner, const std::string& id) {
    return owner.getAssetManager().getScreen(this, id, owner);
  }
  
  std::vector<std::string> Screen::getAvailableProviders() const {
    return cManager.getAssetManager().getAllScreens();
  }  

  bool Screen::renderOtherProviderIcon(const std::string& id) const {
    return cManager.getAssetManager().renderScreenIcon(id);
  }

  bool Screen::hasConfiguration() const {
    return cManager.getAssetManager().isScreenConfigurable(getID());
  }  

  bool Screen::isDefaultConfiguration() const {
    return true;
  }
}
