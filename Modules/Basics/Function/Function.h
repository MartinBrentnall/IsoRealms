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

#include <iostream>
#include <string>
#include <vector>

#include <sol.hpp>

#include "IsoRealms/Assets/IBindingRegistry.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/Lua/LuaState.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "ArgumentDefinition.h"
#include "Binding.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a Lua function.  Each action (call) derived from a
   * function may override dynamic bindings with their own values.  This class
   * also facilitates scripting (in-line functions) via the Script type.
   */
  class Function final : public IActionType {
    public:
    
    /**********************\
     * Resource Interface *
    \**********************/
    Function(IProject& project, Basics& basics, IResourceData& data);
    Function(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier, bool script = false) const;
    bool renderIcon() const;
    void hintInUse(bool inUse);
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);

    // Constructors for use by scripts (in-line functions).
    Function(IProject& project, const std::string& name);
    Function(IProject& project, const std::string& name, JSONObject object, IBindingRegistry* localArgs, bool init);
    void unregisterAssets(IAssets& releaser);
    std::vector<std::unique_ptr<IProperty>> getScriptProperties();
    IProject& getProject() const;
    bool setBindingName(Binding& binding, const std::string& name);
    bool setArgumentDefinitionName(ArgumentDefinition& argumentDefinition, const std::string& name);
    Binding* getBinding(const std::string& name);
    ArgumentDefinition* getArgumentDefinition(const std::string& name);
    std::string getNextAvailableName(const std::string& name);

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
    
    // JSON members.
    static const std::string JSON_ARGUMENT;
    static const std::string JSON_ARGUMENTS;
    static const std::string JSON_BINDINGS;
    static const std::string JSON_CODE;
    static const std::string JSON_DEFAULT_VALUE;
    static const std::string JSON_ID;
    static const std::string JSON_TO;
    static const std::string JSON_TYPE;
    static const std::string JSON_VARIABLE;

    // Private types.
    class Call : public IAction {
      public:
      Call(Function& parent, IProject& project, IBindingRegistry* localObjects);
      Call(Function& parent, JSONObject object, IProject& project, IBindingRegistry* localObjects);
      void release(IAssets& releaser);

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
      Function& cDefParent; /// Owner of this function call.
      
      // Definition data.
      IBindingRegistry* cDefLocalBindingRegistry;                     /// Registry from which local bindings can be retrieved.
      std::vector<std::unique_ptr<IsoRealms::Binding>> cDefArguments; /// Overrides default values of dynamic bindings in the function.
    };

    // External interfaces.
    IProject& cProject;
    sol::state* const cDefLuaState; /// The Lua state machine.
    
    // Definition data.
    std::string cDefName;                                      /// Identifier of this function.
    std::string cDefCode;                                      /// Implementation of this function.
    std::vector<std::unique_ptr<Binding>> cDefBindings;   /// Fixed bindings for access within this function.
    std::vector<std::unique_ptr<ArgumentDefinition>> cDefArgumentDefinitions; /// Dynamic bindings for access within this function.
    
    std::map<IAction*, std::unique_ptr<Call>> cInstances;      /// Calls derived from this function.

    // Private functions.
    template <class VECTOR> std::string generateBindingDeclarations(VECTOR& bindings, unsigned int baseIndex) const {
      std::string mFunctionDeclaration;
      for (unsigned int i = 0; i < bindings.size(); i++) {
        mFunctionDeclaration += bindings[i]->getInitCode();
      }
      mFunctionDeclaration += "\n";
      for (unsigned int i = 0; i < bindings.size(); i++) {
        mFunctionDeclaration += bindings[i]->getCode(cDefName, i + baseIndex);
      }
      mFunctionDeclaration += "\n";
      return mFunctionDeclaration;
    }

    unsigned int getDynamicBindingIndex(const std::string& name);
    void declare();
    void revoke();
  };
}
