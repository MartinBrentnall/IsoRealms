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

#include <iostream>
#include <mutex>

#include "IsoRealms/IModuleHandle.h"
#include "IsoRealms/IsoRealmsConstants.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/ResourceTypeDefinition.h"

#include "Player/Player.h"
#include "Recorder/Recorder.h"

namespace IsoRealms::Replay {
  class Replay : public IModuleHandle {
    private:
    static const std::string ID_RESOURCE_PLAYBACK;
    static const std::string ID_RESOURCE_RECORDER;
      
    ResourceTypeDefinition<Replay, Player>   cResourceTypePlayer;
    ResourceTypeDefinition<Replay, Recorder> cResourceTypeRecorder;
        
    public:
    Replay(IProject& project, IResourceTypeRegistry* registry);

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void load(IProject& project, JSONObject object) override;
    void save(JSONObject object, IAssetIdentifier& identifier) override;
    void registerAssets(IAssetRegistry& assets) override;
    std::vector<std::unique_ptr<IProperty>> getProperties() override;
  };
}
