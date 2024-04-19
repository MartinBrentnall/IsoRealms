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
    for (const std::pair<IAction* const, std::unique_ptr<ScriptAction>>& mScript : cDefScriptActions) {
      mScript.second->unregisterAssets(releaser);
    }
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

  IAction* Script::createAction(DOMNode& node, IProject* project, IBindingRegistry* localArgs) {
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(this, node, project, getNextAvailableIndex(), localArgs);
    IAction* mAction = mScriptAction.get();
    cDefScriptActions.emplace(mAction, std::move(mScriptAction));
    return mAction;
  }
  
  IAction* Script::createAction(IProject* project, IBindingRegistry* localArgs) {
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(this, project, getNextAvailableIndex());
    IAction* mAction = mScriptAction.get();
    cDefScriptActions.emplace(mAction, std::move(mScriptAction));
    return mAction;
  }
  
  void Script::destroyAction(IAction* action, IAssets* assets) {
    std::map<IAction*, std::unique_ptr<ScriptAction>>::iterator mScriptAction = cDefScriptActions.find(action);
    if (mScriptAction == cDefScriptActions.end()) {
      throw ArgumentException("ERROR: Script::destroyAction: Script of specified action not found.");
    }
    mScriptAction->second->destroyInternalAction(assets);
    cDefScriptActions.erase(action);
  }

  bool Script::renderAssetIcon() const {
    return false;
  }

  Script::ScriptAction::ScriptAction(Script* parent, DOMNode& node, IProject* project, unsigned int index, IBindingRegistry* localArgs) :
            cDefParent(parent),
            cDefFunction(project, "_t" + Utils::toString(index), node, localArgs, false),
            cDefAction(cDefFunction.createAction(node, project, nullptr)),
            cDefIndex(index) {
  }

  Script::ScriptAction::ScriptAction(Script* parent, IProject* project, unsigned int index) :
            cDefParent(parent),
            cDefFunction(project, "_t" + Utils::toString(index)),
            cDefAction(cDefFunction.createAction(project, nullptr)),
            cDefIndex(index) {
  }

  const IActionType* Script::ScriptAction::getInternalActionType() {
    return cDefAction->getActionType();
  }

  void Script::ScriptAction::destroyInternalAction(IAssets* assets) {
    cDefFunction.destroyAction(cDefAction, assets);
  }

  unsigned int Script::ScriptAction::getIndex() const {
    return cDefIndex;
  }

  void Script::ScriptAction::unregisterAssets(IAssets* releaser) {
    cDefFunction.unregisterAssets(nullptr, releaser);
  }

  void Script::ScriptAction::execute() {
    cDefAction->execute();
  }
  
  IActionType* Script::ScriptAction::getActionType() const {
    return cDefParent;
  }
  
  void Script::ScriptAction::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefFunction.save(node, identifier, true);
  }
  
  bool Script::ScriptAction::hasConfiguration() const {
    return true;
  }
}
