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
    Function(IProject* project, Basics* basics);
    Function(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);    
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    bool renderIcon() const;
    void hintInUse(bool inUse);
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    // Constructors for use by scripts (in-line functions).
    Function(IProject* project, const std::string& name);
    Function(IProject* project, const std::string& name, DOMNode& node, IBindingRegistry* localArgs);
    
    /**************************\
     * Implements IActionType *
    \**************************/
    IAction* createAction(DOMNode& node, IProject* project, IBindingRegistry* localObjects) override;
    IAction* createAction(IProject* project, IBindingRegistry* localArgs) override;
    void destroyAction(IAction* action, IAssets* assets) override;
    bool renderAssetIcon() const override;
    
    private:
    
    // DOM strings.
    static const std::string TAG_ARGUMENT;
    static const std::string TAG_BIND;
    static const std::string TAG_CODE;
    static const std::string TAG_DEFAULT_VALUE;
    static const std::string TAG_TO;

    static const std::string ATTRIBUTE_ARGUMENT;
    static const std::string ATTRIBUTE_NAME;
    static const std::string ATTRIBUTE_TYPE;
    static const std::string ATTRIBUTE_VARIABLE;
  
    // Private types.
    class Call : public IAction {
      public:
      Call(Function* parent, DOMNode& node, IProject* project, IBindingRegistry* localObjects);
      void release(IAssets* releaser);

      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      IActionType* getActionType() const override;
      void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const override;
      bool hasConfiguration() const override;

      private:
      
      // External interfaces.
      Function* cDefParent; /// Owner of this function call.
      
      // Definition data.
      IBindingRegistry* cDefLocalBindingRegistry;                    /// Registry from which local bindings can be retrieved.
      std::vector<std::unique_ptr<IsoRealms::Binding>> cDefBindings; /// Overrides default values of dynamic bindings in the function.
      std::vector<bool> cDefLocal;                                   /// Specifies whether an overridden value is local or global.
    };

    // External interfaces.
    sol::state* const cDefLuaState; /// The Lua state machine.
    
    // Definition data.
    std::string cDefName;                                      /// Identifier of this function.
    std::string cDefCode;                                      /// Implementation of this function.
    std::vector<std::unique_ptr<Binding>> cDefFixedBindings;   /// Fixed bindings for access within this function.
    std::vector<std::unique_ptr<Binding>> cDefDynamicBindings; /// Dynamic bindings for access within this function.
    
    std::map<IAction*, std::unique_ptr<Call>> cInstances;      /// Calls derived from this function.

    // Private functions.
    std::string generateBindingDeclarations(std::vector<std::unique_ptr<Binding>>& bindings, unsigned int baseIndex) const;
    unsigned int getDynamicBindingIndex(const std::string& name);
    void declare();
    void revoke();
  };
}
