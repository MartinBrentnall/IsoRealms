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
#pragma once

#include "IsoRealms/Types.h"

#include "Function/Function.h"

namespace IsoRealms::Basics {
  
  /**
   * Fixed resource definition for scripts (in-line functions).
   */
  class Script : public IActionType {
    public:
    Script();
    void unregisterAssets(IAssetRemover& remover, IAssets& releaser);

    /**************************\
     * Implements IActionType *
    \**************************/
    IAction* createAction(JSONObject object, IProject& project, IBindingRegistry* localArgs) override;
    IAction* createAction(IProject& project, IBindingRegistry* localArgs) override;
    void destroyAction(IAction* action, IAssets& assets) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:
    class ScriptAction : public IAction {
      public:
      ScriptAction(Script& parent, JSONObject object, IProject& project, unsigned int index, IBindingRegistry* localArgs);
      ScriptAction(Script& parent, IProject& project, unsigned int index);

      void destroyInternalAction(IAssets& assets);
      unsigned int getIndex() const;
      void unregisterAssets(IAssets& releaser);

      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      void save(JSONObject object, IAssetIdentifier& identifier) const override;
      bool hasConfiguration() const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      
      // External interfaces.
      Script& cDefParent; /// Parent action type.
      
      // Definition data.
      Function cDefFunction;  /// Function of this action.
      IAction* cDefAction;    /// Script action call.
      unsigned int cDefIndex; /// Index number of the function call.
    };
      
    // Functions and actions created by scripting.
    std::map<IAction*, std::unique_ptr<ScriptAction>> cDefScriptActions;
    
    // Private functions.
    unsigned int getNextAvailableIndex();
  };
}
