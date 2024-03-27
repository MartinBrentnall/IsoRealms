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

#include <sol.hpp>

#include "IsoRealms/Types.h"
#include "IsoRealms/Assets/Type/IBinding.h"
#include "IsoRealms/Persistence/DOMNode.h"
#include "IsoRealms/Persistence/DOMNodeWriter.h"
#include "IsoRealms/IAssetIdentifier.h"
#include "IsoRealms/IAssets.h"

namespace IsoRealms {
  class BoundAction : public IBinding {
    public:
    BoundAction(IProject* project, DOMNode& node);
    
    IAction* getValue();

    /***********************\
     * Implements IBinding *
    \***********************/
    bool renderAssetIcon() const override;
    void bind(const std::string& bindFunction) const override;
    void save(DOMNodeWriter* node, IBindingRegistry* localBindings, IAssetIdentifier* identifier, const std::string& attribute) const override;
    void release(IAssets* releaser) override;

    private:
    
    // DOM strings.
    static const std::string TAG_ACTION;
    
    // Definition data.
    sol::state* cDefLuaState;
    Action cDefAction;
  };
}
