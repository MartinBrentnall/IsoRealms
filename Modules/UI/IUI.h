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

#include <string>
#include <vector>

#include "IsoRealms/Assets/Registry/IAssetUser.h"
#include "IsoRealms/Persistence.h"

namespace IsoRealms::UI {
  class ILayoutLocation;
  class ILayoutOffset;
  class IMenuItem;
  class LayoutComponentEdge;
  class Menu;

  class IUI {
    public:
    virtual std::vector<std::string> getAllLayoutLocations() = 0;
    virtual std::vector<std::string> getAllLayoutOffsets()   = 0;
    virtual std::vector<std::string> getAllMenuItems()       = 0;

    virtual std::string getID(const ILayoutLocation* asset) const = 0;
    virtual std::string getID(const ILayoutOffset*   asset) const = 0;
    virtual std::string getID(const IMenuItem*       asset) const = 0;

    virtual bool renderLayoutLocationIcon(const std::string& id) const = 0;
    virtual bool renderLayoutOffsetIcon(  const std::string& id) const = 0;
    virtual bool renderMenuItemIcon(      const std::string& id) const = 0;

    virtual bool isLayoutLocationConfigurable(const std::string& id) const = 0;
    virtual bool isLayoutOffsetConfigurable(  const std::string& id) const = 0;
    virtual bool isMenuItemConfigurable(      const std::string& id) const = 0;

    virtual ILayoutLocation* createLiteralLayoutLocation(IAssetUser<ILayoutLocation>* user, LayoutComponentEdge& owner) = 0;
    virtual ILayoutOffset*   createLiteralLayoutOffset(  IAssetUser<ILayoutOffset>*   user, LayoutComponentEdge& owner) = 0;
    virtual IMenuItem*       createLiteralMenuItem(      IAssetUser<IMenuItem>*       user, Menu&                owner) = 0;

    virtual ILayoutLocation* getLayoutLocation(IAssetUser<ILayoutLocation>* user, JSONObject object, LayoutComponentEdge& owner) = 0;
    virtual ILayoutOffset*   getLayoutOffset(  IAssetUser<ILayoutOffset>*   user, JSONObject object, LayoutComponentEdge& owner) = 0;
    virtual IMenuItem*       getMenuItem(      IAssetUser<IMenuItem>*       user, JSONObject object, Menu&                owner) = 0;

    virtual ILayoutLocation* getLayoutLocation(IAssetUser<ILayoutLocation>* user, const std::string& id, LayoutComponentEdge& owner) = 0;
    virtual ILayoutOffset*   getLayoutOffset(  IAssetUser<ILayoutOffset>*   user, const std::string& id, LayoutComponentEdge& owner) = 0;
    virtual IMenuItem*       getMenuItem(      IAssetUser<IMenuItem>*       user, const std::string& id, Menu&                owner) = 0;

    virtual void release(IAssetUser<ILayoutLocation>* user, ILayoutLocation* asset) = 0;
    virtual void release(IAssetUser<ILayoutOffset>*   user, ILayoutOffset*   asset) = 0;
    virtual void release(IAssetUser<IMenuItem>*       user, IMenuItem*       asset) = 0;

    virtual void save(JSONObject object, ILayoutLocation* asset) const = 0;
    virtual void save(JSONObject object, ILayoutOffset*   asset) const = 0;
    virtual void save(JSONObject object, IMenuItem*       asset) const = 0;

    virtual IUI& getAssetManager() = 0;
    virtual IProject& getProject() const = 0;

    virtual bool isReadOnly() const = 0; // TODO: Probably shouldn't be here.
    virtual void setOwner(File* owner) = 0; // TODO: Probably shouldn't be here.

    virtual ~IUI() {}
  };
}

