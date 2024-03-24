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
  const std::string Function::TAG_ARGUMENT      = "Argument";
  const std::string Function::TAG_BIND          = "Bind";
  const std::string Function::TAG_CODE          = "Code";
  const std::string Function::TAG_DEFAULT_VALUE = "DefaultValue";
  const std::string Function::TAG_TO            = "To";

  const std::string Function::ATTRIBUTE_ARGUMENT = "argument";
  const std::string Function::ATTRIBUTE_NAME     = "name";
  const std::string Function::ATTRIBUTE_TYPE     = "type";
  const std::string Function::ATTRIBUTE_VARIABLE = "variable";

  Function::Function(IProject* project, Basics* basics) :
            Function(project, "") {
  }
    
  Function::Function(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data) :
            Function(project, node.getAttribute(ATTRIBUTE_NAME), node, nullptr) {
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
  
  void Function::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    node->addAttribute(ATTRIBUTE_NAME, cDefName);
    
    for (const std::unique_ptr<Binding>& mBinding : cDefFixedBindings) {
      DOMNodeWriter mBindingNode = node->addBranch(TAG_BIND);
      mBinding->save(&mBindingNode, ATTRIBUTE_VARIABLE, identifier, false, TAG_TO); // TODO: TAG_TO is handled elsewhere!
    }
    for (const std::unique_ptr<Binding>& mBinding : cDefDynamicBindings) {
      DOMNodeWriter mBindingNode = node->addBranch(TAG_ARGUMENT);
      mBinding->save(&mBindingNode, ATTRIBUTE_NAME, identifier, false, TAG_DEFAULT_VALUE);
    }
    DOMNodeWriter mCodeBranch = node->addBranch(TAG_CODE);
    mCodeBranch.addText(cDefCode);
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

  Function::Function(IProject* project, const std::string& name, DOMNode& node, IBindingRegistry* localArgs) :
            Function(project, name) {
    for (DOMNode& mNode : node) {
      std::string mChildName = mNode.getName();
      if (mChildName == TAG_ARGUMENT) { // TODO: Shouldn't be allowed for scripts
        cDefDynamicBindings.emplace_back(std::make_unique<Binding>(mNode, ATTRIBUTE_NAME, TAG_DEFAULT_VALUE, project, localArgs));
      } else if (mChildName == TAG_BIND) {
        cDefFixedBindings.emplace_back(std::make_unique<Binding>(mNode, ATTRIBUTE_VARIABLE, TAG_TO, project, localArgs));
      } else if (mChildName == TAG_CODE) {
        cDefCode = mNode.getStringValue();
      } else {
        throw ParseException("Unknown tag for Basics/Function: " + mChildName);
      }
    }
    declare();
  }

  IAction* Function::createAction(DOMNode& node, IProject* project, IBindingRegistry* localObjects) {
    std::unique_ptr<Call> mInstance = std::make_unique<Call>(this, node, project, localObjects);
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

  Function::Call::Call(Function* parent, DOMNode& node, IProject* project, IBindingRegistry* localObjects) :
            cDefParent(parent),
            cDefLocalBindingRegistry(localObjects) {
    if (!cDefParent->cDefDynamicBindings.empty()) {
      for (unsigned int i = 0; i < cDefParent->cDefDynamicBindings.size(); i++) {
        cDefBindings.emplace_back(nullptr);
        cDefLocal.push_back(false);
      }

      for (DOMNode& mNode : node) {
        std::string mChildName = mNode.getName();
        if (mChildName == TAG_BIND) {
          std::string mValue = mNode.getAttribute(TAG_TO);
          std::string mArgumentName = mNode.getAttribute(ATTRIBUTE_ARGUMENT);
          unsigned int mBindingIndex = cDefParent->getDynamicBindingIndex(mArgumentName);
          if (mValue[0] == '~') {
            cDefLocal[mBindingIndex] = true;
          }
          cDefBindings[mBindingIndex] = std::make_unique<IsoRealms::Binding>(project, cDefLocalBindingRegistry);
          cDefBindings[mBindingIndex]->init(mNode.getNode(TAG_TO));
        } else if (mChildName == TAG_CODE) {
          // Script support.
        } else {
          throw ParseException("Unknown tag for Basics/Function: " + mChildName);
        }
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

  void Function::Call::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    identifier->save(node, cDefParent);
    for (unsigned int i = 0; i < cDefBindings.size(); i++) {
      if (cDefBindings[i] != nullptr) {
        DOMNodeWriter mBindBranch = node->addBranch(TAG_BIND);
        cDefParent->cDefDynamicBindings[i]->saveCall(&mBindBranch, ATTRIBUTE_ARGUMENT);
        cDefBindings[i]->save(&mBindBranch, cDefLocal[i], TAG_TO);
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
