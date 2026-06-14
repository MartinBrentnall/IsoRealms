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
#include "Function.h"

#include <sol.hpp>
#include <unordered_map>

#include "IsoRealms/ComponentLoader.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  Function::Function(Basics& basics, IComponentData& data) :
            Function(basics, data.getDummyActionContext()) {
  }
    
  Function::Function(Basics& basics, IComponentData& data, JSONObject object) :
            Function(basics, data.getDummyActionContext()) {
    ComponentLoader mLoader(data, object);
    define(mLoader);
  }

  void Function::define(IComponentDefiner& definer) {
    addBindingPropertyArray(definer, true);
    definer.array("arguments", cDefArgumentDefinitions, [](const std::unique_ptr<ArgumentDefinition>& mArgumentDefinition) -> ArgumentDefinition& {return *mArgumentDefinition;}, [this, &definer](ArgumentDefinition& argumentDefinition) {
      definer.scope("Argument", argumentDefinition.getName(), [&argumentDefinition, this](IComponentDefiner& editingDefiner) {
        argumentDefinition.define(editingDefiner, *this);
      }, [this, &argumentDefinition]() {
        Utils::removeElementUnique(cDefArgumentDefinitions, &argumentDefinition);
      });
    }, [this]() -> ArgumentDefinition& {
      std::string mName = getNextAvailableName("newArgument");
      return *cDefArgumentDefinitions.emplace_back(std::make_unique<ArgumentDefinition>(*this, mName, mName));
    });
    definer.propertyCode(JSON_CODE, [this]() {return cDefCode;}, [this](const std::string& value) {
      cDefCode = value;
    });
    if (definer.loadsPersistedValues()) {
      declare();
    }
  }

  void Function::publish(ResourcePublisher& publisher) {
    publisher.publishProvider<IActionContext, IAction>(this, "", "Call a Function");
  }

  bool Function::renderIcon() const {
    return renderResourceProviderIcon();
  }

  Function::Function(Basics& basics, IActionContext& owner) :
            cProject(basics.getProject()),
            cBasics(basics),
            cComponentData(owner.getComponentData()),
            cDefActionContext(owner),
            cDefLuaState(basics.getProject().getLuaState().getState()),
            cDefID(basics.getAvailableFunctionID()) {
  }

  Function::Function(Basics& basics, IActionContext& owner, JSONObject object, bool init) :
            Function(basics, owner) {
    ComponentLoader mLoader(cComponentData, object);
    if (init) {
      define(mLoader);
    } else {
      getScriptProperties(mLoader);
    }
  }

  void Function::addBindingPropertyArray(IComponentDefiner& definer, bool init) {
    definer.array(JSON_BINDINGS, cDefBindings, [](const std::unique_ptr<Binding>& mBinding) -> Binding& {return *mBinding;}, [this, &definer, init](Binding& binding) {
      definer.scope("Binding", binding.getName(), [&binding, init](IComponentDefiner& editingDefiner) {
        binding.getProperties(editingDefiner, init);
      }, [this, &binding]() {
        Utils::removeElementUnique(cDefBindings, &binding);
      });
    }, [this]() -> Binding& {
      return *cDefBindings.emplace_back(std::make_unique<Binding>(*this, cDefActionContext, getNextAvailableName("newBinding")));
    });
  }

  void Function::getScriptProperties(IComponentDefiner& definer) {
    addBindingPropertyArray(definer, false);
    definer.propertyCode(JSON_CODE, [this]() {return cDefCode;}, [this](const std::string& value) {
      cDefCode = value;
    });
    if (definer.loadsPersistedValues()) {
      declare();
    }
  }

  IsoRealms::Project& Function::getProject() const {
    return cProject;
  }

  IComponentData& Function::getComponentData() const {
    return cComponentData;
  }

  void Function::setBindingName(Binding& binding, const std::string& name) {
    binding.setName(name);
  }

  bool Function::isBindingNameAllowed(Binding& binding, const std::string& name) {
    Binding* mBinding = getBinding(name);
    return mBinding == nullptr || mBinding == &binding;
  }

  void Function::setArgumentDefinitionName(ArgumentDefinition& argumentDefinition, const std::string& name) {
    argumentDefinition.setName(name);
  }

  bool Function::isArgumentDefinitionNameAllowed(ArgumentDefinition& argumentDefinition, const std::string& name) {
    ArgumentDefinition* mArgumentDefinition = getArgumentDefinition(name);
    return mArgumentDefinition == nullptr || mArgumentDefinition == &argumentDefinition;
  }

  Binding* Function::getBinding(const std::string& name) {
    for (std::unique_ptr<Binding>& mBinding : cDefBindings) {
      if (mBinding->getName() == name) {
        return mBinding.get();
      }
    }
    return nullptr;
  }

  ArgumentDefinition* Function::getArgumentDefinition(const std::string& name) {
    for (std::unique_ptr<ArgumentDefinition>& mArgumentDefinition : cDefArgumentDefinitions) {
      if (mArgumentDefinition->getName() == name) {
        return mArgumentDefinition.get();
      }
    }
    return nullptr;
  }

  std::string Function::getNextAvailableName(const std::string& name) {
    std::string mProposedName = name;
    int mCount = 1;
    while (getBinding(mProposedName) != nullptr || getArgumentDefinition(mProposedName) != nullptr) {
      mProposedName = name + Utils::toString(mCount++);
    }
    return mProposedName;
  }

  unsigned int Function::getID() const {
    return cDefID;
  }

  IAction* Function::getResource(IActionContext& owner, JSONObject object) {
    std::unique_ptr<Call> mInstance = std::make_unique<Call>(*this, owner, object);
    IAction* mKey = mInstance.get();
    cInstances.emplace(mKey, std::move(mInstance));
    return mKey;
  }

  IAction* Function::getResource(IActionContext& owner) {
    std::unique_ptr<Call> mInstance = std::make_unique<Call>(*this, owner);
    IAction* mKey = mInstance.get();
    cInstances.emplace(mKey, std::move(mInstance));
    return mKey;
  }  

  void Function::releaseResource(const IAction* asset) {
    std::map<const IAction*, std::unique_ptr<Call>>::iterator mInstance = cInstances.find(asset);
    if (mInstance == cInstances.end()) {
      throw std::invalid_argument("Specified action isn't derived from this function");
    }
    cInstances.erase(asset);
  }

  bool Function::hasConfiguration() const {
    return true;
  }

  bool Function::renderResourceProviderIcon() const {
    glBindTexture(GL_TEXTURE_2D, 0);
    glLineWidth(4.0f);
    glColor3f(1.0f, 1.0f, 0.5f);
    Utils::renderBezier(Utils::Point2D(-0.8f, -0.6f), Utils::Point2D(-0.4f, -1.4f), Utils::Point2D(-0.4f, 1.4f), Utils::Point2D(0.0f, 0.6f), 32);
    glBegin(GL_LINES);
    glVertex2f(-0.7f,  0.2f); glVertex2f(0.0f,  0.2f);
    glVertex2f(-0.1f, -0.1f); glVertex2f(0.4f, -0.8f);
    glVertex2f(-0.1f, -0.8f); glVertex2f(0.4f, -0.1f);
    glEnd();
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    return true;
  }

  bool Function::isHiddenProvider() const {
    return false;
  }

  const Metadata& Function::getMetadata() const {
    return cComponentData.getMetadata();
  }

  Function::Call::Call(Function& parent, IActionContext& owner) :
            cParent(parent),
            cOwner(owner) {
    for (unsigned int i = 0; i < cParent.cDefArgumentDefinitions.size(); i++) {
      cDefArguments.emplace_back(std::make_unique<IsoRealms::Binding>(owner, cParent.cDefArgumentDefinitions[i]->getType()));
    }
  }

  Function::Call::Call(Function& parent, IActionContext& owner, JSONObject object) :
            Call(parent, owner) {
    if (!cParent.cDefArgumentDefinitions.empty()) {
      for (JSONValue mBindingValue : object.getArray(JSON_BINDINGS)) {
        JSONObject mBindingObject = mBindingValue.getObject();
        std::string mArgumentName = mBindingObject.getString(JSON_ARGUMENT);
        unsigned int mBindingIndex = cParent.getDynamicBindingIndex(mArgumentName);
        cDefArguments[mBindingIndex] = std::make_unique<IsoRealms::Binding>(owner, cParent.cDefArgumentDefinitions[mBindingIndex]->getType());
        cDefArguments[mBindingIndex]->set(mBindingObject, JSON_TO);
      }
    }
  }

  void Function::Call::execute() {
    // std::cout << "Executing function \"" << cParent.cComponentData.getComponentName() << "\" on behalf of owner \"" << cOwner.getComponentData().getComponentID() << "\"" << std::endl;
//    std::cout << "===============================================================================" << std::endl << cParent.cDefCode << std::endl;
    for (unsigned int i = 0; i < cParent.cDefBindings.size(); i++) {
      std::string mBindFunctionName = "func" + Utils::toString(cParent.cDefID) + "_arg" + Utils::toString(i);
      IBinding* mBinding = cParent.cDefBindings[i]->getValue();
      mBinding->bind(mBindFunctionName);
    }
    for (unsigned int i = 0; i < cParent.cDefArgumentDefinitions.size(); i++) {
      std::string mBindFunctionName = "func" + Utils::toString(cParent.cDefID) + "_arg" + Utils::toString(static_cast<int>(i + cParent.cDefBindings.size()));
      (**cDefArguments[i])->bind(mBindFunctionName);
    }

    sol::protected_function mFunction = cParent.cDefLuaState["func" + Utils::toString(cParent.cDefID)];
    sol::protected_function_result mResult = mFunction();

    // We have to check for errors manually because Sol2 exception handling is broken.
    if (!mResult.valid()) {
      sol::error mError = mResult;
      std::cout << std::endl;
      std::cout << "Function \"" << cParent.cDefID << "\" failed: " << mError.what() << std::endl;
      std::cout << "Function code: " << std::endl << cParent.cDefCode << std::endl;
    }
  }

  bool Function::Call::renderResourceIcon() const {
    return cParent.renderIcon();
  }

  void Function::Call::saveResource(JSONObject object) const {
    JSONArray mBindingsArray = object.addArray(JSON_BINDINGS);
    for (unsigned int i = 0; i < cDefArguments.size(); i++) {
      JSONObject mBindingObject =  mBindingsArray.addObject();
      cParent.cDefArgumentDefinitions[i]->saveCall(mBindingObject, JSON_ARGUMENT);
      cDefArguments[i]->save(mBindingObject, JSON_TO);
    }
  }

  void Function::Call::getResourceProperties(IComponentDefiner& definer) {
    std::unordered_map<std::string, unsigned int> mArgumentIndices;
    for (unsigned int i = 0; i < cParent.cDefArgumentDefinitions.size(); i++) {
      mArgumentIndices.emplace(cParent.cDefArgumentDefinitions[i]->getName(), i);
    }
    definer.fixedArray(JSON_BINDINGS, cDefArguments, [](const std::unique_ptr<IsoRealms::Binding>& binding) -> IsoRealms::Binding& {return *binding;}, [this, &definer](IsoRealms::Binding& binding, unsigned int index) {
      Options mHint;
      mHint.addOption("name", cParent.cDefArgumentDefinitions[index]->getName());
      mHint.addOption("description", "An argument to the function.");
      definer.propertyResource(JSON_TO, binding, mHint);
    }, [mArgumentIndices](const JSONObject& loadObject) -> unsigned int {
      return mArgumentIndices.at(loadObject.getString(JSON_ARGUMENT));
    });
  }

  bool Function::Call::isDefaultConfiguration() const {
    return false; // TODO: Implement this!
  }
  
  unsigned int Function::getDynamicBindingIndex(const std::string& name) {
    for (unsigned int i = 0; i < cDefArgumentDefinitions.size(); i++) {
      if (cDefArgumentDefinitions[i]->getName() == name) {
        return i;
      }
    }
    throw ArgumentException("ERROR: Function::getDynamicBindingIndex: Dynamic binding \"" + name + "\" not found.");
  }

  void Function::declare() {
    std::string mFunctionDeclaration;
    mFunctionDeclaration += generateBindingDeclarations(cDefBindings,  0);
    mFunctionDeclaration += generateBindingDeclarations(cDefArgumentDefinitions, static_cast<unsigned int>(cDefBindings.size()));
    mFunctionDeclaration += "function func" + Utils::toString(cDefID) + "()\n";
    mFunctionDeclaration += cDefCode + "\n";
    for (unsigned int i = 0; i < cDefBindings.size(); i++) {
      mFunctionDeclaration += cDefBindings[i]->getCleanup();
    }
    for (unsigned int i = 0; i < cDefArgumentDefinitions.size(); i++) {
      mFunctionDeclaration += cDefArgumentDefinitions[i]->getCleanup();
    }
    mFunctionDeclaration += "end\n";
    cDefLuaState.script(mFunctionDeclaration);
  }
  
  void Function::revoke() {
    // TODO: Implement and use this.  Must remove functions and tables from the Lua state machine that were created by declare().  Should be called on destruction, or whenever the configuration of the function is changed.
  }
}
