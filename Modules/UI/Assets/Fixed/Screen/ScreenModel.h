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

#include <cmath>
#include <queue>

#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::UI {
  class ScreenModel final : public IScreen {
    public:
    
    // Constructors.
    ScreenModel(IProject& project, IResourceData& owner);
    ScreenModel(IProject& project, IResourceData& owner, JSONObject object);

    void updateRuntime(unsigned int milliseconds);
    void updateEditing(unsigned int milliseconds);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /*********************************\
     * Implements IAsset via IScreen *
    \*********************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    
    // JSON members.
    static const std::string JSON_MODEL;

    // Definition data.
    Model cDefModel;
    std::unique_ptr<ModelInstance> cDefModelInstance;
  };
}
