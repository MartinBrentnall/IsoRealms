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
#pragma once

namespace IsoRealms {
  class IAssetRegistry;
  class IAssetRemover;
  class IAssets;

  namespace UI {
    class Menu;

    class IMenuItem {
      public:
      virtual void registerAssets(IAssetRegistry* assets) = 0;
      virtual void unregisterAssets(IAssetRemover* assets, IAssets* releaser) = 0;
      virtual void save(JSONObject object) const = 0;
      virtual bool input(sf::Event& event) = 0;
      virtual void selectTop() = 0;
      virtual void selectBottom() = 0;
      virtual void render(float aspectRatio, float y, bool selected, const Menu& menu) const = 0;
      virtual float getHeight(const Menu& menu) const = 0;
      virtual float getSelectedY(const Menu& menu) const = 0;

      virtual ~IMenuItem() {
      }
    };
  }
}
