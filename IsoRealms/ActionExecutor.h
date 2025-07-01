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

#include <memory>

#include "Assets/Client/ActionType.h"
#include "Types.h"

namespace IsoRealms {
  class Project;

  /**
    * A wrapper that controls execution of an action.  Specifically, if the
    * Project is currently handling input, the action executor postpones
    * execution of the action until the beginning of the next update cycle.
    */
  class ActionExecutor {
    public:

    /**
      * Construct the ActionExecutor.
      *
      * @param project The parent Project.
      * @param action The action to execute/postpone.
      */
    ActionExecutor(Project& project, IAssetUser<ActionExecutor>* user);
    ActionExecutor(Project& project, IAssetUser<ActionExecutor>* user, const std::string& id);
    ActionExecutor(Project& project, IAssetUser<ActionExecutor>* user, JSONObject object, IBindingRegistry* localArgs);

    ~ActionExecutor();

    void execute();
    void postpone();
    std::string getID() const;
    void save(JSONObject object) const;
    bool renderAssetIcon() const;
    bool hasConfiguration() const;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties();
    bool isDefaultConfiguration() const;

    private:
    Project& cProject;                 /// Parent Project.
    ActionType cActionType;            /// Type from which the action is derived.
    IAction* cAction;                  /// Action to be executed.
    IAssetUser<ActionExecutor>* cUser; /// Client of this action.
  };
}
