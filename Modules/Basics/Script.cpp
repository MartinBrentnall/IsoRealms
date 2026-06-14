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

  IAction* Script::getResource(IActionContext& owner, JSONObject object) {
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(*this, owner, getNextAvailableIndex());
    IAction* mAction = mScriptAction.get();
    cDefScriptActions.emplace(mAction, std::move(mScriptAction));
    return mAction;
  }

  IAction* Script::getResource(IActionContext& owner) {
    std::unique_ptr<ScriptAction> mScriptAction = std::make_unique<ScriptAction>(*this, owner, getNextAvailableIndex());
    IAction* mAction = mScriptAction.get();
    cDefScriptActions.emplace(mAction, std::move(mScriptAction));
    return mAction;
  }

  void Script::releaseResource(const IAction* resource) {
    std::map<const IAction*, std::unique_ptr<ScriptAction>>::iterator mScriptAction = cDefScriptActions.find(resource);
    if (mScriptAction == cDefScriptActions.end()) {
      throw ArgumentException("ERROR: Script::destroyAction: Script of specified action not found.");
    }
    mScriptAction->second->destroyInternalAction();
    cDefScriptActions.erase(resource);
  }

  bool Script::renderResourceProviderIcon() const {
    Utils::renderIconTerminal();
    return true;
  }

  bool Script::isHiddenProvider() const {
    return false;
  }

  bool Script::hasConfiguration() const {
    return true;
  }

  const Metadata& Script::getMetadata() const {
    return cBasics.getMetadata("Script");
  }

  Script::ScriptAction::ScriptAction(Script& parent, IActionContext& owner, unsigned int index, JSONObject object) :
            cParent(parent),
            cDefFunction(cParent.cBasics, owner, object, false),
            cDefAction(cDefFunction.getResource(owner, object)),
            cDefIndex(index) {
  }

  Script::ScriptAction::ScriptAction(Script& parent, IActionContext& owner, unsigned int index) :
            cParent(parent),
            cDefFunction(cParent.cBasics, owner),
            cDefAction(cDefFunction.getResource(owner)),
            cDefIndex(index) {
  }

  void Script::ScriptAction::destroyInternalAction() {
    cDefFunction.releaseResource(cDefAction);
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
  
  bool Script::ScriptAction::renderResourceIcon() const {
    Utils::renderIconTerminal();
    return true;
  }
  
  void Script::ScriptAction::getResourceProperties(IComponentDefiner& definer) {
    cDefFunction.getScriptProperties(definer);
  }
}
