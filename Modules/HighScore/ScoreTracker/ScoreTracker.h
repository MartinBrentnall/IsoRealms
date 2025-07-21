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

#include <vector>

#include "IsoRealms.h"

#include "Modules/HighScore/HighScoreTable.h"

namespace IsoRealms::HighScore {
  class HighScore;
  
  class ScoreTracker final : public IScreen,
                             public IInputHandler {
    public:
    
    /**********************\
     * Resource interface *
    \**********************/
    ScoreTracker(IProject& project, HighScore& highScore, IResourceData& data);
    ScoreTracker(IProject& project, HighScore& highScore, IResourceData& data, JSONObject object, IOptions& options);
    std::vector<std::unique_ptr<IProperty>> getProperties(IPropertyOwner& owner);
    void registerAssets(ResourceAssetRegistry& assets);  
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;

    /*********************\
     * Module interfaces *
    \*********************/
    void updateRuntime(unsigned int milliseconds);
    void reset();
      
    /***********************\
     * Scripting Interface *
    \***********************/
    void writeValues();
    void quit();
    
    /*********************\
     * Implements IScreen *
    \*********************/
    void renderScreen(float scale, float aspectRatio) const override;

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event&) override;
    void resetInput() override;

    /*********************\
     * Implements IAsset *
    \*********************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_COMPARE;
    static const std::string JSON_HIGH_SCORE_TABLE;
    static const std::string JSON_MAPPINGS;
    static const std::string JSON_NAME;
    static const std::string JSON_ON_FINISH;
    static const std::string JSON_ON_HIGH_SCORE_ACHIEVED;
    static const std::string JSON_ON_HIGH_SCORE_FAILED;
    static const std::string JSON_RECORD_LIMIT;
    static const std::string JSON_SAVE_PATH;
    static const std::string JSON_TYPE;
    static const std::string JSON_USER;
    static const std::string JSON_VALUE;

    static const std::string TYPE_INTEGER;
    static const std::string TYPE_STRING;

    // External interfaces.
    IProject& cParentProject;
    
    std::unique_ptr<Project> cProject;
    Action cScriptQuit;
    Action cScriptOnHighScoreAchieved;
    Action cScriptOnHighScoreFailed;
    std::string cComparisonField;
    unsigned int cMaximumRecords;
    std::vector<std::string> cFields;
    std::vector<std::string> cExposures;
    std::map<std::string, Integer> cWriteIntegers;
    std::map<std::string, String> cWriteStrings;
    String cProjectDataPath;
    Boolean cProjectUser;
    std::unique_ptr<HighScoreTable> cHighScoreTable;
//    std::vector<ScoreTable*> cScoreTables;
      
    LuaBinding<ScoreTracker> cLuaBinding;
    
    void projectCompleted();
    std::unique_ptr<HighScoreTable> readHighScoreTable();
    std::string getFieldValue(const std::string&);
  };
}
