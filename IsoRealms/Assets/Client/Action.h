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

#include <functional>

#include "IsoRealms/Assets/Type/IAction.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/JSONDocument.h"

namespace IsoRealms {
  class Action final : public IAssetUser<IAction> {
    public:
    Action(IProject* project);

    void init(JSONObject object, const std::string& member, IBindingRegistry* localArgs = nullptr, const std::string& id = "");
    void set(JSONObject object, const std::string& member, IBindingRegistry* localArgs = nullptr, const std::string& id = "");
    void execute();
    void save(JSONObject object, const std::string& name) const;

    IAction* operator*() const {
      return cAction;
    }

    /**********************************\
     * Implements IAssetUser<IAction> *
    \**********************************/
    void relinquish(IAction* asset) override;

    virtual ~Action();

    private:
    IProject* cProject;
    IAction* cAction;

    Action(Action const& action) = delete;
    Action& operator=(Action const& action) = delete;
  };
}
