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

#include "AttractControlLoop.h" 

AttractControlLoop::AttractControlLoop(DOMNodeWrapper* node, IEngineArguments* engineArguments) {
  cFrontEndActive = false;
  int mCurrentLayer = 0;
  assignDummyPlugin(&cFont, "Font");
  cFontSocket.push_back(new PlugSocket("Font"));
  
  // TODO: This three pass code path seems quite common for loading plugins; used here, in the editor and in the map.  Maybe we can reduce duplication.
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      // TODO: Deallocate plugins on destruction of the attract control loop
      cPluginRegistry.registerPlugin(mNode, &cCommandRegistry, &cColourRegistry, NULL, false, NULL, NULL);
    }
  }
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      cPluginRegistry.connectPlugin(mNode);
    }
  }
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      cPluginRegistry.loadConfiguration(mNode);
    } else if (mValueAsString == "UsePlugin") {
      cPluginRegistry.setPlugin(this, mNode);
    } else if (mValueAsString == "FrontEnd") {
      std::string mFrontEndName = mNode->getAttribute("name");
      std::string mFrontEndLocation = System::getConfigurationResource("Engine/Default/ControlLoop/AttractManager/FrontEnd/" + mFrontEndName + "/libFrontEnd");
      void* mFrontEndSO = dlopen(mFrontEndLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
      if (!mFrontEndSO) {
        throw InitException("Cannot load library: " + std::string(dlerror()));
      }
      createFrontEnd* createFrontEndFunction = cast_voidptr_to_funcptr<createFrontEnd*>(dlsym(mFrontEndSO, "create"));
      const char* mDlsymError = dlerror();
      if (mDlsymError) {
        throw InitException("Cannot load symbol: " + std::string(mDlsymError));
      }
      cFrontEnd = createFrontEndFunction(mNode, cFont, engineArguments);
    } else if (mValueAsString == "AttractScene") {
      // TODO: Use relative path
      try {
        std::string mAttractName = mNode->getAttribute("name");
        std::string mAttractLocation = System::getConfigurationResource("Engine/Default/ControlLoop/AttractManager/Attract/" + mAttractName + "/libAttract");
        void* mAttractSO = dlopen(mAttractLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        if (!mAttractSO) {
          throw InitException("Cannot load library: " + std::string(dlerror()));
        }
        createAttract* createAttractFunction = cast_voidptr_to_funcptr<createAttract*>(dlsym(mAttractSO, "create"));
        const char* mDlsymError = dlerror();
        if (mDlsymError) {
          throw InitException("Cannot load symbol: " + std::string(mDlsymError));
        }
        cAttractServices[mAttractName] = createAttractFunction(cFont);
      } catch (InitException e) {
        std::cout << e.getMessage() << std::endl;
        std::cout << "Warning: could not load attract plug-in \"" << mNode->getStringValue() << "\":" << std::endl << e.getMessage() << std::endl;
      }
    } else if (mValueAsString == "Layer") {
      cLayers[mNode->getStringValue()] = mCurrentLayer++;
    } else if (mValueAsString == "Init") {
      // TODO: May be overwriting (if there are two INIT tags), how should we handle this?
      cInitCommands = parseEventCommands(mNode);
    } else if (mValueAsString == "AttractSceneEnd") {
      cSceneEndCommands[cAttractServices[mNode->getAttribute("name")]] = parseEventCommands(mNode);
    } else if (mValueAsString == "FrontEndStart") {
      cFrontEndStartCommands = parseEventCommands(mNode);
    }
  }

  for (unsigned int i = 0; i < cInitCommands.size(); i++) {
    cInitCommands[i]->execute();
  }
}

std::vector<ICommand*> AttractControlLoop::parseEventCommands(DOMNodeWrapper* node) {
  std::vector<ICommand*> mSceneCommands;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Start") {
      IAttract* mAttractScene = cAttractServices[mNode->getStringValue()];
      std::string mLayerName = mNode->getAttribute("layer");
      mSceneCommands.push_back(new StartAttractSceneCommand(cAttractSceneManager, mAttractScene, cLayers[mLayerName]));
    } else if (mValueAsString == "End") {
      IAttract* mAttractScene = cAttractServices[mNode->getStringValue()];
      mSceneCommands.push_back(new TerminateAttractSceneCommand(cAttractSceneManager, mAttractScene));
    }
  }
  return mSceneCommands;
}

bool AttractControlLoop::checkActiveInput(int type) {
  switch (type) {
    case SDL_KEYDOWN         : // Fallthrough
    case SDL_MOUSEBUTTONDOWN : return true; break;
  }
  return false;
}

std::vector<PlugSocket*> AttractControlLoop::getPlugSockets() {
  return cFontSocket;
}

void AttractControlLoop::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "Font") {
    assignPlugin(plugin, &cFont, *socket);
  } else {
    // TODO: Throw
  }
}

IPlugin* AttractControlLoop::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "Font") {return cFont;}
  // TODO: Throw
  return NULL;
}

void AttractControlLoop::input(SDL_Event& event) {
  if (!cFrontEndActive && checkActiveInput(event.type)) {
    cFrontEndActive = true;
    cFrontEnd->setActive(true);
    for (unsigned int i = 0; i < cFrontEndStartCommands.size(); i++) {
      cFrontEndStartCommands[i]->execute();
    }
    
    for (std::map<std::string, IAttract*>::iterator i = cAttractServices.begin(); i != cAttractServices.end(); i++) {
      i->second->frontEndActive(true);
    }          
  } else {
    cFrontEnd->input(event);
  }
}

void AttractControlLoop::execute(int milliseconds) {
  cAttractSceneManager.update(milliseconds);
  cAttractSceneManager.render();
  if (cFrontEndActive) {
    cFrontEnd->update(milliseconds);
    cFrontEnd->render();
    if (cFrontEnd->hasIdled()) {
      cFrontEnd->setActive(false);
      for (unsigned int i = 0; i < cFrontEndEndCommands.size(); i++) {
        cFrontEndEndCommands[i]->execute();
      }
        
      for (std::map<std::string, IAttract*>::iterator i = cAttractServices.begin(); i != cAttractServices.end(); i++) {
        i->second->frontEndActive(false);
      }          
      cFrontEndActive = false;
    }
  }
  std::vector<IAttract*> mCompletedScenes = cAttractSceneManager.getCompletedScenes();
  for (unsigned int i = 0; i < mCompletedScenes.size(); i++) {
    std::vector<ICommand*> mCommandsToExecute = cSceneEndCommands[mCompletedScenes[i]];
    for (unsigned int j = 0; j < mCommandsToExecute.size(); j++) {
      mCommandsToExecute[j]->execute();
    }        
  }
}

extern "C" IControlLoop* create(DOMNodeWrapper* node, IEngineArguments* engineArguments) {
  return new AttractControlLoop(node, engineArguments);
}

extern "C" void destroy(IControlLoop* controlLoop) {
  delete controlLoop;
}
