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
#include "ScoreTracker.h"

namespace IsoRealms::HighScore {
  ScoreTracker::ScoreTracker(IProject& project, HighScore& highScore, IResourceData& data) :
            cParentProject(project),
            cScriptQuit(data.getDummyActionClient()),
            cScriptOnHighScoreAchieved(data.getDummyActionClient()),
            cScriptOnHighScoreFailed(data.getDummyActionClient()),
            cProjectDataPath(data),
            cProjectUser(data, false),
            cLuaBinding(project, this) {
    project.mainThreadInit([this]() {
      cProject->initMainThread();      
    });
  }
  
  ScoreTracker::ScoreTracker(IProject& project, HighScore& highScore, IResourceData& data, JSONObject object, IOptions& options) :
            ScoreTracker(project, highScore, data) {
    LocalOptions mLocalOptions("Project", options);
    cProject = std::make_unique<Project>(cParentProject.getApplication(), mLocalOptions, [this](bool forceQuit) {
      if (forceQuit) {
        cParentProject.finish(true);
      } else {
        projectCompleted();
      }
    });

    cProjectUser.init(object, JSON_USER);
    cProjectDataPath.init(object, JSON_SAVE_PATH);
    cMaximumRecords = object.getInteger(JSON_RECORD_LIMIT);
    cScriptOnHighScoreAchieved.init(object, JSON_ON_HIGH_SCORE_ACHIEVED);
    cScriptOnHighScoreFailed.init(object, JSON_ON_HIGH_SCORE_FAILED);
    cScriptQuit.init(object, JSON_ON_FINISH);

    for (JSONObject mFieldMappingsObject : object.getArray(JSON_MAPPINGS)) {
      std::string mFieldName  = mFieldMappingsObject.getString(JSON_NAME);
      std::string mFieldType  = mFieldMappingsObject.getString(JSON_TYPE);
      cFields.push_back(mFieldName);
      if (mFieldType == TYPE_INTEGER) {
        cWriteIntegers.emplace(mFieldName, data);
        cWriteIntegers.find(mFieldName)->second.init(mFieldMappingsObject, JSON_VALUE);
        bool mCompare = mFieldMappingsObject.getBoolean(JSON_COMPARE);
        if (mCompare) {
          cComparisonField = mFieldName;
        }
      } else if (mFieldType == TYPE_STRING) {
        cWriteStrings.emplace(std::piecewise_construct, std::forward_as_tuple(mFieldName), std::forward_as_tuple(data));
        cWriteStrings.find(mFieldName)->second.init(mFieldMappingsObject, JSON_VALUE);
      } else {
        throw ResourceInitException("ERROR: ScoreTracker::ScoreTracker: Unsupported field type \"" + mFieldType + "\".");
      }
    }
  }

  bool ScoreTracker::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> ScoreTracker::getProperties(IPropertyOwner& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ScoreTracker::renderAssetIcon() const {
    return false;
  }

  void ScoreTracker::saveAsset(JSONObject object) const {
    // Nothing to do.
  }
  
  void ScoreTracker::updateRuntime(unsigned int milliseconds) {
    cProject->updateRuntime(milliseconds);
    cProject->updateRuntimeComplete();
  }

  void ScoreTracker::reset() {
    cProject->reset();
  }
  
  std::vector<std::unique_ptr<IProperty>> ScoreTracker::getAssetProperties(IPropertyOwner& owner) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool ScoreTracker::isDefaultConfiguration() const {
    return true;
  }

  void ScoreTracker::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void ScoreTracker::renderScreen(float scale, float aspectRatio) const {
    cProject->render(aspectRatio);
  }

  bool ScoreTracker::input(sf::Event& event) {
    return cProject->input(event);
  }

  void ScoreTracker::resetInput() {
    // Nothing to do.
  }

  void ScoreTracker::save(JSONObject object) const {
    object.addInteger(JSON_RECORD_LIMIT, cMaximumRecords);
    cProjectUser.save(object, JSON_USER);
    cProjectDataPath.save(object, JSON_SAVE_PATH);
    cScriptOnHighScoreAchieved.save(object, JSON_ON_HIGH_SCORE_ACHIEVED);
    cScriptOnHighScoreFailed.save(object, JSON_ON_HIGH_SCORE_FAILED);
    cScriptQuit.save(object, JSON_ON_FINISH);
    JSONArray mMappingsArray = object.addArray(JSON_MAPPINGS);
    for (const std::pair<const std::string, Integer>& mWriteInteger : cWriteIntegers) {
      JSONObject mMappingObject = mMappingsArray.addObject();
      mMappingObject.addString(JSON_TYPE, TYPE_INTEGER);
      mMappingObject.addString(JSON_NAME, mWriteInteger.first);
      mWriteInteger.second.save(mMappingObject, JSON_VALUE);
    }
    for (const std::pair<const std::string, String>& mWriteString : cWriteStrings) {
      JSONObject mMappingObject = mMappingsArray.addObject();
      mMappingObject.addString(JSON_TYPE, TYPE_STRING);
      mMappingObject.addString(JSON_NAME, mWriteString.first);
      mWriteString.second.save(mMappingObject, JSON_VALUE);
    }
  }

  void ScoreTracker::registerAssets(ResourceAssetRegistry& assets) {
    assets.add<IScreen>(this, "", "Score Trackers");
    assets.add<IInputHandler>(this, "", "Score Trackers");
    assets.add<IBinding>(&cLuaBinding, "", "Score Trackers");
    assets.add<IAssets>(cProject.get(), "Project", "Score Trackers");
//    for (ScoreTable* mScoreTable : cScoreTables) {
      
//    }
  }
  
  void ScoreTracker::projectCompleted() {
    
    // See if score qualifies
    cHighScoreTable = readHighScoreTable();
    // TODO: Support other types.
    IInteger* mInteger = *cWriteIntegers.find(cComparisonField)->second;
    std::string mChallengerValue = std::to_string(mInteger->getValue());
    if (cHighScoreTable->qualifies(mChallengerValue)) {
      cScriptOnHighScoreAchieved.execute();
    } else {
      cScriptOnHighScoreFailed.execute();
    }
  }

  std::unique_ptr<HighScoreTable> ScoreTracker::readHighScoreTable() {
    std::string mProjectName = cProjectDataPath->getValue();
    bool mUser = cProjectUser->getValue();
    std::size_t mExtensionPosition = mProjectName.find_last_of('.');
    std::string mHighScoreTablePath = (mUser ? "User/" : "Program/") + mProjectName.substr(0, mExtensionPosition) + "/HighScores.table";
    if (System::fileExists(mHighScoreTablePath, true)) {
      JSONDocument mHighScoreTableDocument(mHighScoreTablePath, true);
      JSONObject mHighScoreTableObject = mHighScoreTableDocument.getObject(JSON_HIGH_SCORE_TABLE);
      return std::make_unique<HighScoreTable>(mHighScoreTableObject);
    }
    return std::make_unique<HighScoreTable>(cFields, cComparisonField, cMaximumRecords);
  }

  std::string ScoreTracker::getFieldValue(const std::string& field) {
    for (std::pair<const std::string, Integer>& mWriteInteger : cWriteIntegers) {
      if (mWriteInteger.first == field) {
        return std::to_string(mWriteInteger.second->getValue());
      }
    }
    for (const std::pair<const std::string, String>& mWriteString : cWriteStrings) {
      if (mWriteString.first == field) {
        return mWriteString.second->getValue();
      }
    }
    throw ArgumentException("ERROR: ScoreTracker::getFieldValue: Field \"" + field + "\" not found.");
  }

  void ScoreTracker::writeValues() {
    std::map<std::string, std::string> cRecordToInsert;
    for (std::string mFieldName : cFields) {
      cRecordToInsert[mFieldName] = getFieldValue(mFieldName);
    }
    cHighScoreTable->insertRecord(cRecordToInsert);

    std::string mProjectName = cProjectDataPath->getValue();
    bool mUser = cProjectUser->getValue();
    std::size_t mExtensionPosition = mProjectName.find_last_of('.');
    std::string mHighScoreTablePath = (mUser ? "User/" : "Program/") + mProjectName.substr(0, mExtensionPosition) + "/HighScores.table";
    JSONDocument mHighScoreTableDocument;
    JSONObject mHighScoreTableObject = mHighScoreTableDocument.addObject(JSON_HIGH_SCORE_TABLE);
    cHighScoreTable->save(mHighScoreTableObject);
    mHighScoreTableDocument.save(mHighScoreTablePath);
  }

  void ScoreTracker::quit() {
    cScriptQuit.execute();
  }

  const std::string ScoreTracker::JSON_COMPARE                = "compare";
  const std::string ScoreTracker::JSON_HIGH_SCORE_TABLE       = "highScoreTable";
  const std::string ScoreTracker::JSON_MAPPINGS               = "mappings";
  const std::string ScoreTracker::JSON_NAME                   = "name";
  const std::string ScoreTracker::JSON_ON_FINISH              = "onFinish";
  const std::string ScoreTracker::JSON_ON_HIGH_SCORE_ACHIEVED = "onHighScoreAchieved";
  const std::string ScoreTracker::JSON_ON_HIGH_SCORE_FAILED   = "onHighScoreFailed";
  const std::string ScoreTracker::JSON_RECORD_LIMIT           = "recordLimit";
  const std::string ScoreTracker::JSON_SAVE_PATH              = "savePath";
  const std::string ScoreTracker::JSON_TYPE                   = "type";
  const std::string ScoreTracker::JSON_USER                   = "user";
  const std::string ScoreTracker::JSON_VALUE                  = "value";

  const std::string ScoreTracker::TYPE_INTEGER = "Integer";
  const std::string ScoreTracker::TYPE_STRING  = "String";
}
