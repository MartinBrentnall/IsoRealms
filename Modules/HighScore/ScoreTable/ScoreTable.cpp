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
#include "ScoreTable.h"

namespace IsoRealms::HighScore {
  const std::string ScoreTable::JSON_COMPARE          = "compare";
  const std::string ScoreTable::JSON_FIELD            = "field";
  const std::string ScoreTable::JSON_FIELDS           = "fields";
  const std::string ScoreTable::JSON_HIGH_SCORE_TABLE = "highScoreTable";
  const std::string ScoreTable::JSON_NAME             = "name";
  const std::string ScoreTable::JSON_PROJECT          = "project";
  const std::string ScoreTable::JSON_RECORD_LIMIT     = "recordLimit";
  const std::string ScoreTable::JSON_RECORDS          = "records";
  const std::string ScoreTable::JSON_USER             = "user";
  const std::string ScoreTable::JSON_VALUE            = "value";
  const std::string ScoreTable::JSON_VALUES           = "values";

  ScoreTable::ScoreTable(IProject& project, HighScore& highScore, IResourceData& data) :
            cProjectDataPath(project),
            cProjectUser(project, false),
            cLuaBinding(project, this) {
  }

  ScoreTable::ScoreTable(IProject& project, HighScore& highScore, IResourceData& data, JSONObject object, IOptions& options) :
            ScoreTable(project, highScore, data) {
    cProjectUser.init(object, JSON_USER);
    cProjectDataPath.init(object, JSON_PROJECT);
    int mRecordCount = object.getInteger(JSON_RECORD_LIMIT);
    std::vector<std::string> mFieldNames;
    for (JSONObject mRecordObject : object.getArray(JSON_FIELDS)) {
      std::string mFieldName = mRecordObject.getString(JSON_NAME);
      std::vector<LiteralString> mValues;
      for (int i = 0; i < mRecordCount; i++) {
        mValues.emplace_back(LiteralString(""));
      }
      cValues.emplace(mFieldName, std::move(mValues));
    }
    readRecords(object);

    project.init([this](IAssets& assets) {
      writeDefaultTable();
    });
  }

  bool ScoreTable::renderIcon() {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> ScoreTable::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  void ScoreTable::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void ScoreTable::save(JSONObject object, IAssetIdentifier& identifier) const {
    cProjectDataPath.save(object, JSON_PROJECT);
    cProjectUser.save(object, JSON_USER);
    object.addInteger(JSON_RECORD_LIMIT, 10);
    JSONArray mFieldsArray = object.addArray(JSON_FIELDS);
    for (std::pair<std::string, std::vector<LiteralString>> mValue : cValues) {
      JSONObject mFieldObject = mFieldsArray.addObject();
      mFieldObject.addString(JSON_NAME, mValue.first);
    }

    // TODO: This is real dogshit, need to rethink the data structure here.
    JSONArray mRecordsArray = object.addArray(JSON_RECORDS);
    for (unsigned int i = 0; i < 10; i++) {
      JSONObject mRecordObject = mRecordsArray.addObject();
      JSONArray mFieldsArray = mRecordObject.addArray(JSON_VALUES);
      for (std::pair<std::string, std::vector<LiteralString>> mValue : cValues) {
        if (!mValue.second[i].getValue().empty()) {
          JSONObject mFieldObject = mFieldsArray.addObject();
          mFieldObject.addString(JSON_FIELD, mValue.first);
          mFieldObject.addString(JSON_VALUE, mValue.second[i].getValue());
        }
      }
    }
  }

  void ScoreTable::registerAssets(IAssetRegistry& assets) {
    // TODO: This is dangerous!  Exposing IStrings from inside a map/vector... Addresses could change without warning.
    for (std::map<const std::string, std::vector<LiteralString>>::iterator mValue = cValues.begin(); mValue != cValues.end(); mValue++) {
      for (unsigned int i = 0; i < mValue->second.size(); i++) {
        assets.add(&mValue->second[i], mValue->first + "_" + std::to_string(i), "System");
      }
    }
    assets.add(&cLuaBinding, "", "Score Tables");
  }
  
  void ScoreTable::unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish) {
    for (std::pair<std::string, std::vector<LiteralString>> mValue : cValues) {
      for (LiteralString& mAsset : mValue.second) {
        assets.remove(&mAsset, relinquish);
      }
    }
    assets.remove(&cLuaBinding, relinquish);
  }
  
  void ScoreTable::readRecords(JSONObject object) {
    // TODO: Keep definition and values separate.
    int mRecordIndex = 0;
    for (JSONObject mRecordObject : object.getArray(JSON_RECORDS)) {
      for (JSONObject mFieldValueObject : mRecordObject.getArray(JSON_VALUES)) {
        std::string mFieldName = mFieldValueObject.getString(JSON_FIELD);
        std::string mFieldValue = mFieldValueObject.getString(JSON_VALUE);
        cValues[mFieldName][mRecordIndex].setValue(mFieldValue);
      }
      mRecordIndex++;
    }
  }

  void ScoreTable::reload() {
    std::string mProjectName = cProjectDataPath->getValue();
    bool mUser = cProjectUser->getValue();
    std::size_t mExtensionPosition = mProjectName.find_last_of('.');
    std::string mHighScoreTablePath = (mUser ? "User/" : "Program/") + mProjectName.substr(0, mExtensionPosition) + "/HighScores.table";
    if (System::fileExists(mHighScoreTablePath, true)) {
      try {
        JSONDocument mHighScoreTableDocument(mHighScoreTablePath, true);
        JSONObject mHighScoreTableObject = mHighScoreTableDocument.getObject(JSON_HIGH_SCORE_TABLE);
        readRecords(mHighScoreTableObject);
      } catch (ParseException& e) {
        std::cout << "WARNING: ScoreTable::reload: Failed to parse score table: " << std::endl << e.getMessage() << std::endl;
      }
    }
  }

  void ScoreTable::writeDefaultTable() {
    std::string mProjectName = cProjectDataPath->getValue();
    bool mUser = cProjectUser->getValue();
    std::size_t mExtensionPosition = mProjectName.find_last_of('.');
    std::string mHighScoreTablePath = (mUser ? "User/" : "Program/") + mProjectName.substr(0, mExtensionPosition) + "/HighScores.table";
    if (!System::fileExists(mHighScoreTablePath, true)) {
      JSONDocument mHighScoreTableDocument;
      JSONObject mHighScoreTableObject = mHighScoreTableDocument.addObject(JSON_HIGH_SCORE_TABLE);
      mHighScoreTableObject.addInteger(JSON_RECORD_LIMIT, 10);
      JSONArray mFieldArray = mHighScoreTableObject.addArray(JSON_FIELDS);

      for (std::pair<std::string, std::vector<LiteralString>> mPair : cValues) {
        JSONObject mFieldObject = mFieldArray.addObject();
        mFieldObject.addString(JSON_NAME, mPair.first);

        // TODO: This is a hack... this whole thing is a hack really.
        if (mPair.first == "Score") {
          mFieldObject.addBoolean(JSON_COMPARE, true);
        }
      }
      JSONArray mRecordArray = mHighScoreTableObject.addArray(JSON_RECORDS);
      for (int i = 0; i < 10; i++) {
        JSONObject mRecordObject = mRecordArray.addObject();
        JSONArray mValuesArray = mRecordObject.addArray(JSON_VALUES);
        for (std::pair<std::string, std::vector<LiteralString>> mPair : cValues) {
          if (!mPair.second[i].getValue().empty()) {
            JSONObject mValueObject = mValuesArray.addObject();
            mValueObject.addString(JSON_FIELD, mPair.first);
            mValueObject.addString(JSON_VALUE, mPair.second[i].getValue());
          }
        }
      }
      mHighScoreTableDocument.save(mHighScoreTablePath);
    }
  }
}
