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

  unsigned int Script::getNextAvailableIndex() {
    unsigned int mAvailableIndex = 0;
    bool mAvailableIndexChanged = true;
    while (mAvailableIndexChanged) {
      mAvailableIndexChanged = false;
      for (const std::pair<IAction* const, std::unique_ptr<ScriptAction>>& mScript : cDefScriptActions) {
        if (mScript.second->getIndex() == mAvailableIndex) {
          mAvailableIndex++;
          mAvailableIndexChanged = true;
        }
      }
    }
    return mAvailableIndex;
  }

  IAction* Script::createAction(JSONObject object, IResourceData& owner, IBindingRegistry* localArgs) {
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(*this, object, owner, getNextAvailableIndex(), localArgs);
    IAction* mAction = mScriptAction.get();
    cDefScriptActions.emplace(mAction, std::move(mScriptAction));
    return mAction;
  }

  IAction* Script::createAction(IResourceData& owner, IBindingRegistry* localArgs) {
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(*this, owner, getNextAvailableIndex());
    IAction* mAction = mScriptAction.get();
    cDefScriptActions.emplace(mAction, std::move(mScriptAction));
    return mAction;
  }
  
  void Script::destroyAction(IAction* action, IAssets& assets) {
    std::map<IAction*, std::unique_ptr<ScriptAction>>::iterator mScriptAction = cDefScriptActions.find(action);
    if (mScriptAction == cDefScriptActions.end()) {
      throw ArgumentException("ERROR: Script::destroyAction: Script of specified action not found.");
    }
    mScriptAction->second->destroyInternalAction(assets);
    cDefScriptActions.erase(action);
  }

  bool Script::renderAssetIcon() const {
    Utils::renderIconTerminal();
    return true;
  }

  void Script::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Script::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Script::isDefaultConfiguration() const {
    return true;
  }

  Script::ScriptAction::ScriptAction(Script& parent, JSONObject object, IResourceData& owner, unsigned int index, IBindingRegistry* localArgs) :
            cDefParent(parent),
            cDefFunction(owner.getProject(), "_t" + Utils::toString(index), owner, object, localArgs, false),
            cDefAction(cDefFunction.createAction(object, owner, nullptr)),
            cDefIndex(index) {
  }

  Script::ScriptAction::ScriptAction(Script& parent, IResourceData& owner, unsigned int index) :
            cDefParent(parent),
            cDefFunction(owner.getProject(), "_t" + Utils::toString(index), owner),
            cDefAction(cDefFunction.createAction(owner, nullptr)),
            cDefIndex(index) {
  }

  void Script::ScriptAction::destroyInternalAction(IAssets& assets) {
    cDefFunction.destroyAction(cDefAction, assets);
  }

  unsigned int Script::ScriptAction::getIndex() const {
    return cDefIndex;
  }

  void Script::ScriptAction::execute() {
    cDefAction->execute();
  }
  
  void Script::ScriptAction::save(JSONObject object) const {
    cDefFunction.save(object, true);
  }
  
  bool Script::ScriptAction::hasConfiguration() const {
    return true;
  }

  std::vector<std::unique_ptr<IProperty>> Script::ScriptAction::getAssetProperties() {
    return cDefFunction.getScriptProperties();
  }  

  bool Script::ScriptAction::isDefaultConfiguration() const {
    return true;
  }
}
