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

DefaultZoneRenderer PluginRegistry::DEFAULT_ZONE_RENDERER;

void PluginRegistry::registerPlugin(DOMNodeWrapper* node) {
  std::string mImplementation = node->getAttribute("implementation");
  std::string mInstance = node->getAttribute("instance");
  std::string mType = node->getAttribute("type");
  std::cout << "Loading plugin \"" << mType << ":" << mImplementation << "\"" << std::endl;
  loadPlugin(mType, mImplementation, mInstance);
}

void PluginRegistry::connectPlugin(DOMNodeWrapper* node) {
  std::string mInstance = node->getAttribute("instance");
  std::string mType = node->getAttribute("type");
  IPlugin* mPlugin = getPlugin(mType, mInstance);
  std::cout << "Setting connections for plugin \"" << mType << ":" << mInstance << "\"" << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "UsePlugin") {
      setPlugin(mPlugin, mNode);
    } else if (mValueAsString == "Configuration") {
      mPlugin->load(mNode);
    }
  }
}

void PluginRegistry::setPlugin(IPlugin* plugin, DOMNodeWrapper* node) {
  std::string mImplementation = node->getAttribute("implementation");
  std::string mInstance = node->getAttribute("instance");
  std::string mSocketID = node->getAttribute("socketid");
  std::cout << "    Connecting plugin \"" << mImplementation << ":" << mInstance << "\"" << std::endl;
  PlugSocket* mPlugSocket = new PlugSocket(mImplementation, mSocketID);
  IPlugin* mPlugin = getPlugin(mImplementation, mInstance);
  plugin->setPlugin(mPlugSocket, mPlugin);
}

void PluginRegistry::loadPlugin(std::string& type, std::string& implementation, std::string& instanceName) {
  // TODO: Test if it exists already!
  std::string mPluginLocation = System::getConfigurationResource("Plugins/" + type + "/" + implementation + "/libModule");
  void* mPluginSO = dlopen(mPluginLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  if (!mPluginSO) {
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
  cPluginInstances[type][instanceName] = createPluginFunction(NULL);
  cDestroyFunctions[cPluginInstances[type][instanceName]] = destroyPluginFunction;
  cImplementationNames[cPluginInstances[type][instanceName]] = implementation;
  cSOHandles[type][implementation] = mPluginSO;
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->pluginInstanceAdded(this, type, instanceName);
  }
}

std::vector<std::string> PluginRegistry::getInstances(std::string& type) {
  std::map<std::string, IPlugin*> mInstances = cPluginInstances[type];
  std::vector<std::string> mInstanceNames;
  for(std::map<std::string, IPlugin*>::iterator i = mInstances.begin(); i != mInstances.end(); i++) {
    mInstanceNames.push_back(i->first);
  }
  return mInstanceNames;
}

IPlugin* PluginRegistry::getPlugin(std::string& type, std::string& instanceName) {
  return cPluginInstances[type][instanceName];
}

void PluginRegistry::addListener(IPluginRegistryListener* listener) {
  cListeners.push_back(listener);
}

void PluginRegistry::removeListener(IPluginRegistryListener* listener) {
  // TODO: URGENT!  Remove the damn thing like it should!
}

std::string PluginRegistry::getInstanceName(std::string type, IPlugin* instance) {
  std::map<std::string, IPlugin*> mPluginsOfType = cPluginInstances[type];
  for (std::map<std::string, IPlugin*>::iterator i = mPluginsOfType.begin(); i != mPluginsOfType.end(); i++) {
    if (i->second == instance) {
      return i->first;
    }
  }
  return "";
}

void PluginRegistry::notifyZoneAction(IZone* zone) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      j->second->notifyZoneAction(zone);
    }
  }
}

void PluginRegistry::initPlugins(IZone* zone, unsigned int pass) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      j->second->initPlugin(zone, pass);
    }
  }
}

void PluginRegistry::renderPreZone(IZone* zone) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      j->second->renderPreZone(zone);
    }
  }
}

void PluginRegistry::zoneContextChanged(IMap* map, IZone* zone) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      j->second->zoneContextChanged(map, zone);
    }
  }
}


void PluginRegistry::save(DOMNodeWriter* node) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::string mTypeName = i->first;
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      IPlugin* mInstance = j->second;
      std::string mInstanceName = j->first;
      std::string mImplementationName = cImplementationNames[mInstance];
      DOMNodeWriter* mPluginBranch = node->addBranch("Plugin");
      mPluginBranch->addAttribute("type", mTypeName);
      mPluginBranch->addAttribute("implementation", mImplementationName);
      mPluginBranch->addAttribute("instance", mInstanceName);
      std::vector<PlugSocket*> mPlugSockets = mInstance->getPlugSockets();
      for (unsigned int k = 0; k < mPlugSockets.size(); k++) {
        IPlugin* mPlugin = mInstance->getClientPlugin(mPlugSockets[k]);
        if (mPlugin != NULL) {
          DOMNodeWriter* mUseBranch = mPluginBranch->addBranch("UsePlugin");
          std::string mType = mPlugSockets[k]->getType();
          std::string mInstanceName = getInstanceName(mType, mPlugin);
          mUseBranch->addAttribute("implementation", mType);
          mUseBranch->addAttribute("instance", mInstanceName);
          if (mPlugSockets[k]->getID() != "") {
            mUseBranch->addAttribute("socketid", mPlugSockets[k]->getID());
          }
        }
      }
      DOMNodeWriter* mPluginConfigurationBranch = mPluginBranch->addBranch("Configuration");
      mInstance->save(mPluginConfigurationBranch);
    }
  }
}

void PluginRegistry::saveData(DOMNodeWriter* node, IMap* map, IZone* zone) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::string mTypeName = i->first;
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      // TODO: Only add branch if there's actually something to write!
      DOMNodeWriter* mDataBranch = node->addBranch("Plugin");
      mDataBranch->addAttribute("type", mTypeName);
      mDataBranch->addAttribute("instance", j->first);
      j->second->saveData(mDataBranch, map, zone);
    }
  }
}

void PluginRegistry::loadPluginData(DOMNodeWrapper* node, IZone* zone) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      std::string mPluginType = mNode->getAttribute("type");
      std::string mPluginInstance = mNode->getAttribute("instance");
      IPlugin* mPlugin = getPlugin(mPluginType, mPluginInstance);
      mPlugin->loadData(mNode, this, zone);
    }
  }  
}

std::string PluginRegistry::getPluginType(IPlugin* instance) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      if (instance == j->second) {
        return i->first;
      }
    }
  }
  // TODO: Throw exception
  std::cout << "Warning: plugin type was not found!" << std::endl;
  return "";
}

IZoneRenderer* PluginRegistry::getZoneRenderer(DOMNodeWrapper* node) {
  std::string mType = node->getAttribute("type");
  if (mType == "") {
    return &DEFAULT_ZONE_RENDERER;
  }
  std::string mInstance = node->getAttribute("instance");
  IPlugin* mPlugin = getPlugin(mType, mInstance);
  std::string mRenderer = node->getAttribute("renderer");
  return mPlugin->getZoneRenderer(mRenderer);
}

void PluginRegistry::removePlugin(IPlugin* instance) {
  std::string mImplementation = cImplementationNames.find(instance)->second;
  destroyPlugin* mDestroyFunction = cDestroyFunctions.find(instance)->second;
  std::string mTypeName = getPluginType(instance);
  pluginRemoved(instance);
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->pluginInstanceRemoved(instance, mTypeName);
  }
  mDestroyFunction(instance);
  void* mHandleToClose = cSOHandles[mTypeName][mImplementation];

  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      if (instance == j->second) {
        mInstanceOfType.erase(j);
        cPluginInstances[i->first] = mInstanceOfType;
        // TODO: Check if it's empty, and remove if so?
      }
    }
  }
  // TODO: Remove from cSOHandles if it was the last one!
  dlclose(mHandleToClose);
}

void PluginRegistry::pluginRemoved(IPlugin* instanceToRemove) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::string mTypeName = i->first;
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      IPlugin* mInstance = j->second;
      std::vector<PlugSocket*> mPlugSockets = mInstance->getPlugSockets();
      for (unsigned int k = 0; k < mPlugSockets.size(); k++) {
        IPlugin* mUsedPlugin = mInstance->getClientPlugin(mPlugSockets[k]);
        if (mUsedPlugin == instanceToRemove) {
          mInstance->setPlugin(mPlugSockets[k], NULL);
        }
      }
    }
  }
}

std::vector<IDynamicElement*> PluginRegistry::getPreLoopCommands() {
  std::vector<IDynamicElement*> mCommands;
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      std::vector<IDynamicElement*> mPluginCommands = j->second->getPreLoopCommands();
      for (unsigned int k = 0; k < mPluginCommands.size(); k++) {
        mCommands.push_back(mPluginCommands[k]);
      }
    }
  }
  return mCommands;
}

std::vector<IInteractiveElement*> PluginRegistry::getInteractiveElements() {
  std::vector<IInteractiveElement*> mAllInteractiveElements;
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      std::vector<IInteractiveElement*> mPluginInteractiveElements = j->second->getInteractiveElements();
      for (unsigned int k = 0; k < mPluginInteractiveElements.size(); k++) {
        mAllInteractiveElements.push_back(mPluginInteractiveElements[k]);
      }
    }
  }
  return mAllInteractiveElements;
}

PluginRegistry::~PluginRegistry() {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::string mTypeName = i->first;
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      IPlugin* mInstance = j->second;
      removePlugin(mInstance);
    }
  }
}


