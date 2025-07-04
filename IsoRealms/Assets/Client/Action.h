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
#include "IsoRealms/IActionClient.h"

#include "Asset.h"

namespace IsoRealms {
  class Action : public Asset<Action, IAction, IActionClient> {
    public:
    Action(IActionClient& owner);
    void execute();

    /****************************************************\
     * Implements Asset<Action, IAction, IActionClient> *
    \****************************************************/
    IAction* createLiteralAsset(IActionClient& owner);
    IAction* getAsset(IActionClient& owner, JSONObject object);
    IAction* getAsset(IActionClient& owner, const std::string& id);
    bool isDefaultConfiguration() const;
  };
}
