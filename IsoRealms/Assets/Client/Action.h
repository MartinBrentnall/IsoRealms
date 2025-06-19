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

#include "IsoRealms/ActionExecutor.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/JSONDocument.h"

namespace IsoRealms {
  class Action final : public IAssetUser<ActionExecutor> {
    public:
    Action(IResourceData& owner);

    void init(JSONObject object, const std::string& member, IBindingRegistry* localArgs = nullptr);
    void set(JSONObject object, const std::string& member, IBindingRegistry* localArgs = nullptr);
    void setID(const std::string& id);
    void execute();
    std::string getID() const;
    std::vector<std::string> getAvailableProviders() const;
    bool renderProviderIcon(const std::string& id) const;
    bool hasConfiguration() const;
    bool isDefaultConfigured() const;
    bool renderAssetIcon() const;
    void save(JSONObject object, const std::string& name) const;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties();

    ActionExecutor* operator*() const {
      return cAction;
    }

    ActionExecutor* operator->() const {
      return cAction;
    }

    IApplication& getApplication() {
      return cProject.getApplication();
    }

    /*****************************************\
     * Implements IAssetUser<ActionExecutor> *
    \*****************************************/
    void relinquish(ActionExecutor* asset) override;

    virtual ~Action();

    private:
    IProject& cProject;
    IResourceData& cManager;
    ActionExecutor* cAction;

    Action(Action const& action) = delete;
    Action& operator=(Action const& action) = delete;
  };
}
