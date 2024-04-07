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
  const std::string ScoreTable::TAG_FIELD            = "Field";
  const std::string ScoreTable::TAG_HIGH_SCORE_TABLE = "HighScoreTable";
  const std::string ScoreTable::TAG_PROJECT          = "Project";
  const std::string ScoreTable::TAG_RECORD           = "Record";
  const std::string ScoreTable::TAG_USER             = "User";

  const std::string ScoreTable::ATTRIBUTE_RECORDS    = "records";
  const std::string ScoreTable::ATTRIBUTE_NAME       = "name";
  const std::string ScoreTable::ATTRIBUTE_FIELD      = "field";
  const std::string ScoreTable::ATTRIBUTE_VALUE      = "value";

  ScoreTable::ScoreTable(IProject* project, HighScore* highScore) :
            cProjectDataPath(project),
            cProjectUser(project, false),
            cLuaBinding(project, this) {
  }

  ScoreTable::ScoreTable(IProject* project, HighScore* highScore, DOMNode& node, IOptions* options, IResourceData* data) :
            ScoreTable(project, highScore) {
    int mRecordCount = node.getIntegerAttribute(ATTRIBUTE_RECORDS);
    std::vector<std::string> mFieldNames;
    for (DOMNode& mFieldNode : node) {
      std::string mValue = mFieldNode.getName();
      if (mValue == TAG_FIELD) {
        std::string mFieldName = mFieldNode.getAttribute(ATTRIBUTE_NAME);
        std::vector<LiteralString> mValues;
        for (int i = 0; i < mRecordCount; i++) {
          mValues.emplace_back(LiteralString(""));
        }
        cValues.emplace(mFieldName, std::move(mValues));
      } else if (mValue == TAG_USER || mValue == TAG_PROJECT || mValue == TAG_RECORD) {
        // Handled separately, nothing to do.
      } else {
        throw ParseException("Unknown tag for HighScore/ScoreTable: " + mValue);
      }
    }
    readRecords(node);
    
    cProjectUser.init(node, TAG_USER);
    cProjectDataPath.init(node, TAG_PROJECT);
    project->init([this](IAssets* assets) {
      writeDefaultTable();
    });
  }

  bool ScoreTable::renderIcon() {
    return false;
  }

  std::vector<IProperty*> ScoreTable::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void ScoreTable::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  void ScoreTable::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    // TODO
  }

  void ScoreTable::registerAssets(IAssetRegistry* assets) {
    // TODO: This is dangerous!  Exposing IStrings from inside a map/vector... Addresses could change without warning.
    for (std::map<const std::string, std::vector<LiteralString>>::iterator mValue = cValues.begin(); mValue != cValues.end(); mValue++) {
      for (unsigned int i = 0; i < mValue->second.size(); i++) {
        assets->add(&mValue->second[i], mValue->first + "_" + std::to_string(i), "System");
      }
    }
    assets->add(&cLuaBinding, "", "Score Tables");
  }
  
  void ScoreTable::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    for (std::pair<std::string, std::vector<LiteralString>> mValue : cValues) {
      for (LiteralString& mAsset : mValue.second) {
        assets->remove(&mAsset);
      }
    }
    assets->remove(&cLuaBinding);
  }
  
  void ScoreTable::readRecords(DOMNode& node) {
    int mRecordIndex = 0;
    for (DOMNode& mNode : node) {
      std::string mValue = mNode.getName();
      if (mValue == TAG_FIELD || mValue == TAG_PROJECT || mValue == TAG_USER) { // TODO: Shouldn't be here
        // Nothing to do
      } else if (mValue == TAG_RECORD) {
        for (DOMNode& mFieldNode : mNode) {
          std::string mFieldName = mFieldNode.getAttribute(ATTRIBUTE_FIELD);
          std::string mFieldValue = mFieldNode.getAttribute(ATTRIBUTE_VALUE);
          cValues[mFieldName][mRecordIndex].setValue(mFieldValue);
        }
        mRecordIndex++;
      } else {
        throw ParseException("Unknown tag for HighScore/ScoreTable: " + mValue);
      }
    }
  }

  void ScoreTable::reload() {
    std::string mProjectName = cProjectDataPath->getValue();
    bool mUser = cProjectUser->getValue();
    std::size_t mExtensionPosition = mProjectName.find_last_of('.');
    std::string mHighScoreTablePath = (mUser ? "User/" : "Program/") + mProjectName.substr(0, mExtensionPosition) + "/HighScores.table";
    if (System::fileExists(mHighScoreTablePath, true)) {
      DOMNode mHighScoreNode(mHighScoreTablePath, DOMNode::Type::USER);
      DOMNode& mTableNode = mHighScoreNode.getNode(TAG_HIGH_SCORE_TABLE);
      readRecords(mTableNode);
    }
  }

  void ScoreTable::writeDefaultTable() {
    std::string mProjectName = cProjectDataPath->getValue();
    bool mUser = cProjectUser->getValue();
    std::size_t mExtensionPosition = mProjectName.find_last_of('.');
    std::string mHighScoreTablePath = (mUser ? "User/" : "Program/") + mProjectName.substr(0, mExtensionPosition) + "/HighScores.table";
    if (!System::fileExists(mHighScoreTablePath, true)) {
      DOMNodeWriter mHighScoreNode(TAG_HIGH_SCORE_TABLE);
      mHighScoreNode.addAttribute("maximumRecords", 10);
      for (std::pair<std::string, std::vector<LiteralString>> mPair : cValues) {
        DOMNodeWriter mFieldNode = mHighScoreNode.addBranch(TAG_FIELD);
        mFieldNode.addAttribute(ATTRIBUTE_NAME, mPair.first);
        // TODO: This is a hack... this whole thing is a hack really.
        if (mPair.first == "Score") {
          mFieldNode.addAttribute("compare", true);
        }
      }
      for (int i = 0; i < 10; i++) {
        DOMNodeWriter mRecordNode = mHighScoreNode.addBranch(TAG_RECORD);
        for (std::pair<std::string, std::vector<LiteralString>> mPair : cValues) {
          DOMNodeWriter mFieldNode = mRecordNode.addBranch(TAG_FIELD);
          mFieldNode.addAttribute(ATTRIBUTE_FIELD, mPair.first);
          mFieldNode.addAttribute(ATTRIBUTE_VALUE, mPair.second[i].getValue());
        }
      }
      mHighScoreNode.save(mHighScoreTablePath);
    }
  }
}
