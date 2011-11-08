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
#include "ElementSetRegistry.h"

ElementSetRegistry::ElementSetRegistry() {
  // Nothing to do.
}

void ElementSetRegistry::registerElementSet(DOMNodeWrapper* node, Registry<IUserCommand, CommandProxy>* commandDirectory, IMap* map, bool editing, IScriptSource* scriptSource) {
  std::string mInstance = node->getAttribute("instance");
  std::string mType = node->getAttribute("type");
  std::cout << "Registering element set \"" << mType << ":" << mInstance << "\"" << std::endl;
  IElementSet* mElementSet = createInstance(mType, mInstance);
  std::vector<std::string> mDirectory;
  mDirectory.push_back("ElementSet");
  mDirectory.push_back(mInstance);
  RegistryProxy<IUserCommand, CommandProxy>* mCommandGateway = new RegistryProxy<IUserCommand, CommandProxy>(commandDirectory, mDirectory);
  RuntimeContext* mRuntimeContext = new RuntimeContext(map, mCommandGateway, editing, scriptSource);
  mElementSet->setRuntimeContext(mRuntimeContext);
  mElementSet->setElementSetRegistry(this);
}

void ElementSetRegistry::connectPlugin(PluginRegistry* pluginRegistry, DOMNodeWrapper* node) {
  std::string mInstance = node->getAttribute("instance");
  IElementSet* mElementSet = getElementSet(&mInstance);
  std::cout << "Setting connections for element set \"" << mInstance << "\"" << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "UsePlugin") {
      setPlugin(pluginRegistry, mElementSet, mNode);
    }
  }
}

void ElementSetRegistry::loadConfiguration(DOMNodeWrapper* node) {
  std::string mInstance = node->getAttribute("instance");
  IElementSet* mElementSet = getElementSet(&mInstance);
  std::cout << "Loading configuration for element set \"" << mInstance << "\"" << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Configuration") {
      mElementSet->load(mNode);
    }
  }
}

void ElementSetRegistry::setPlugin(PluginRegistry* pluginRegistry, IPluginSupport* pluginSupporter, DOMNodeWrapper* node) {
  std::string mImplementation = node->getAttribute("implementation");
  std::string mInstance = node->getAttribute("instance");
  std::string mSocketID = node->getAttribute("socketid");
  PlugSocket* mPlugSocket = new PlugSocket(mImplementation, mSocketID);
  IPlugin* mPlugin = pluginRegistry->getPlugin(mImplementation, mInstance);
  pluginSupporter->setPlugin(mPlugSocket, mPlugin);
}

std::vector<IElement*> ElementSetRegistry::loadElements(DOMNodeWrapper* node, BlockLocation* zoneLocation, IElementContainer* elementContainer) {
  std::vector<IElement*> mElements;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Element") {
      std::string mElementSetName = mNode->getAttribute("set");
      std::string mElementFactoryName = mNode->getAttribute("type");
      IElementSet* mElementSet = getElementSet(&mElementSetName);
      std::vector<IElementFactory*> mElementFactories = mElementSet->getElementFactories();
      for (unsigned int j = 0; j < mElementFactories.size(); j++) {
        if (mElementFactories[j]->getName() == mElementFactoryName) {
          IElement* mElement = mElementFactories[j]->getElement(mNode, zoneLocation, elementContainer);
          mElements.push_back(mElement);
          break;
        }
      }      
    }
  }
  return mElements;
}

IElementSet* ElementSetRegistry::createInstance(std::string implementation, std::string instanceName) {
  if (exists(instanceName)) {
    // TODO: throw exception.
    std::cout << "ElementRegistryThingy exists." << std::endl;
    exit(1);
  }

  std::string mElementLocation = System::getConfigurationResource("Elements/" + implementation + "/libElementSet");
  void* mElementSO = dlopen(mElementLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  if (!mElementSO) {
    throw InitException("Cannot load library: " + std::string(dlerror()));
  }
  createElementSet* createElementSetFunction = cast_voidptr_to_funcptr<createElementSet*>(dlsym(mElementSO, "create"));
  const char* mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  destroyElementSet* destroyElementSetFunction = cast_voidptr_to_funcptr<destroyElementSet*>(dlsym(mElementSO, "destroy"));
  mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  cElementSets[instanceName] = createElementSetFunction(NULL);
  cElementSetTypes[instanceName] = implementation;
  cDestroyFunctions[instanceName] = destroyElementSetFunction;
  cSOHandles[instanceName] = mElementSO;
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->elementSetInstantiated(cElementSets[instanceName]);
  }
  return cElementSets[instanceName];
}

bool ElementSetRegistry::exists(std::string instanceName) {
  // TODO: Test if there is an instance for the instancename.
  return false;
}

void ElementSetRegistry::destroyInstance(IElementSet* elementSet) {
  std::string mInstanceName = getInstanceName(elementSet);
  destroyElementSet* mDestroyFunction = cDestroyFunctions[mInstanceName];
  IElementSet* mElementSet = cElementSets[mInstanceName];
  mDestroyFunction(mElementSet);
  void* mHandleToClose = cSOHandles[mInstanceName];
  cElementSetTypes.erase(mInstanceName);
  cElementSets.erase(mInstanceName);
  cDestroyFunctions.erase(mInstanceName);
  // TODO: Remove from cSOHandles if it was the last one!
  dlclose(mHandleToClose);
  // TODO: Fire event to listeners.
}

void ElementSetRegistry::setEditingInfo(BlockLocation* location, IEditingContext* editingContext, IElementGateway* gateway, IComponentContainer* container) {
  for (std::map<std::string, IElementSet*>::iterator i = cElementSets.begin(); i != cElementSets.end(); i++) {
    i->second->setEditingContext(editingContext);
    std::vector<IElementFactory*> mElementFactories = i->second->getElementFactories();
    for (std::vector<IElementFactory*>::iterator j = mElementFactories.begin(); j != mElementFactories.end(); j++) {
      (*j)->setEditingContext(location, gateway, container);
    }
  }
}

void ElementSetRegistry::addElementRegistryListener(IElementRegistryListener* listener) {
  cListeners.push_back(listener);
}

int ElementSetRegistry::indexOf(IElementRegistryListener* listener) {
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    if (cListeners[i] == listener) {
      return i;
    }
  }
  // TODO: Throw listener not found exception.
  return -1;
}

void ElementSetRegistry::removeElementRegistryListener(IElementRegistryListener* listener) {
  int mIndex = indexOf(listener);
  IElementRegistryListener* mTemp = cListeners[mIndex];
  cListeners[mIndex] = cListeners[cListeners.size() - 1];
  cListeners[cListeners.size() - 1] = mTemp;
  cListeners.pop_back();
}

std::vector<std::string*> ElementSetRegistry::getElementSets() {
  std::vector<std::string*> mElementSetInstanceNames;
  for (std::map<std::string, IElementSet*>::iterator i = cElementSets.begin(); i != cElementSets.end(); i++) {
    std::string* mString = new std::string(i->first);
    mElementSetInstanceNames.push_back(mString);
  }
  return mElementSetInstanceNames;
}

std::string ElementSetRegistry::getInstanceName(IElementSet* elementSet) {
  for (std::map<std::string, IElementSet*>::iterator i = cElementSets.begin(); i != cElementSets.end(); i++) {
    if (i->second == elementSet) {
      return i->first;
    }
  }
  // TODO: Throw something!
  return "";
}

IElementSet* ElementSetRegistry::getElementSet(std::string* instanceName) {
  return cElementSets[*instanceName];
}

void ElementSetRegistry::save(PluginRegistry* pluginRegistry, DOMNodeWriter* node) {
  for (std::map<std::string, IElementSet*>::iterator i = cElementSets.begin(); i != cElementSets.end(); i++) {
    std::string mInstanceName = i->first;
    IElementSet* mElementSet = i->second;
    std::string mTypeName = cElementSetTypes[mInstanceName];
    DOMNodeWriter* mElementSetBranch = node->addBranch("ElementSet");
    mElementSetBranch->addAttribute("type", mTypeName);
    mElementSetBranch->addAttribute("instance", mInstanceName);
    std::vector<PlugSocket*> mPlugSockets = mElementSet->getPlugSockets();
    for (unsigned int j = 0; j < mPlugSockets.size(); j++) {
      IPlugin* mPlugin = mElementSet->getClientPlugin(mPlugSockets[j]);
      if (mPlugin != NULL) {
        DOMNodeWriter* mUseBranch = mElementSetBranch->addBranch("UsePlugin");
        std::string mType = mPlugSockets[j]->getType();
        std::string mPluginInstanceName = pluginRegistry->getInstanceName(mType, mPlugin);
        std::string mSocketID = mPlugSockets[j]->getID();
        mUseBranch->addAttribute("implementation", mType);
        mUseBranch->addAttribute("instance", mPluginInstanceName);
        if (mSocketID != "") {
          mUseBranch->addAttribute("socketid", mSocketID);
        }
      }
    }
    DOMNodeWriter* mConfigurationBranch = mElementSetBranch->addBranch("Configuration");
    mElementSet->save(mConfigurationBranch);
  }
}

void ElementSetRegistry::initElementsComplete() {
  for (std::map<std::string, IElementSet*>::iterator i = cElementSets.begin(); i != cElementSets.end(); i++) {
    i->second->initElementsComplete();
  }
}

void ElementSetRegistry::pluginRemoved(IPlugin* instanceToRemove) {
  for (std::map<std::string, IElementSet*>::iterator i = cElementSets.begin(); i != cElementSets.end(); i++) {
    IElementSet* mElementSet = i->second;
    std::vector<PlugSocket*> mPlugSockets = mElementSet->getPlugSockets();
    for (unsigned int j = 0; j < mPlugSockets.size(); j++) {
      IPlugin* mUsedPlugin = mElementSet->getClientPlugin(mPlugSockets[j]);
      if (mUsedPlugin == instanceToRemove) {
        mElementSet->setPlugin(mPlugSockets[j], NULL);
      }
    }
  }  
}

std::string ElementSetRegistry::getEntityPath(IElementSet* elementSet) {
  return "ElementSet/" + getInstanceName(elementSet) + "/";
}

ElementSetRegistry::~ElementSetRegistry() {
  for (std::map<std::string, IElementSet*>::iterator i = cElementSets.begin(); i != cElementSets.end(); i++) {
    destroyInstance(i->second);
  }
}
