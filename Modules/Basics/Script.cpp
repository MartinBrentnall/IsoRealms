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
#include "Script.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  Script::Script(Basics& basics) :
            cBasics(basics) {
  }

  unsigned int Script::getNextAvailableFunctionID(unsigned int functionID) const {
    bool mFunctionIDChanged = true;
    while (mFunctionIDChanged) {
      mFunctionIDChanged = false;
      for (const std::pair<const IAction* const, std::unique_ptr<ScriptAction>>& mScriptAction : cDefScriptActions) {
        if (mScriptAction.second->getFunctionID() == functionID) {
          functionID++;
          mFunctionIDChanged = true;
        }
      }
    }
    return functionID;
  }

  unsigned int Script::getNextAvailableIndex() const {
    unsigned int mAvailableIndex = 0;
    bool mAvailableIndexChanged = true;
    while (mAvailableIndexChanged) {
      mAvailableIndexChanged = false;
      for (const std::pair<const IAction* const, std::unique_ptr<ScriptAction>>& mScript : cDefScriptActions) {
        if (mScript.second->getIndex() == mAvailableIndex) {
          mAvailableIndex++;
          mAvailableIndexChanged = true;
        }
      }
    }
    return mAvailableIndex;
  }

  IAction* Script::getAsset(IActionClient& owner, JSONObject object) {
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(*this, owner, getNextAvailableIndex(), object);
    IAction* mAction = mScriptAction.get();
    cDefScriptActions.emplace(mAction, std::move(mScriptAction));
    return mAction;
  }

  IAction* Script::getAsset(IActionClient& owner) {
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(*this, owner, getNextAvailableIndex());
    IAction* mAction = mScriptAction.get();
    cDefScriptActions.emplace(mAction, std::move(mScriptAction));
    return mAction;
  }
  
  void Script::releaseAsset(const IAction* asset) {
    std::map<const IAction*, std::unique_ptr<ScriptAction>>::iterator mScriptAction = cDefScriptActions.find(asset);
    if (mScriptAction == cDefScriptActions.end()) {
      throw ArgumentException("ERROR: Script::destroyAction: Script of specified action not found.");
    }
    mScriptAction->second->destroyInternalAction();
    cDefScriptActions.erase(asset);
  }

  bool Script::renderAssetProviderIcon() const {
    Utils::renderIconTerminal();
    return true;
  }

  bool Script::isHiddenProvider() const {
    return false;
  }

  bool Script::hasConfiguration() const {
    return true;
  }

  Script::ScriptAction::ScriptAction(Script& parent, IActionClient& owner, unsigned int index, JSONObject object) :
            cParent(parent),
            cDefFunction(cParent.cBasics, "_t" + Utils::toString(index), owner, object, false),
            cDefAction(cDefFunction.getAsset(owner, object)),
            cDefIndex(index) {
  }

  Script::ScriptAction::ScriptAction(Script& parent, IActionClient& owner, unsigned int index) :
            cParent(parent),
            cDefFunction(cParent.cBasics, "_t" + Utils::toString(index), owner),
            cDefAction(cDefFunction.getAsset(owner)),
            cDefIndex(index) {
  }

  void Script::ScriptAction::destroyInternalAction() {
    cDefFunction.releaseAsset(cDefAction);
  }

  unsigned int Script::ScriptAction::getFunctionID() const {
    return cDefFunction.getID();
  }

  unsigned int Script::ScriptAction::getIndex() const {
    return cDefIndex;
  }

  void Script::ScriptAction::execute() {
    cDefAction->execute();
  }
  
  bool Script::ScriptAction::renderAssetIcon() const {
    Utils::renderIconTerminal();
    return true;
  }

  void Script::ScriptAction::saveAsset(JSONObject object) const {
    cDefFunction.save(object, true);
  }
  
  void Script::ScriptAction::getAssetProperties(PropertyMaker& owner) {
    const Metadata& mMetadata = cParent.cBasics.getMetadata("Script");
    cDefFunction.getScriptProperties(owner, mMetadata);
  }  

  bool Script::ScriptAction::isDefaultConfiguration() const {
    return true;
  }
}
