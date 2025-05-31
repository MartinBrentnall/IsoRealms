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
            Function(project, "") {
  }
    
  Function::Function(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            Function(project, object.getString(JSON_ID), object, nullptr, true) {
  }

  void Function::registerAssets(IAssetRegistry& assets) {
    assets.add(this, "", "Call Function");
  }
    
  void Function::unregisterAssets(IAssetRemover& remover, IAssets& releaser, bool relinquish) {
    remover.remove(this, relinquish);
    unregisterAssets(releaser);
  }
  
  void Function::save(JSONObject object, IAssetIdentifier& identifier, bool script) const {
    JSONArray mBindingsArray = object.addArray(JSON_BINDINGS);
    for (const std::unique_ptr<Binding>& mBinding : cDefBindings) {
      JSONObject mBindingObject = mBindingsArray.addObject();
      mBinding->save(mBindingObject, JSON_VARIABLE, identifier, JSON_TO); // TODO: JSON_TO is handled elsewhere!
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
    return renderAssetIcon();
  }

  std::vector<std::unique_ptr<IProperty>> Function::getProperties(IAssetBrowser& browser, IAssetRegistry& assets) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyStruct>("Bindings", "Edit...", [this]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      for (std::unique_ptr<Binding>& mBinding : cDefBindings) {
        mProperties.emplace_back(std::make_unique<PropertyStruct>(mBinding->getName(), "Edit...", [&mBinding]() {
          return mBinding->getProperties();
        }, [this, &mBinding]() {
          Utils::removeElementUnique(cDefBindings, mBinding.get());
        }));
      }
      mProperties.emplace_back(std::make_unique<PropertyAdd>("Binding", "New...", [this]() {
        Binding* mNewBinding = cDefBindings.emplace_back(std::make_unique<Binding>(*this, nullptr, getNextAvailableName("newBinding"))).get();
        return std::make_unique<PropertyStruct>(mNewBinding->getName(), "Edit...", [mNewBinding]() {
          return mNewBinding->getProperties();
        }, [this, mNewBinding]() {
          Utils::removeElementUnique(cDefBindings, mNewBinding);
        });
      }));
      return mProperties;
    }));
    mProperties.emplace_back(std::make_unique<PropertyStruct>("Arguments", "Edit...", [this]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      for (std::unique_ptr<ArgumentDefinition>& mArgumentDefinition : cDefArgumentDefinitions) {
        mProperties.emplace_back(std::make_unique<PropertyStruct>(mArgumentDefinition->getName(), "Edit...", [this, &mArgumentDefinition]() {
          return mArgumentDefinition->getProperties(*this);
        }, [this, &mArgumentDefinition]() {
          Utils::removeElementUnique(cDefArgumentDefinitions, mArgumentDefinition.get());
        }));
      }
      mProperties.emplace_back(std::make_unique<PropertyAdd>("Argument", "New...", [this]() {
        ArgumentDefinition* mNewArgumentDefinition = cDefArgumentDefinitions.emplace_back(std::make_unique<ArgumentDefinition>(cProject, getNextAvailableName("newArgument"))).get();
        return std::make_unique<PropertyStruct>(mNewArgumentDefinition->getName(), "Edit...", [this, mNewArgumentDefinition]() {
          return mNewArgumentDefinition->getProperties(*this);
        }, [this, mNewArgumentDefinition]() {
          Utils::removeElementUnique(cDefArgumentDefinitions, mNewArgumentDefinition);
        });
      }));
      return mProperties;
    }));
    mProperties.emplace_back(std::make_unique<PropertyCode>(cProject, "Code", [this]() {return cDefCode;}, [this](const std::string& value) {cDefCode = value;}));
    return mProperties;
  }
  
  Function::Function(IProject& project, const std::string& name) :
            cProject(project),
            cDefLuaState(project.getLuaState()->getState()),
            cDefName(name) {
  }

  Function::Function(IProject& project, const std::string& name, JSONObject object, IBindingRegistry* localArgs, bool init) :
            Function(project, name) {
    if (init) {
      for (JSONObject mArgumentObject : object.getArray(JSON_ARGUMENTS)) {
        cDefArgumentDefinitions.emplace_back(std::make_unique<ArgumentDefinition>(project, mArgumentObject));
      }
    }
    for (JSONObject mBindingObject : object.getArray(JSON_BINDINGS)) {
      cDefBindings.emplace_back(std::make_unique<Binding>(*this, localArgs, init, mBindingObject));
    }
    cDefCode = object.getString(JSON_CODE);
    declare();
  }

  void Function::unregisterAssets(IAssets& releaser) {
    for (std::unique_ptr<Binding>& mBinding : cDefBindings) {
      mBinding->release(releaser);
    }
//    for (std::unique_ptr<ArgumentDefinition>& mArgumentDefinition : cDefArgumentDefinitions) {
// TODO      mArgumentDefinition->release(releaser);
//    }
    for (auto& mInstance : cInstances) {
      mInstance.second->release(releaser);
    }
  }
  
  std::vector<std::unique_ptr<IProperty>> Function::getScriptProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyStruct>("Bindings", "Edit...", [this]() {
      std::vector<std::unique_ptr<IProperty>> mProperties;
      for (std::unique_ptr<Binding>& mBinding : cDefBindings) {
        mProperties.emplace_back(std::make_unique<PropertyStruct>(mBinding->getName(), "Edit...", [&mBinding]() {
          return mBinding->getProperties();
        }, [this, &mBinding]() {
          Utils::removeElementUnique(cDefBindings, mBinding.get());
        }));
      }
      mProperties.emplace_back(std::make_unique<PropertyAdd>("Binding", "New...", [this]() {
        Binding* mNewBinding = cDefBindings.emplace_back(std::make_unique<Binding>(*this, nullptr, getNextAvailableName("newBinding"))).get();
        return std::make_unique<PropertyStruct>(mNewBinding->getName(), "Edit...", [mNewBinding]() {
          return mNewBinding->getProperties();
        }, [this, mNewBinding]() {
          Utils::removeElementUnique(cDefBindings, mNewBinding);
        });
      }));
      return mProperties;
    }));
    mProperties.emplace_back(std::make_unique<PropertyCode>(cProject, "Code", [this]() {return cDefCode;}, [this](const std::string& value) {cDefCode = value;}));
    return mProperties;
  }

  IProject& Function::getProject() const {
    return cProject;
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

  IAction* Function::createAction(JSONObject object, IProject& project, IBindingRegistry* localArgs) {
    std::unique_ptr<Call> mInstance = std::make_unique<Call>(*this, object, project, localArgs);
    IAction* mKey = mInstance.get();
    cInstances.emplace(mKey, std::move(mInstance));
    return mKey;
  }

  IAction* Function::createAction(IProject& project, IBindingRegistry* localArgs) {
    std::unique_ptr<Call> mInstance = std::make_unique<Call>(*this, project, localArgs);
    IAction* mKey = mInstance.get();
    cInstances.emplace(mKey, std::move(mInstance));
    return mKey;
  }  

  void Function::destroyAction(IAction* action, IAssets& assets) {
    std::map<IAction*, std::unique_ptr<Call>>::iterator mInstance = cInstances.find(action);
    if (mInstance == cInstances.end()) {
      throw std::invalid_argument("Specified action isn't derived from this function");
    }
    mInstance->second->release(assets);
    cInstances.erase(action);
  }

  bool Function::renderAssetIcon() const {
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

  void Function::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> Function::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool Function::isDefaultConfiguration() const {
    return false; // TODO: Implement this.
  }

  Function::Call::Call(Function& parent, IProject& project, IBindingRegistry* localObjects) :
            cDefParent(parent),
            cDefLocalBindingRegistry(localObjects) {
    for (unsigned int i = 0; i < cDefParent.cDefArgumentDefinitions.size(); i++) {
      cDefArguments.emplace_back(std::make_unique<IsoRealms::Binding>(project, cDefLocalBindingRegistry, cDefParent.cDefArgumentDefinitions[i]->getType()));
    }
  }

  Function::Call::Call(Function& parent, JSONObject object, IProject& project, IBindingRegistry* localObjects) :
            Call(parent, project, localObjects) {
    if (!cDefParent.cDefArgumentDefinitions.empty()) {
      for (JSONObject mBindingObject : object.getArray(JSON_BINDINGS)) {
        std::string mArgumentName = mBindingObject.getString(JSON_ARGUMENT);
        unsigned int mBindingIndex = cDefParent.getDynamicBindingIndex(mArgumentName);
        cDefArguments[mBindingIndex] = std::make_unique<IsoRealms::Binding>(project, cDefLocalBindingRegistry, cDefParent.cDefArgumentDefinitions[mBindingIndex]->getType());
        cDefArguments[mBindingIndex]->set(mBindingObject, JSON_TO);
      }
    }
  }

  // TODO: What was the purpose of this?  I'm pretty sure this has no effect because the type of cDefArguments is different than asset?
//   void Function::Call::relinquish(IBinding* asset) {
//     for (unsigned int i = 0; i < cDefArguments.size(); i++) {
//       if (cDefArguments[i] == asset) {
//         cDefArguments[i] = std::make_unique<IsoRealms::Binding>(project, cDefLocalBindingRegistry);
//       }
//     }
//   }

  void Function::Call::release(IAssets& releaser) {
    // TODO: I think this needs enabling... to be investigated.
//     for (unsigned int i = 0; i < cDefArguments.size(); i++) {
//       if (!cDefLocal[i] && !cDefParent.cDefArgumentDefinitions[i]->isDefaultValue(cDefArguments[i])) {
//         releaser->release(cDefParent, cDefArguments[i]);
//       }
//     }
  }

  void Function::Call::execute() {
    // if (cDefParent.cDefName[0] != '_') {
    //   std::cout << "Executing Function \"" << cDefParent.cDefName << "\"..." << std::endl;
    // } else {
      std::cout << "\n\n\n\nExecuting \"" << cDefParent.cDefName  << "\": ===============================================================================" << std::endl << cDefParent.cDefCode << std::endl;
    // }
    try {
      for (unsigned int i = 0; i < cDefParent.cDefBindings.size(); i++) {
        std::string mBindFunctionName = cDefParent.cDefName + "_arg" + Utils::toString(i);
        IBinding* mBinding = cDefParent.cDefBindings[i]->getValue();
        mBinding->bind(mBindFunctionName);
      }
      for (unsigned int i = 0; i < cDefParent.cDefArgumentDefinitions.size(); i++) {
        std::string mBindFunctionName = cDefParent.cDefName + "_arg" + Utils::toString(static_cast<int>(i + cDefParent.cDefBindings.size()));
        (**cDefArguments[i])->bind(mBindFunctionName);
      }
      (*cDefParent.cDefLuaState)[cDefParent.cDefName]();
    } catch (sol::error& e) {
      std::cout << "Error in Script: " << e.what() << std::endl;
      throw e;
    }
  }

  void Function::Call::save(JSONObject object, IAssetIdentifier& identifier) const {
    JSONArray mBindingsArray = object.addArray(JSON_BINDINGS);
    for (unsigned int i = 0; i < cDefArguments.size(); i++) {
      JSONObject mBindingObject =  mBindingsArray.addObject();
      cDefParent.cDefArgumentDefinitions[i]->saveCall(mBindingObject, JSON_ARGUMENT);
      cDefArguments[i]->save(mBindingObject, JSON_TO);
    }
  }

  bool Function::Call::hasConfiguration() const {
    return !cDefArguments.empty();
  }

  std::vector<std::unique_ptr<IProperty>> Function::Call::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    for (unsigned int i = 0; i < cDefParent.cDefArgumentDefinitions.size(); i++) {
      std::string mArgumentName = cDefParent.cDefArgumentDefinitions[i]->getName();
      std::unique_ptr<IsoRealms::Binding>& mBinding = cDefArguments[i];
      mProperties.emplace_back(std::make_unique<PropertyAsset<IsoRealms::Binding>>(mArgumentName, *mBinding));
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
