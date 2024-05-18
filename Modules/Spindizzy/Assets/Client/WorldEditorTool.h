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

#include "IsoRealms/Persistence/JSONDocument.h"

#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;
  
  class WorldEditorTool : public IAssetUser<IWorldEditorTool> {
    public:
    WorldEditorTool(Spindizzy* spindizzy);

    void init(JSONObject object);
    void set(JSONObject object);
    void save(JSONObject object, const std::string& name) const;

    IWorldEditorTool* operator->() const {
      return cWorldEditorTool;
    }

    IWorldEditorTool* operator*() const {
      return cWorldEditorTool;
    }

    /*******************************************\
     * Implements IAssetUser<IWorldEditorTool> *
    \*******************************************/
    void relinquish(IWorldEditorTool* asset) override;

    virtual ~WorldEditorTool();

    private:
    Spindizzy* cSpindizzy;
    IWorldEditorTool* cWorldEditorTool;

    WorldEditorTool(WorldEditorTool const& WorldEditorTool) = delete;
    WorldEditorTool& operator=(WorldEditorTool const& WorldEditorTool) = delete;
  };
}
