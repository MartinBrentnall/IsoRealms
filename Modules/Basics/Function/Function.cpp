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
  const std::string Function::JSON_NAME          = "name";
  const std::string Function::JSON_TO            = "to";
  const std::string Function::JSON_VARIABLE      = "variable";

  Function::Function(IProject* project, Basics* basics) :
            Function(project, "") {
  }
    
  Function::Function(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data) :
            Function(project, object.getString(JSON_NAME), object, nullptr, true) {
  }

  void Function::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Call Function");
  }
    
  void Function::unregisterAssets(IAssetRemover* remover, IAssets* releaser) {
    if (remover != nullptr) {
      remover->remove(this);
    }
    for (std::unique_ptr<Binding>& mBinding : cDefFixedBindings) {
      mBinding->release(releaser);
    }
    for (std::unique_ptr<Binding>& mBinding : cDefDynamicBindings) {
      mBinding->release(releaser);
    }
    for (auto& mInstance : cInstances) {
      mInstance.second->release(releaser);
    }
  }
  
  void Function::save(JSONObject object, IAssetIdentifier* identifier, bool script) const {
    if (!script) {
      object.addString(JSON_NAME, cDefName);
    }

    JSONArray mBindingsArray = object.addArray(JSON_BINDINGS);
    for (const std::unique_ptr<Binding>& mBinding : cDefFixedBindings) {
      JSONObject mBindingObject = mBindingsArray.addObject();
      mBinding->save(mBindingObject, JSON_VARIABLE, identifier, JSON_TO); // TODO: JSON_TO is handled elsewhere!
    }

    if (!script) {
      JSONArray mArgumentsArray = object.addArray(JSON_ARGUMENTS);
      for (const std::unique_ptr<Binding>& mBinding : cDefDynamicBindings) {
        JSONObject mArgumentObject = mArgumentsArray.addObject();
        mBinding->save(mArgumentObject, JSON_NAME, identifier, JSON_DEFAULT_VALUE);
      }
    }
    object.addString(JSON_CODE, cDefCode);
  }

  void Function::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool Function::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> Function::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }
  
  Function::Function(IProject* project, const std::string& name) :
            cDefLuaState(project->getLuaState()->getState()),
            cDefName(name) {
  }

  Function::Function(IProject* project, const std::string& name, JSONObject object, IBindingRegistry* localArgs, bool init) :
            Function(project, name) {
    if (init) {
      for (JSONObject mArgumentObject : object.getArray(JSON_ARGUMENTS)) {
        cDefDynamicBindings.emplace_back(std::make_unique<Binding>(mArgumentObject, JSON_NAME, JSON_DEFAULT_VALUE, project, localArgs, init));
      }
    }
    for (JSONObject mBindingObject : object.getArray(JSON_BINDINGS)) {
      cDefFixedBindings.emplace_back(std::make_unique<Binding>(mBindingObject, JSON_VARIABLE, JSON_TO, project, localArgs, init));
    }
    cDefCode = object.getString(JSON_CODE);
    declare();
  }

  IAction* Function::createAction(JSONObject object, IProject* project, IBindingRegistry* localObjects) {
    std::unique_ptr<Call> mInstance = std::make_unique<Call>(this, object, project, localObjects);
    IAction* mKey = mInstance.get();
    cInstances.emplace(mKey, std::move(mInstance));
    return mKey;
  }

  IAction* Function::createAction(IProject* project, IBindingRegistry* localArgs) {
    std::cout << "FIXME:IsoRealmsEditor: createAction() for editing needs to be implemented." << std::endl;
//     std::vector<Binding> mBindings;
//     std::vector<bool> mLocal;
//     std::unique_ptr<Call> mAction = std::make_unique<Call>(this, mBindings, mLocal);
//     cInstances[mAction] = mAction;
//     return mAction;
    return nullptr;
  }  

  void Function::destroyAction(IAction* action, IAssets* assets) {
    std::map<IAction*, std::unique_ptr<Call>>::iterator mInstance = cInstances.find(action);
    if (mInstance == cInstances.end()) {
      throw std::invalid_argument("Specified action isn't derived from this function");
    }
    mInstance->second->release(assets);
    cInstances.erase(action);
  }

  bool Function::renderAssetIcon() const {
    return false;
  }

  void Function::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  Function::Call::Call(Function* parent, JSONObject object, IProject* project, IBindingRegistry* localObjects) :
            cDefParent(parent),
            cDefLocalBindingRegistry(localObjects) {
    if (!cDefParent->cDefDynamicBindings.empty()) {
      for (unsigned int i = 0; i < cDefParent->cDefDynamicBindings.size(); i++) {
        cDefBindings.emplace_back(nullptr);
      }

      for (JSONObject mBindingObject : object.getArray(JSON_BINDINGS)) {
        std::string mArgumentName = mBindingObject.getString(JSON_ARGUMENT);
        unsigned int mBindingIndex = cDefParent->getDynamicBindingIndex(mArgumentName);
        cDefBindings[mBindingIndex] = std::make_unique<IsoRealms::Binding>(project, cDefLocalBindingRegistry);
        cDefBindings[mBindingIndex]->set(mBindingObject, JSON_TO);
      }
    }
  }

  // TODO: What was the purpose of this?  I'm pretty sure this has no effect because the type of cDefBindings is different than asset?
//   void Function::Call::relinquish(IBinding* asset) {
//     for (unsigned int i = 0; i < cDefBindings.size(); i++) {
//       if (cDefBindings[i] == asset) {
//         cDefBindings[i] = nullptr;
//       }
//     }
//   }

  void Function::Call::release(IAssets* releaser) {
    // TODO: I think this needs enabling... to be investigated.
//     for (unsigned int i = 0; i < cDefBindings.size(); i++) {
//       if (!cDefLocal[i] && cDefBindings[i] != nullptr && !cDefParent->cDefDynamicBindings[i]->isDefaultValue(cDefBindings[i])) {
//         releaser->release(cDefParent, cDefBindings[i]);
//       }
//     }
  }

  void Function::Call::execute() {
//    if (cDefParent->cDefName[0] != '_') {
//      std::cout << "Executing Function \"" << cDefParent->cDefName << "\"..." << std::endl;
//    } else {
//      std::cout << "Executing Script: ===============================================================================" << std::endl << cDefParent->cDefCode << std::endl;
//    }
    try {
      for (unsigned int i = 0; i < cDefParent->cDefFixedBindings.size(); i++) {
        std::string mBindFunctionName = cDefParent->cDefName + "_arg" + Utils::toString(i);
        IBinding* mBinding = cDefParent->cDefFixedBindings[i]->getValue();
        mBinding->bind(mBindFunctionName);
      }
      for (unsigned int i = 0; i < cDefParent->cDefDynamicBindings.size(); i++) {
        std::string mBindFunctionName = cDefParent->cDefName + "_arg" + Utils::toString(static_cast<int>(i + cDefParent->cDefFixedBindings.size()));
        if (cDefBindings[i] != nullptr) {
          (**cDefBindings[i])->bind(mBindFunctionName);
        } else {
          IBinding* mDefaultBinding = cDefParent->cDefDynamicBindings[i]->getValue();
          if (mDefaultBinding != nullptr) {
            mDefaultBinding->bind(mBindFunctionName);
          } else {
            std::cout << "WARNING: Function::Call::execute: No value bound for function \"" << mBindFunctionName << "\"" << std::endl;
            // TODO: Throw something
          }
        }
      }
      (*cDefParent->cDefLuaState)[cDefParent->cDefName]();
    } catch (sol::error& e) {
      std::cout << "Error in Script: " << e.what() << std::endl;
      throw e;
    }
  }

  IActionType* Function::Call::getActionType() const {
    return cDefParent;
  }

  void Function::Call::save(JSONObject object, IAssetIdentifier* identifier) const {
    JSONArray mBindingsArray = object.addArray(JSON_BINDINGS);
    for (unsigned int i = 0; i < cDefBindings.size(); i++) {
      if (cDefBindings[i] != nullptr) {
        JSONObject mBindingObject =  mBindingsArray.addObject();
        cDefParent->cDefDynamicBindings[i]->saveCall(mBindingObject, JSON_ARGUMENT);
        cDefBindings[i]->save(mBindingObject, JSON_TO);
      }
    }
  }

  bool Function::Call::hasConfiguration() const {
    return !cDefBindings.empty();
  }

  unsigned int Function::getDynamicBindingIndex(const std::string& name) {
    for (unsigned int i = 0; i < cDefDynamicBindings.size(); i++) {
      if (cDefDynamicBindings[i]->getName() == name) {
        return i;
      }
    }
    throw ArgumentException("ERROR: Function::getDynamicBindingIndex: Dynamic binding \"" + name + "\" not found.");
  }

  std::string Function::generateBindingDeclarations(std::vector<std::unique_ptr<Binding>>& bindings, unsigned int baseIndex) const {
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
  
  void Function::declare() {
    std::string mFunctionDeclaration;
    mFunctionDeclaration += generateBindingDeclarations(cDefFixedBindings,  0);
    mFunctionDeclaration += generateBindingDeclarations(cDefDynamicBindings, static_cast<unsigned int>(cDefFixedBindings.size()));
    mFunctionDeclaration += "function " + cDefName + "()\n";
    mFunctionDeclaration += cDefCode + "\n";
    for (unsigned int i = 0; i < cDefFixedBindings.size(); i++) {
      mFunctionDeclaration += cDefFixedBindings[i]->getCleanup();
    }
    for (unsigned int i = 0; i < cDefDynamicBindings.size(); i++) {
      mFunctionDeclaration += cDefDynamicBindings[i]->getCleanup();
    }
    mFunctionDeclaration += "end\n";
    cDefLuaState->script(mFunctionDeclaration);
  }
  
  void Function::revoke() {
    // TODO: Implement and use this.  Must remove functions and tables from the Lua state machine that were created by declare().  Should be called on destruction, or whenever the configuration of the function is changed.
  }

  // TODO: Does this need to be moved somewhere?
//   void Function::relinquish(IBinding* asset, IAssetLiterals* assets) {
//     for (std::pair<IAction*, Call*> mInstance : cInstances) {
//       mInstance.second->relinquish(asset, assets);
//     }
//   }
}
