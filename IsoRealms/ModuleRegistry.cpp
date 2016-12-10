/*
 * Copyright 2009 Martin Brentnall
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
#include "ModuleRegistry.h"

void ModuleRegistry::registerModule(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResources* resources, IResourceTypeRegistry* resourceTypeRegistry, IProjectOptions* options) {
  std::string mType = node->getAttribute("name");
  std::vector<std::string> mDirectory;
  mDirectory.push_back(mType);
  RuntimeContext* mRuntimeContext = new RuntimeContext(resources, mDirectory);
  std::cout << "Loading Module \"" << mType  << "\"" << std::endl;
  loadModule(mType, mRuntimeContext, resourceTypeRegistry);
  IModule* mModule = getModule(mType);
  IModuleOptions* mModuleOptions = options != nullptr ? options->getModuleOptions(mType) : nullptr;
  mModule->load(node, cache, mRuntimeContext, mModuleOptions);
}

void ModuleRegistry::loadModule(std::string& type, IResourceRegistry* resourceRegistry, IResourceTypeRegistry* resourceTypeRegistry) {
  // TODO: Test if it exists already!
  std::string mModuleLocation = System::getConfigurationResource("Modules/" + type + "/libModule");
  void* mModuleSO = dlopen(mModuleLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  if (!mModuleSO) {
    std::cout << dlerror() << std::endl;
    throw InitException("Cannot load library: " + std::string(dlerror()));
  }
  createModule* createModuleFunction = cast_voidptr_to_funcptr<createModule*>(dlsym(mModuleSO, "create"));
  const char* mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  destroyModule* destroyModuleFunction = cast_voidptr_to_funcptr<destroyModule*>(dlsym(mModuleSO, "destroy"));
  mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  InitLuaFunction* initLuaFunction = cast_voidptr_to_funcptr<InitLuaFunction*>(dlsym(mModuleSO, "initLua"));
  mDlsymError = dlerror();
  if (!mDlsymError) {
    std::cout << "Lua support found for \"" << type << "\"" << std::endl;
    Configuration* mConfiguration = Configuration::getInstance();
    mConfiguration->enableLuaSupport(initLuaFunction);
  }
  cModuleInstances[type] = createModuleFunction(resourceTypeRegistry);
  cDestroyFunctions[cModuleInstances[type]] = destroyModuleFunction;
  cImplementationNames[cModuleInstances[type]] = type;
  cSOHandles[type][type] = mModuleSO;
}

std::vector<std::string> ModuleRegistry::getInstances() {
  std::vector<std::string> mInstanceNames;
  for(std::map<std::string, IModule*>::iterator i = cModuleInstances.begin(); i != cModuleInstances.end(); i++) {
    mInstanceNames.push_back(i->first);
  }
  return mInstanceNames;
}

IModule* ModuleRegistry::getModule(std::string& type) {
  return cModuleInstances[type];
}

std::string ModuleRegistry::getInstanceName(IModule* instance) {
  for (std::map<std::string, IModule*>::iterator i = cModuleInstances.begin(); i != cModuleInstances.end(); i++) {
    if (i->second == instance) {
      return i->first;
    }
  }
  return "";
}

std::string ModuleRegistry::getEntityPath(IModule* module) {
  std::string mType = getModuleType(module);
  return "Module/" + mType + "/";
}

void ModuleRegistry::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (std::map<std::string, IModule*>::iterator j = cModuleInstances.begin(); j != cModuleInstances.end(); j++) {
    IModule* mInstance = j->second;
    std::string mTypeName = j->first;
    DOMNodeWriter* mModuleBranch = node->addBranch("Module");
    mModuleBranch->addAttribute("name", mTypeName);
    mInstance->save(mModuleBranch, resourceLocator);
  }
}

void ModuleRegistry::saveCache(DOMNodeWriter* cache) {
  for (std::map<std::string, IModule*>::iterator j = cModuleInstances.begin(); j != cModuleInstances.end(); j++) {
    IModule* mInstance = j->second;
    std::string mTypeName = j->first;
    mInstance->saveCache(cache);
  }
}

std::string ModuleRegistry::getModuleType(IModule* instance) {
  for (std::map<std::string, IModule*>::iterator j = cModuleInstances.begin(); j != cModuleInstances.end(); j++) {
    if (instance == j->second) {
      return j->first;
    }
  }
  // TODO: Throw exception
  std::cout << "Warning: module type was not found!" << std::endl;
  return "";
}

void ModuleRegistry::removeModule(IModule* instance) {
  std::string mImplementation = cImplementationNames.find(instance)->second;
  destroyModule* mDestroyFunction = cDestroyFunctions.find(instance)->second;
  std::string mTypeName = getModuleType(instance);
  mDestroyFunction(instance);
  void* mHandleToClose = cSOHandles[mTypeName][mImplementation];

  for (std::map<std::string, IModule*>::iterator j = cModuleInstances.begin(); j != cModuleInstances.end(); j++) {
    if (instance == j->second) {
      cModuleInstances.erase(j);
    }
  }
  // TODO: Remove from cSOHandles if it was the last one!
  dlclose(mHandleToClose);
}

void ModuleRegistry::projectInitialised() {
  for (std::pair<std::string, IModule*> mModule : cModuleInstances) {
    mModule.second->projectInitialised();
  }
}

ModuleRegistry::~ModuleRegistry() {
//   for (std::map<std::string, IModule*>::iterator j = cModuleInstances.begin(); j != cModuleInstances.end(); j++) {
//     IModule* mInstance = j->second;
//     removeModule(mInstance);
//   }
}


