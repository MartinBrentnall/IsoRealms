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
#include "Engine.h"

Engine::Engine(DOMNodeWrapper* node) {
  cTerminate = false;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ControlLoop") {
      string mControlLoopName = mNode->getAttribute("name");
      string mControlLoopLocation = System::getConfigurationResource("Engine/Default/ControlLoop/" + mControlLoopName + "/ControlLoop");
      void* mControlLoopSO = dlopen(mControlLoopLocation.c_str(), RTLD_LAZY);
      if (!mControlLoopSO) {
        throw InitException("Cannot load library: " + string(dlerror()));
      }
      createControlLoop* createControlLoopFunction = (createControlLoop*) dlsym(mControlLoopSO, "create");
      const char* mDlsymError = dlerror();
      if (mDlsymError) {
        throw InitException("Cannot load symbol: " + string(mDlsymError));
      }
      cControlLoops[mControlLoopName] = createControlLoopFunction(mNode);
      string mControlLoopInit = mNode->getAttribute("init");
      if (mControlLoopInit == "true") {
        cControlLoop.push(cControlLoops[mControlLoopName]);
      }
    } else if (mValueAsString == "GlobalAction") {
      string mCommandType = mNode->getAttribute("type");
      ICommand* mCommand;
      if (mCommandType == "TerminateEngineCommand") {
        mCommand = new TerminateEngineCommand(cTerminate);
      } else if (mCommandType == "PopControlLoopCommand") {
        mCommand = new PopControlLoopCommand(&cControlLoop);
      } else if (mCommandType == "PushControlLoopCommand") {
        IControlLoop* mControlLoop = parseControlLoop(mNode);
        if (mControlLoop == NULL) {
          throw new InitException("PushControlLoopCommand must specified a declared control loop!");
        }
        mCommand = new PushControlLoopCommand(&cControlLoop, *mControlLoop);
      }
      string mCommandName = mNode->getAttribute("name");
      registerEngineCommand(mCommandName, mCommand);
    }
  }
}

void Engine::registerEngineCommand(string name, ICommand* command) {
  GenerateEngineCommand* mGenerateEngineCommand = new GenerateEngineCommand(&cPendingCommands, command);
  CommandManager::addCommand(name, mGenerateEngineCommand);
}

IControlLoop* Engine::parseControlLoop(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ControlLoop") {
      string mControlLoopName = mNode->getStringValue();
      return cControlLoops[mControlLoopName];
    }
  }
  return NULL;
}

void Engine::run() {

  // Flush event buffer before we start
  while (SDL_PollEvent(&cEvent));
  if (cControlLoop.empty()) {
    throw InitException("No initial control loop was specified by the engine configuration");
  }
  int mOldTicks = SDL_GetTicks();
  int mStartTicks = mOldTicks;
  do {
    while (!cPendingCommands.empty()) {
      cPendingCommands.front()->execute();
      cPendingCommands.pop();
    }
    int mNewTicks = SDL_GetTicks();
    int mTicksPassed = mNewTicks - mOldTicks;
    if (mTicksPassed > 0) {
      glLoadIdentity();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      while (SDL_PollEvent(&cEvent)) {
        KeyStates::input(cEvent);
        cControlLoop.top()->input(cEvent);
      }
      cControlLoop.top()->execute(mTicksPassed);
      SDL_GL_SwapBuffers();
    }
 
    mOldTicks = mNewTicks;
  } while (!cTerminate);
  cout << "Engine terminated" << endl;
}

extern "C" IEngine* create(DOMNodeWrapper* node) {
  return new Engine(node);
}

extern "C" void destroy(IEngine* engine) {
  delete engine;
}
