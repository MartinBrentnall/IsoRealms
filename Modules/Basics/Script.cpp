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
#include "Script.h"

namespace IsoRealms::Basics {
  Script::Script() {
    // Nothing to do
  }

  void Script::unregisterAssets(IAssetRemover* remover, IAssets* releaser) {
    remover->remove(this);
    for (const std::pair<const unsigned int, std::unique_ptr<Function>>& mScript : cDefScriptFunctions) {
      mScript.second->unregisterAssets(nullptr, releaser);
    }
  }

  unsigned int Script::getNextAvailableIndex() {
    unsigned int mAvailableIndex = 0;
    bool mAvailableIndexChanged = true;
    while (mAvailableIndexChanged) {
      mAvailableIndexChanged = false;
      for (const std::pair<const unsigned int, std::unique_ptr<Function>>& mScript : cDefScriptFunctions) {
        if (mScript.first == mAvailableIndex) {
          mAvailableIndex++;
          mAvailableIndexChanged = true;
        }
      }
    }
    return mAvailableIndex;
  }

  IAction* Script::createAction(DOMNode& node, IProject* project, IBindingRegistry* localArgs) {
    unsigned int mNextAvailableIndex = getNextAvailableIndex();
    std::string mTempFunctionName = "_t" + Utils::toString(mNextAvailableIndex);
    cDefScriptFunctions[mNextAvailableIndex] = std::make_unique<Function>(project, mTempFunctionName, node, localArgs);
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(this, cDefScriptFunctions[mNextAvailableIndex]->createAction(node, project, nullptr));
    return cDefScriptActions.emplace(mScriptAction.get(), std::move(mScriptAction)).first->first;
  }
  
  IAction* Script::createAction(IProject* project, IBindingRegistry* localArgs) {
    unsigned int mNextAvailableIndex = getNextAvailableIndex();
    std::string mTempFunctionName = "_t" + Utils::toString(mNextAvailableIndex);
    cDefScriptFunctions[mNextAvailableIndex] = std::make_unique<Function>(project, mTempFunctionName);
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(this, cDefScriptFunctions[mNextAvailableIndex]->createAction(project, nullptr));
    return cDefScriptActions.emplace(mScriptAction.get(), std::move(mScriptAction)).first->first;
  }
  
  void Script::destroyAction(IAction* action, IAssets* assets) {
    std::map<IAction*, std::unique_ptr<ScriptAction>>::iterator mScriptAction = cDefScriptActions.find(action);
    if (mScriptAction == cDefScriptActions.end()) {
      throw ArgumentException("ERROR: Script::destroyAction: Script of specified action not found.");
    }

    std::vector<unsigned int> mScriptsToRemove;
    for (const std::pair<const unsigned int, std::unique_ptr<Function>>& mScript : cDefScriptFunctions) {
      if (mScriptAction->second->getInternalActionType() == mScript.second.get()) {
        mScriptAction->second->destroyInternalAction(mScript.second.get(), assets);
        mScriptsToRemove.push_back(mScript.first);
      }
    }

    cDefScriptActions.erase(mScriptAction);
    for (unsigned int mIndexToRemove : mScriptsToRemove) {
      cDefScriptFunctions.erase(mIndexToRemove);
    }
  }

  bool Script::renderAssetIcon() const {
    return false;
  }

  Script::ScriptAction::ScriptAction(Script* parent, IAction* action) :
            cDefParent(parent),
            cDefAction(action) {
  }

  const IActionType* Script::ScriptAction::getInternalActionType() {
    return cDefAction->getActionType();
  }

  void Script::ScriptAction::destroyInternalAction(Function* mType, IAssets* assets) {
    mType->destroyAction(cDefAction, assets);
  }

  void Script::ScriptAction::execute() {
    cDefAction->execute();
  }
  
  IActionType* Script::ScriptAction::getActionType() const {
    return cDefParent;
  }
  
  void Script::ScriptAction::save(DOMNodeWriter* node, IAssetIdentifier* identifier, const std::string& tag) const {
    // TODO: Implement this
  }
  
  bool Script::ScriptAction::hasConfiguration() const {
    return true;
  }
}
