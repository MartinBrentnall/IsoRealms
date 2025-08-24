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
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <sol.hpp>

#include "IsoRealms.h"

#include "ArgumentDefinition.h"
#include "Binding.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a Lua function.  Each action (call) derived from a
   * function may override dynamic bindings with their own values.  This class
   * also facilitates scripting (in-line functions) via the Script type.
   */
  class Function final : public IAssetProvider<IActionClient, IAction> {
    public:
    
    /**********************\
     * Resource Interface *
    \**********************/
    Function(Basics& basics, IResourceData& data);
    Function(Basics& basics, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object, bool script = false) const;
    bool renderIcon() const;
    void hintInUse(bool inUse);
    void getProperties(PropertyMaker& owner, const Metadata& metadata);

    // Constructors for use by scripts (in-line functions).
    Function(Basics& basics, const std::string& name, IActionClient& owner);
    Function(Basics& basics, const std::string& name, IActionClient& owner, JSONObject object, bool init);
    void getScriptProperties(PropertyMaker& owner, const Metadata& metadata);
    IsoRealms::Project& getProject() const;
    IResourceData& getResourceData() const;
    void setBindingName(Binding& binding, const std::string& name);
    bool isBindingNameAllowed(Binding& binding, const std::string& name);
    void setArgumentDefinitionName(ArgumentDefinition& argumentDefinition, const std::string& name);
    bool isArgumentDefinitionNameAllowed(ArgumentDefinition& argumentDefinition, const std::string& name);
    Binding* getBinding(const std::string& name);
    ArgumentDefinition* getArgumentDefinition(const std::string& name);
    std::string getNextAvailableName(const std::string& name);

    /****************************************************\
     * Implements IAssetProvider<IActionClient, IAction *
    \****************************************************/
    IAction* getAsset(IActionClient& owner, JSONObject object) override;
    IAction* getAsset(IActionClient& owner) override;
    void releaseAsset(const IAction* asset) override;
    bool hasConfiguration() const override;
    bool renderAssetProviderIcon() const override;

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
      Call(Function& parent, IActionClient& owner);
      Call(Function& parent, IActionClient& owner, JSONObject object);

      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
      void getAssetProperties(PropertyMaker& owner) override;
      bool isDefaultConfiguration() const override;

      private:
      
      // External interfaces.
      Function& cParent; /// Function from which this call is derived.
      IActionClient& cOwner;                                       /// The one who is making this call.

      // Definition data.
      std::vector<std::unique_ptr<IsoRealms::Binding>> cDefArguments; /// Overrides default values of dynamic bindings in the function.
    };

    // External interfaces.
    Project& cProject;
    Basics& cBasics;
    IResourceData& cResourceData;
    sol::state* const cDefLuaState; /// The Lua state machine.
    
    // Definition data.
    std::string cDefName;                                      /// Identifier of this function.
    std::string cDefCode;                                      /// Implementation of this function.
    std::vector<std::unique_ptr<Binding>> cDefBindings;   /// Fixed bindings for access within this function.
    std::vector<std::unique_ptr<ArgumentDefinition>> cDefArgumentDefinitions; /// Dynamic bindings for access within this function.
    
    mutable std::map<const IAction*, std::unique_ptr<Call>> cInstances;      /// Calls derived from this function.

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
