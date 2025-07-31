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

#include <map>
#include <string>
#include <vector>

#include "IsoRealms.h"

namespace IsoRealms::HighScore {
  class HighScore;
  
  class ScoreTable {
    public:
    
    /**********************\
     * Resource interface *
    \**********************/
    ScoreTable(IProject& project, HighScore& highScore, IResourceData& data);
    ScoreTable(IProject& project, HighScore& highScore, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);  
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    void getProperties(PropertyMaker& owner, const Metadata& metadata);
    
    /***********************\
     * Scripting Interface *
    \***********************/
    void reload();
    
    private:
    static const std::string JSON_COMPARE;
    static const std::string JSON_FIELD;
    static const std::string JSON_FIELDS;
    static const std::string JSON_HIGH_SCORE_TABLE;
    static const std::string JSON_NAME;
    static const std::string JSON_PROJECT;
    static const std::string JSON_RECORD_LIMIT;
    static const std::string JSON_RECORDS;
    static const std::string JSON_USER;
    static const std::string JSON_VALUE;
    static const std::string JSON_VALUES;

    String cProjectDataPath;
    Boolean cProjectUser;
    
    std::map<std::string, std::vector<LiteralString>> cValues;

    LuaBinding<ScoreTable> cLuaBinding;

    void writeDefaultTable();
    void readRecords(JSONObject object);
  };
}
