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

#include "IsoRealms/Assets/Type/IInputHandler.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/IAssets.h"
#include "IsoRealms/Persistence/JSONDocument.h"

namespace IsoRealms {
  class InputHandler : public IAssetUser<IInputHandler> {
    private:
    IProject* cProject;
    IInputHandler* cInputHandler;

    InputHandler(InputHandler const& inputHandler) = delete;
    InputHandler& operator=(InputHandler const& inputHandler) = delete;
    
    public:
    InputHandler(IProject* project);

    void init(JSONObject object, const std::string& member);
    void set(JSONObject object, const std::string& member);
    void save(JSONObject object, const std::string& name) const;

    IInputHandler* operator->() const {
      return cInputHandler;
    }

    IInputHandler* operator*() const {
      return cInputHandler;
    }

    /****************************************\
     * Implements IAssetUser<IInputHandler> *
    \****************************************/
    void relinquish(IInputHandler* asset) override;

    virtual ~InputHandler();
  };
}
