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

namespace IsoRealms::Basics {
  const std::string Function::JSON_ARGUMENT      = "argument";
  const std::string Function::JSON_ARGUMENTS     = "arguments";
  const std::string Function::JSON_BINDINGS      = "bindings";
  const std::string Function::JSON_CODE          = "code";
  const std::string Function::JSON_DEFAULT_VALUE = "defaultValue";
  const std::string Function::JSON_ID            = "id";
  const std::string Function::JSON_TO            = "to";
  const std::string Function::JSON_TYPE          = "type";
  const std::string Function::JSON_VARIABLE      = "variable";

  Function::Function(IProject& project, Basics& basics, IResourceData& data) :
            Function(project, "", data.getDummyActionClient()) {
  }
    
  Function::Function(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            Function(project, object.getString(JSON_ID), data.getDummyActionClient(), object, true) {
  }

  void Function::registerAssets(ResourceAssetRegistry& assets) {
    assets.addProvider<IActionClient, IAction>(this, "", "Call Function");
  }
    
  void Function::save(JSONObject object, bool script) const {
    JSONArray mBindingsArray = object.addArray(JSON_BINDINGS);
    for (const std::unique_ptr<Binding>& mBinding : cDefBindings) {
      JSONObject mBindingObject = mBindingsArray.addObject();
      mBinding->save(mBindingObject, JSON_VARIABLE, JSON_TO); // TODO: JSON_TO is handled elsewhere!
    }

    if (!script) {
      JSONArray mArgumentsArray = object.addArray(JSON_ARGUMENTS);
      for (const std::unique_ptr<ArgumentDefinition>& mArgumentDefinition : cDefArgumentDefinitions) {
        JSONObject mArgumentObject = mArgumentsArray.addObject();
        mArgumentDefinition->save(mArgumentObject);
      }
    }
    object.addString(JSON_CODE, cDefCode);
  }

  void Function::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Function::renderIcon() const {
    return renderAssetProviderIcon();
  }

  std::vector<std::unique_ptr<IProperty>> Function::getProperties(IPropertyOwner& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyStruct>(owner.getPropertyData("Bindings"), "Edit...", [this, &owner]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      for (std::unique_ptr<Binding>& mBinding : cDefBindings) {
        mProperties.emplace_back(std::make_unique<PropertyStruct>(PropertyData(mBinding->getName(), "TODO"), "Edit...", [&mBinding]() {
          return mBinding->getProperties();
        }, [this, &mBinding]() {
          Utils::removeElementUnique(cDefBindings, mBinding.get());
        }));
      }
      mProperties.emplace_back(std::make_unique<PropertyAdd>(owner.getPropertyData("Binding"), "New...", [this, &owner]() {
        Binding* mNewBinding = cDefBindings.emplace_back(std::make_unique<Binding>(*this, owner.getDummyActionClient(), getNextAvailableName("newBinding"))).get();
        return std::make_unique<PropertyStruct>(PropertyData(mNewBinding->getName(), "TODO"), "Edit...", [mNewBinding]() {
          return mNewBinding->getProperties();
        }, [this, mNewBinding]() {
          Utils::removeElementUnique(cDefBindings, mNewBinding);
        });
      }));
      return mProperties;
    }));
    mProperties.emplace_back(std::make_unique<PropertyStruct>(owner.getPropertyData("Arguments"), "Edit...", [this, &owner]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      for (std::unique_ptr<ArgumentDefinition>& mArgumentDefinition : cDefArgumentDefinitions) {
        mProperties.emplace_back(std::make_unique<PropertyStruct>(PropertyData(mArgumentDefinition->getName(), "TODO"), "Edit...", [this, &mArgumentDefinition]() {
          return mArgumentDefinition->getProperties(*this);
        }, [this, &mArgumentDefinition]() {
          Utils::removeElementUnique(cDefArgumentDefinitions, mArgumentDefinition.get());
        }));
      }
      mProperties.emplace_back(std::make_unique<PropertyAdd>(owner.getPropertyData("Argument"), "New...", [this]() {
        ArgumentDefinition* mNewArgumentDefinition = cDefArgumentDefinitions.emplace_back(std::make_unique<ArgumentDefinition>(cProject, *this, getNextAvailableName("newArgument"))).get();
        return std::make_unique<PropertyStruct>(PropertyData(mNewArgumentDefinition->getName(), "TODO"), "Edit...", [this, mNewArgumentDefinition]() {
          return mNewArgumentDefinition->getProperties(*this);
        }, [this, mNewArgumentDefinition]() {
          Utils::removeElementUnique(cDefArgumentDefinitions, mNewArgumentDefinition);
        });
      }));
      return mProperties;
    }));
    mProperties.emplace_back(std::make_unique<PropertyCode>(cProject, owner.getPropertyData("Code"), [this]() {return cDefCode;}, [this](const std::string& value) {cDefCode = value;}));
    return mProperties;
  }
  
  Function::Function(IProject& project, const std::string& name, IActionClient& owner) :
            cProject(project),
            cResourceData(owner.getResourceData()),
            cDefLuaState(project.getLuaState()->getState()),
            cDefName(name) {
  }

  Function::Function(IProject& project, const std::string& name, IActionClient& owner, JSONObject object, bool init) :
            Function(project, name, owner) {
    if (init) {
      for (JSONObject mArgumentObject : object.getArray(JSON_ARGUMENTS)) {
        cDefArgumentDefinitions.emplace_back(std::make_unique<ArgumentDefinition>(project, *this, mArgumentObject));
      }
    }
    for (JSONObject mBindingObject : object.getArray(JSON_BINDINGS)) {
      cDefBindings.emplace_back(std::make_unique<Binding>(*this, owner, init, mBindingObject));
    }
    cDefCode = object.getString(JSON_CODE);
    declare();
  }

  std::vector<std::unique_ptr<IProperty>> Function::getScriptProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyStruct>(PropertyData("Bindings", "TODO"), "Edit...", [this]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      for (std::unique_ptr<Binding>& mBinding : cDefBindings) {
        mProperties.emplace_back(std::make_unique<PropertyStruct>(PropertyData(mBinding->getName(), "TODO"), "Edit...", [&mBinding]() {
          return mBinding->getProperties();
        }, [this, &mBinding]() {
          Utils::removeElementUnique(cDefBindings, mBinding.get());
        }));
      }
      mProperties.emplace_back(std::make_unique<PropertyAdd>(PropertyData("Binding", "TODO"), "New...", [this]() {
        Binding* mNewBinding = cDefBindings.emplace_back(std::make_unique<Binding>(*this, cResourceData.getDummyActionClient(), getNextAvailableName("newBinding"))).get();
        return std::make_unique<PropertyStruct>(PropertyData(mNewBinding->getName(), "TODO"), "Edit...", [mNewBinding]() {
          return mNewBinding->getProperties();
        }, [this, mNewBinding]() {
          Utils::removeElementUnique(cDefBindings, mNewBinding);
        });
      }));
      return mProperties;
    }));
    mProperties.emplace_back(std::make_unique<PropertyCode>(cProject, PropertyData("Code", "TODO"), [this]() {return cDefCode;}, [this](const std::string& value) {cDefCode = value;}));
    return mProperties;
  }

  IProject& Function::getProject() const {
    return cProject;
  }

  IResourceData& Function::getResourceData() const {
    return cResourceData;
  }

  bool Function::setBindingName(Binding& binding, const std::string& name) {
    Binding* mBinding = getBinding(name);
    if (mBinding != nullptr && mBinding != &binding) {
      return false;
    }
    binding.setName(name);
    return true;
  }

  bool Function::setArgumentDefinitionName(ArgumentDefinition& argumentDefinition, const std::string& name) {
    ArgumentDefinition* mArgumentDefinition = getArgumentDefinition(name);
    if (mArgumentDefinition != nullptr && mArgumentDefinition != &argumentDefinition) {
      return false;
    }
    argumentDefinition.setName(name);
    return true;
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

  IAction* Function::getAsset(IActionClient& owner, JSONObject object) {
    std::unique_ptr<Call> mInstance = std::make_unique<Call>(*this, owner, object);
    IAction* mKey = mInstance.get();
    cInstances.emplace(mKey, std::move(mInstance));
    return mKey;
  }

  IAction* Function::getAsset(IActionClient& owner) {
    std::unique_ptr<Call> mInstance = std::make_unique<Call>(*this, owner);
    IAction* mKey = mInstance.get();
    cInstances.emplace(mKey, std::move(mInstance));
    return mKey;
  }  

  void Function::releaseAsset(const IAction* asset) {
    std::map<const IAction*, std::unique_ptr<Call>>::iterator mInstance = cInstances.find(asset);
    if (mInstance == cInstances.end()) {
      throw std::invalid_argument("Specified action isn't derived from this function");
    }
    cInstances.erase(asset);
  }

  bool Function::hasConfiguration() const {
    return true;
  }

  bool Function::renderAssetProviderIcon() const {
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

  Function::Call::Call(Function& parent, IActionClient& owner) :
            cParent(parent),
            cOwner(owner) {
    for (unsigned int i = 0; i < cParent.cDefArgumentDefinitions.size(); i++) {
      cDefArguments.emplace_back(std::make_unique<IsoRealms::Binding>(owner, cParent.cDefArgumentDefinitions[i]->getType()));
    }
  }

  Function::Call::Call(Function& parent, IActionClient& owner, JSONObject object) :
            Call(parent, owner) {
    if (!cParent.cDefArgumentDefinitions.empty()) {
      for (JSONObject mBindingObject : object.getArray(JSON_BINDINGS)) {
        std::string mArgumentName = mBindingObject.getString(JSON_ARGUMENT);
        unsigned int mBindingIndex = cParent.getDynamicBindingIndex(mArgumentName);
        cDefArguments[mBindingIndex] = std::make_unique<IsoRealms::Binding>(owner, cParent.cDefArgumentDefinitions[mBindingIndex]->getType());
        cDefArguments[mBindingIndex]->set(mBindingObject, JSON_TO);
      }
    }
  }

  void Function::Call::execute() {
    // if (cParent.cDefName[0] != '_') {
    //   std::cout << "Executing Function \"" << cParent.cDefName << "\"..." << std::endl;
    // } else {
    //  std::cout << "\n\n\n\nExecuting \"" << cParent.cDefName  << "\": ===============================================================================" << std::endl << cParent.cDefCode << std::endl;
    // }
    for (unsigned int i = 0; i < cParent.cDefBindings.size(); i++) {
      std::string mBindFunctionName = cParent.cDefName + "_arg" + Utils::toString(i);
      IBinding* mBinding = cParent.cDefBindings[i]->getValue();
      mBinding->bind(mBindFunctionName);
    }
    for (unsigned int i = 0; i < cParent.cDefArgumentDefinitions.size(); i++) {
      std::string mBindFunctionName = cParent.cDefName + "_arg" + Utils::toString(static_cast<int>(i + cParent.cDefBindings.size()));
      (**cDefArguments[i])->bind(mBindFunctionName);
    }

    sol::protected_function mFunction = (*cParent.cDefLuaState)[cParent.cDefName];
    sol::protected_function_result mResult = mFunction();

    // We have to check for errors manually because Sol2 exception handling is broken.
    if (!mResult.valid()) {
      sol::error mError = mResult;
      std::cout << std::endl;
      std::cout << "Function \"" << cParent.cDefName << "\" failed: " << mError.what() << std::endl;
    }
  }

  bool Function::Call::renderAssetIcon() const {
    return cParent.renderIcon();
  }

  void Function::Call::saveAsset(JSONObject object) const {
    JSONArray mBindingsArray = object.addArray(JSON_BINDINGS);
    for (unsigned int i = 0; i < cDefArguments.size(); i++) {
      JSONObject mBindingObject =  mBindingsArray.addObject();
      cParent.cDefArgumentDefinitions[i]->saveCall(mBindingObject, JSON_ARGUMENT);
      cDefArguments[i]->save(mBindingObject, JSON_TO);
    }
  }

  std::vector<std::unique_ptr<IProperty>> Function::Call::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    for (unsigned int i = 0; i < cParent.cDefArgumentDefinitions.size(); i++) {
      std::string mArgumentName = cParent.cDefArgumentDefinitions[i]->getName();
      std::unique_ptr<IsoRealms::Binding>& mBinding = cDefArguments[i];
      mProperties.emplace_back(std::make_unique<PropertyAsset<IsoRealms::Binding>>(PropertyData(mArgumentName, "TODO"), *mBinding));
    }
    return mProperties;
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
    mFunctionDeclaration += "function " + cDefName + "()\n";
    mFunctionDeclaration += cDefCode + "\n";
    for (unsigned int i = 0; i < cDefBindings.size(); i++) {
      mFunctionDeclaration += cDefBindings[i]->getCleanup();
    }
    for (unsigned int i = 0; i < cDefArgumentDefinitions.size(); i++) {
      mFunctionDeclaration += cDefArgumentDefinitions[i]->getCleanup();
    }
    mFunctionDeclaration += "end\n";
    cDefLuaState->script(mFunctionDeclaration);
  }
  
  void Function::revoke() {
    // TODO: Implement and use this.  Must remove functions and tables from the Lua state machine that were created by declare().  Should be called on destruction, or whenever the configuration of the function is changed.
  }
}
