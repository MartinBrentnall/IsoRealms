/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <iostream>
#include <mutex>

#include "IsoRealms.h"

#include "HighScoreTable.h"
#include "ScoreTable/ScoreTable.h"
#include "ScoreTracker/ScoreTracker.h"

namespace IsoRealms::HighScore {
  class HighScore : public IModuleHandle {
    public:
    HighScore(Project& project, IResourceTypeRegistry* registry);

    /****************************\
     * Implements IModuleHandle *
    \****************************/
    void load(IProject& project, JSONObject object) override;
    void save(JSONObject object) override;
    void registerAssets(ResourceAssetRegistry& assets) override;
    std::vector<std::unique_ptr<IProperty>> getProperties() override;
    void updateRuntime(unsigned int milliseconds) override;
    void updateEditing(unsigned int milliseconds) override;
    void reset() override;
      
    private:
    ResourceTypeDefinition<HighScore, ScoreTable>   cResourceTypeScoreTable;
    ResourceTypeDefinition<HighScore, ScoreTracker> cResourceTypeScoreTracker;
  };
}
