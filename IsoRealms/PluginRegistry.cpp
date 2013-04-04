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
#include "PluginRegistry.h"

DefaultZoneHandler PluginRegistry::DEFAULT_ZONE_RENDERER;

void PluginRegistry::registerPlugin(DOMNodeWrapper* node, IResources* resources) {
  std::string mType = node->getAttribute("name");
  std::vector<std::string> mDirectory;
  mDirectory.push_back(mType);
  RuntimeContext* mRuntimeContext = new RuntimeContext(resources, mDirectory);
  std::cout << "Loading Module \"" << mType  << "\"" << std::endl;
  loadPlugin(mType, mRuntimeContext);
  IPlugin* mPlugin = getPlugin(mType);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Resources") {
      mPlugin->createResources(mNode, mRuntimeContext);
    }
  }
  mPlugin->setPluginRegistry(this);
}

void PluginRegistry::loadPlugin(std::string& type, IRuntimeContext* runtimeContext) {
  // TODO: Test if it exists already!
  std::string mPluginLocation = System::getConfigurationResource("Modules/" + type + "/libModule");
  void* mPluginSO = dlopen(mPluginLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  if (!mPluginSO) {
    std::cout << dlerror() << std::endl;
    throw InitException("Cannot load library: " + std::string(dlerror()));
  }
  createPlugin* createPluginFunction = cast_voidptr_to_funcptr<createPlugin*>(dlsym(mPluginSO, "create"));
  const char* mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  destroyPlugin* destroyPluginFunction = cast_voidptr_to_funcptr<destroyPlugin*>(dlsym(mPluginSO, "destroy"));
  mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  InitLuaFunction* initLuaFunction = cast_voidptr_to_funcptr<InitLuaFunction*>(dlsym(mPluginSO, "initLua"));
  mDlsymError = dlerror();
  if (!mDlsymError) {
    std::cout << "Lua support found for \"" << type << "\"" << std::endl;
    Configuration* mConfiguration = Configuration::getInstance();
    mConfiguration->enableLuaSupport(initLuaFunction, runtimeContext);
  }
  cPluginInstances[type] = createPluginFunction();
  cDestroyFunctions[cPluginInstances[type]] = destroyPluginFunction;
  cImplementationNames[cPluginInstances[type]] = type;
  cSOHandles[type][type] = mPluginSO;
}

void PluginRegistry::setEditingContext(IEditingContext* editingContext, IResourceManager* resourceManager) {
  for (std::map<std::string, IPlugin*>::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    i->second->setEditingContext(editingContext, resourceManager);
  }
}

std::vector<std::string> PluginRegistry::getInstances() {
  std::vector<std::string> mInstanceNames;
  for(std::map<std::string, IPlugin*>::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    mInstanceNames.push_back(i->first);
  }
  return mInstanceNames;
}

IPlugin* PluginRegistry::getPlugin(std::string& type) {
  return cPluginInstances[type];
}

std::string PluginRegistry::getInstanceName(IPlugin* instance) {
  for (std::map<std::string, IPlugin*>::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    if (i->second == instance) {
      return i->first;
    }
  }
  return "";
}

std::string PluginRegistry::getEntityPath(IPlugin* plugin) {
  std::string mType = getPluginType(plugin);
  return "Plugin/" + mType + "/";
}

void PluginRegistry::initPlugins(IZone* zone, unsigned int pass) {
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    j->second->initPlugin(zone, pass);
  }
}

void PluginRegistry::renderPreZone(IZone* zone) {
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    j->second->renderPreZone(zone);
  }
}

void PluginRegistry::zoneContextChanged(IMap* map, IZone* zone) {
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    j->second->zoneContextChanged(map, zone);
  }
}


void PluginRegistry::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    IPlugin* mInstance = j->second;
    std::string mTypeName = j->first;
    DOMNodeWriter* mPluginBranch = node->addBranch("Plugin");
    mPluginBranch->addAttribute("name", mTypeName);
    DOMNodeWriter* mPluginConfigurationBranch = mPluginBranch->addBranch("Resources");
    mInstance->save(mPluginConfigurationBranch, resourceLocator);
  }
}

void PluginRegistry::saveData(DOMNodeWriter* node, IMap* map, IZone* zone) {
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    // TODO: Only add branch if there's actually something to write!
    std::string mTypeName = j->first;
    DOMNodeWriter* mDataBranch = node->createBranch("Plugin");
    j->second->saveData(mDataBranch, map, zone);
    if (!mDataBranch->empty()) {
      mDataBranch->addAttribute("type", mTypeName);
      node->addBranch(mDataBranch);
    }
  }
}

void PluginRegistry::loadPluginData(DOMNodeWrapper* node, IZone* zone) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      std::string mPluginType = mNode->getAttribute("type");
      IPlugin* mPlugin = getPlugin(mPluginType);
      mPlugin->loadData(mNode, this, zone);
    }
  }
  for (std::map<std::string, IPlugin*>::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    i->second->initZone(zone);
  }
}

std::string PluginRegistry::getPluginType(IPlugin* instance) {
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    if (instance == j->second) {
      return j->first;
    }
  }
  // TODO: Throw exception
  std::cout << "Warning: plugin type was not found!" << std::endl;
  return "";
}

void PluginRegistry::removePlugin(IPlugin* instance) {
  std::string mImplementation = cImplementationNames.find(instance)->second;
  destroyPlugin* mDestroyFunction = cDestroyFunctions.find(instance)->second;
  std::string mTypeName = getPluginType(instance);
  mDestroyFunction(instance);
  void* mHandleToClose = cSOHandles[mTypeName][mImplementation];

  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    if (instance == j->second) {
      cPluginInstances.erase(j);
    }
  }
  // TODO: Remove from cSOHandles if it was the last one!
  dlclose(mHandleToClose);
}

std::vector<IDynamicElement*> PluginRegistry::getPostLoopCommands() {
  std::vector<IDynamicElement*> mCommands;
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    std::vector<IDynamicElement*> mPluginCommands = j->second->getPostLoopCommands();
    for (unsigned int k = 0; k < mPluginCommands.size(); k++) {
      mCommands.push_back(mPluginCommands[k]);
    }
  }
  return mCommands;
}

std::vector<IVisualElement*> PluginRegistry::getPostLoopRenderers() {
  std::vector<IVisualElement*> mRenderers;
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    std::vector<IVisualElement*> mPluginRenderers = j->second->getPostLoopRenderers();
    for (unsigned int k = 0; k < mPluginRenderers.size(); k++) {
      mRenderers.push_back(mPluginRenderers[k]);
    }
  }
  return mRenderers;
}

std::vector<IInteractiveElement*> PluginRegistry::getInteractiveElements() {
  std::vector<IInteractiveElement*> mAllInteractiveElements;
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    std::vector<IInteractiveElement*> mPluginInteractiveElements = j->second->getInteractiveElements();
    for (unsigned int k = 0; k < mPluginInteractiveElements.size(); k++) {
      mAllInteractiveElements.push_back(mPluginInteractiveElements[k]);
    }
  }
  return mAllInteractiveElements;
}

PluginRegistry::~PluginRegistry() {
  for (std::map<std::string, IPlugin*>::iterator j = cPluginInstances.begin(); j != cPluginInstances.end(); j++) {
    IPlugin* mInstance = j->second;
    removePlugin(mInstance);
  }
}


