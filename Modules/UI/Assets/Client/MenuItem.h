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

#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/Persistence/JSONDocument.h"

#include "Modules/UI/Assets/Type/IMenuItem.h"

namespace IsoRealms::UI {
  class UI;
  class Menu;

  class MenuItem : public IAssetUser<IMenuItem> {
    public:
    MenuItem(UI* ui);

    void init(JSONObject object, const std::string& member, Menu* owner);
    void set(JSONObject object, const std::string& member, Menu* owner);
    void save(JSONObject object, const std::string& name) const;

    IMenuItem* operator->() const {
      return cMenuItem;
    }

    IMenuItem* operator*() const {
      return cMenuItem;
    }

    /************************************\
     * Implements IAssetUser<IMenuItem> *
    \************************************/
    void relinquish(IMenuItem* asset) override;

    virtual ~MenuItem();

    private:
    UI* cUI;
    IMenuItem* cMenuItem;

    MenuItem(MenuItem const& MenuItem) = delete;
    MenuItem& operator=(MenuItem const& MenuItem) = delete;
  };
}

