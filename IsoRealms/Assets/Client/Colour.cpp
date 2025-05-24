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
#include "Colour.h"

#include "IsoRealms/Editing/Property/IProperty.h"

namespace IsoRealms {
  Colour::Colour(IProject& project, float defaultRed, float defaultGreen, float defaultBlue, float defaultAlpha, std::function<void()> listener) :
            Asset<IColour, IProject>(project, project.createLiteralColour(this, defaultRed, defaultGreen, defaultBlue, defaultAlpha)),
            cDefaultRed(defaultRed),
            cDefaultGreen(defaultGreen),
            cDefaultBlue(defaultBlue),
            cDefaultAlpha(defaultAlpha),
            cListener(listener) {
  }

  IColour* Colour::createLiteralAsset(IProject& project) {
    return project.createLiteralColour(this, cDefaultRed, cDefaultGreen, cDefaultBlue, cDefaultAlpha);
  }
  
  IColour* Colour::getAsset(IProject& project, JSONObject object) {
    return project.getColour(this, object, cListener != nullptr ? this : nullptr);
  }
  
  IColour* Colour::getAsset(IProject& project, const std::string& id) {
    return project.getColour(this, id, cListener != nullptr ? this : nullptr);
  }
  
  std::vector<std::string> Colour::getAvailableProviders() const {
    return cManager.getAllColours();
  }  

  bool Colour::renderOtherProviderIcon(const std::string& id) const {
    return cManager.renderColourIcon(id);
  }

  bool Colour::hasConfiguration() const {
    return cManager.isColourConfigurable(getID());
  }

  bool Colour::isDefaultConfiguration() const {
    return true;
  }

  void Colour::stateChanged(IColour* value) {
    if (value == cAsset && cListener != nullptr) {
      cListener();
    }
  }
}
