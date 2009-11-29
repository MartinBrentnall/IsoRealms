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

std::map<std::string, IPlugin*> PluginRegistry::cDummyPlugins;  

void PluginRegistry::registerPlugin(DOMNodeWrapper* node) {
  std::string mImplementation = node->getAttribute("implementation");
  std::string mInstance = node->getAttribute("instance");
  std::string mType = node->getAttribute("type");
  loadPlugin(mType, mImplementation, mInstance);
  IPlugin* mPlugin = getPlugin(mType, mInstance);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "UsePlugin") {
      // TODO: WE CANNOT ASSUME THE PLUGIN EXISTS IMMEDIATELY!
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
  PlugSocket* mPlugSocket = new PlugSocket(mImplementation, mSocketID);
  IPlugin* mPlugin = getPlugin(mImplementation, mInstance);
  plugin->setPlugin(mPlugSocket, mPlugin);
}

void PluginRegistry::loadPlugin(std::string& type, std::string& implementation, std::string& instanceName) {
  // TODO: Test if it exists already!
  std::string mPluginLocation = System::getConfigurationResource("Plugins/" + type + "/" + implementation + "/module");
  void* mPluginSO = dlopen(mPluginLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  if (!mPluginSO) {
    throw InitException("Cannot load library: " + std::string(dlerror()));
  }
  createPlugin* createPluginFunction = cast_voidptr_to_funcptr<createPlugin*>(dlsym(mPluginSO, "create"));
  const char* mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  cPluginInstances[type][instanceName] = createPluginFunction(NULL);
  cImplementationNames[cPluginInstances[type][instanceName]] = implementation;
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->pluginInstanceAdded(this, type, instanceName);
  }
  std::cout << "Loaded Plugin \"" << type << "\" successfully!" << std::endl;
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

bool PluginRegistry::isDummyPlugin(IPlugin* instance) {
  for (std::map<std::string, IPlugin*>::iterator i = cDummyPlugins.begin(); i != cDummyPlugins.end(); i++) {
    if (instance == i->second) {
      return true;
    }
  }
  return false;
}

IPlugin* PluginRegistry::getDummyPlugin(std::string& type) {
  IPlugin* mDummyPlugin = cDummyPlugins[type];
  if (mDummyPlugin == NULL) {
    std::string mPluginLocation = System::getConfigurationResource("Plugins/" + type + "/dummy");
    void* mPluginSO = dlopen(mPluginLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (!mPluginSO) {
      throw InitException("Cannot load library: " + std::string(dlerror()));
    }
    createPlugin* createPluginFunction = cast_voidptr_to_funcptr<createPlugin*>(dlsym(mPluginSO, "create"));
    const char* mDlsymError = dlerror();
    if (mDlsymError) {
      throw InitException("Cannot load symbol: " + std::string(mDlsymError));
    }
    mDummyPlugin = createPluginFunction(NULL);
    cDummyPlugins[type] = mDummyPlugin;
  }
  return mDummyPlugin;
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

void PluginRegistry::notifyZoneAction(Zone* zone) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      j->second->notifyZoneAction(zone);
    }
  }
}

void PluginRegistry::initPlugins(Zone* zone) {
  for (std::map<std::string, std::map<std::string, IPlugin*> >::iterator i = cPluginInstances.begin(); i != cPluginInstances.end(); i++) {
    std::map<std::string, IPlugin*> mInstanceOfType = i->second;
    for (std::map<std::string, IPlugin*>::iterator j = mInstanceOfType.begin(); j != mInstanceOfType.end(); j++) {
      j->second->initPlugin(zone);
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
        IPlugin* mPlugin = mInstance->getPlugin(mPlugSockets[k]);
        if (mPlugin != NULL) {
          DOMNodeWriter* mUseBranch = mPluginBranch->addBranch("UsePlugin");
          std::string mType = mPlugSockets[k]->getType();
          std::string mInstanceName = getInstanceName(mType, mPlugin);
          mUseBranch->addAttribute("implementation", mType);
          mUseBranch->addAttribute("instance", mInstanceName);
          mUseBranch->addAttribute("socketid", mPlugSockets[k]->getID());
        }
      }
      DOMNodeWriter* mPluginConfigurationBranch = mPluginBranch->addBranch("Configuration");
      mInstance->save(mPluginConfigurationBranch);
    }
  }
}

