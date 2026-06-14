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

#include "IsoRealms.h"

#include "Function/Function.h"

namespace IsoRealms::Basics {
  
  /**
   * Fixed resource definition for scripts (in-line functions).
   */
  class Script : public IResourceProvider<IActionContext, IAction> {
    public:
    Script(Basics& basics);

    unsigned int getNextAvailableFunctionID(unsigned int functionID) const;

    /*********************************************************\
     * Implements IResourceProvider<IActionContext, IAction> *
    \*********************************************************/
    IAction* getResource(IActionContext& owner, JSONObject object) override;
    IAction* getResource(IActionContext& owner) override;
    void releaseResource(const IAction* resource) override;
    bool hasConfiguration() const override;
    bool renderResourceProviderIcon() const override;
    bool isHiddenProvider() const override;
    const Metadata& getMetadata() const override;
    
    private:
    class ScriptAction : public IAction {
      public:
      ScriptAction(Script& parent, IActionContext& owner, unsigned int index, JSONObject object);
      ScriptAction(Script& parent, IActionContext& owner, unsigned int index);

      void destroyInternalAction();
      unsigned int getFunctionID() const;
      unsigned int getIndex() const;

      /**********************\
       * Implements IAction *
      \**********************/
      void execute() override;
      bool renderResourceIcon() const override;
      void getResourceProperties(IComponentDefiner& definer) override;
      
      private:
      
      // Definition data.
      Script& cParent;
      Function cDefFunction;  /// Function of this action.
      IAction* cDefAction;    /// Script action call.
      unsigned int cDefIndex; /// Index number of the function call.
    };
      
    // External interfaces.
    Basics& cBasics;

    // Functions and actions created by scripting.
    mutable std::map<const IAction*, std::unique_ptr<ScriptAction>> cDefScriptActions;
    
    // Private functions.
    unsigned int getNextAvailableIndex() const;
  };
}
