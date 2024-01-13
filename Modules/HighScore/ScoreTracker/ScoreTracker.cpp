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
#include "ScoreTracker.h"

namespace IsoRealms::HighScore {
  ScoreTracker::ScoreTracker(IProject* project, HighScore* highScore) :
            cParentProject(project),
            cScriptQuit(project),
            cScriptOnHighScoreAchieved(project),
            cScriptOnHighScoreFailed(project),
            cProjectDataPath(project),
            cProjectUser(project, false),
            cLuaBinding(project, this) {
    project->mainThreadInit([this]() {
      cProject->initMainThread();      
    });
    
    project->reset([this]() {
      cProject->reset();
    });
    
    project->updateRuntime([this](unsigned int milliseconds) {
      cProject->updateRuntime(milliseconds);
      cProject->updateRuntimeComplete();
    });
  }
  
  ScoreTracker::ScoreTracker(IProject* project, HighScore* highScore, DOMNode& node, IOptions* options, IResourceData* data) :
            ScoreTracker(project, highScore) {
//     for (DOMNode& mNode : node) {
//       std::string mValue = mNode.getName();
//       if (mValue == "ExposeTable") {
//         cScoreTables.push_back(new ScoreTable(mNode, registry));
//       } else {
//         // TODO: Throw
//       }
//     }
    LocalOptions mLocalOptions("Project", options);
    cProject = std::make_unique<Project>(cParentProject->getApplication(), &mLocalOptions, [this](bool forceQuit) {
      if (forceQuit) {
        cParentProject->finish(true);
      } else {
        projectCompleted();
      }
    });

    cProjectUser.init(node.getNode(TAG_USER));
    cProjectDataPath.init(node.getNode(TAG_SAVE_PATH));
    cMaximumRecords = node.getIntegerAttribute("maximumRecords");
    cScriptOnHighScoreAchieved.init(node, "OnHighScoreAchieved");
    cScriptOnHighScoreFailed.init(node, "Otherwise");
    cScriptQuit.init(node, "Quit");

    for (DOMNode& mNode : node) {
      std::string mValue = mNode.getName();
      if (mValue == "Quit" || mValue == "OnHighScoreAchieved" || mValue == TAG_SAVE_PATH || mValue == TAG_USER || mValue == "Otherwise") {
        // TODO: Shouldn't be necessary.
      } else if (mValue == "WriteField") {
        std::string mFieldName  = mNode.getAttribute("name");
        std::string mFieldType  = mNode.getAttribute("type");
        cFields.push_back(mFieldName);
        bool mCompare = mNode.getBooleanAttribute("compare");
        if (mFieldType == "Integer") {
          cWriteIntegers.emplace(mFieldName, project);
          cWriteIntegers.find(mFieldName)->second.init(mNode.getNode(TAG_VALUE));
          if (mCompare) {
            cComparisonField = mFieldName;
          }
        } else if (mFieldType == "String") {
          cWriteStrings.emplace(std::piecewise_construct, std::forward_as_tuple(mFieldName), std::forward_as_tuple(project));
          cWriteStrings.find(mFieldName)->second.init(mNode.getNode(TAG_VALUE));
        } else {
          throw ResourceInitException("ERROR: ScoreTracker::ScoreTracker: Unsupported field type \"" + mFieldType + "\".");
        }
      } else if (mValue == "ExposeTable") {
        // TODO: Shouldn't be here
      } else {
        throw ParseException("Unknown tag for HighScore/ScoreTracker: " + mValue);
      }
    }
      
// TODO       if (cScriptQuit == nullptr) {
//         std::cout << "Warning: No 'Quit' script set.  Project won't terminate autamotically" << std::endl;
//       }
//       if (cScriptOnHighScoreAchieved == nullptr) {
//         std::cout << "Warning: No 'OnHighScoreAchieved' script set.  No action will be taken when the project completes" << std::endl;
//       }
      
      // Expose any data
  //     for (ScoreTable* mScoreTable : cScoreTables) {
  //       mScoreTable->reload();
  //     }
  }

  bool ScoreTracker::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> ScoreTracker::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  bool ScoreTracker::renderAssetIcon() const {
    return false;
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

  void ScoreTracker::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    // Not supported
  }

  void ScoreTracker::registerAssets(IAssetRegistry* assets) {
    assets->add(static_cast<IScreen*>(this), "", "Score Trackers");
    assets->add(static_cast<IInputHandler*>(this), "", "Score Trackers");
    assets->add(&cLuaBinding, "", "Score Trackers");
    assets->add(static_cast<IAssets*>(cProject.get()), "Project", "Score Trackers");
//    for (ScoreTable* mScoreTable : cScoreTables) {
      
//    }
  }
  
  void ScoreTracker::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(static_cast<IInputHandler*>(this));
    assets->remove(static_cast<IScreen*>(this));
    assets->remove(&cLuaBinding);
    assets->remove(static_cast<IAssets*>(cProject.get()));
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
      DOMNode mHighScoreNode(mHighScoreTablePath, DOMNode::Type::USER);
      for (DOMNode& mNode : mHighScoreNode) {
        std::string mValue = mNode.getName();
        if (mValue == "HighScoreTable") {
          return std::make_unique<HighScoreTable>(mNode);
        } else {
          throw ParseException("Unknown tag for HighScore/ScoreTracker: " + mValue);
        }
      }
      throw ParseException("ERROR: ScoreTracker::readHighScoreTable: High score table not found in existing file.");
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
    DOMNodeWriter mNode("HighScoreTable");
    cHighScoreTable->save(&mNode);
    mNode.save(mHighScoreTablePath);
  }

  void ScoreTracker::quit() {
    cScriptQuit.execute();
  }

  const std::string ScoreTracker::TAG_SAVE_PATH = "SavePath";
  const std::string ScoreTracker::TAG_USER      = "User";
  const std::string ScoreTracker::TAG_VALUE     = "Value";
}
