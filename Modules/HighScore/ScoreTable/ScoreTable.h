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

#include <map>
#include <string>
#include <vector>

#include "IsoRealms/Lua.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::HighScore {
  class HighScore;
  
  class ScoreTable {
    public:
    
    /**********************\
     * Resource interface *
    \**********************/
    ScoreTable(IProject* project, HighScore* highScore);
    ScoreTable(IProject* project, HighScore* highScore, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);  
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);
    
    /***********************\
     * Scripting Interface *
    \***********************/
    void reload();
    
    private:
    static const std::string TAG_FIELD;
    static const std::string TAG_HIGH_SCORE_TABLE;
    static const std::string TAG_PROJECT;
    static const std::string TAG_RECORD;
    static const std::string TAG_USER;

    static const std::string ATTRIBUTE_RECORDS;
    static const std::string ATTRIBUTE_NAME;
    static const std::string ATTRIBUTE_FIELD;
    static const std::string ATTRIBUTE_VALUE;

    String cProjectDataPath;
    Boolean cProjectUser;
    
    std::map<std::string, std::vector<LiteralString>> cValues;

    LuaBinding<ScoreTable> cLuaBinding;

    void writeDefaultTable();
    void readRecords(DOMNode& node);
  };
}
