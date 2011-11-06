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
  
  registerEngineCommand("Pop", new PopControlLoopCommand(this));
  registerEngineCommand("Terminate", new TerminateEngineCommand(cTerminate));
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ControlLoop") {
      std::string mControlLoopInit = mNode->getAttribute("init");
      std::string mControlLoopName = mNode->getAttribute("name");
      if (mControlLoopInit == "true") {
        pushControlLoop(mNode);
      }
      std::string mCommandName = "Push:" + mControlLoopName;
      registerEngineCommand(mCommandName, new PushControlLoopCommand(this, mNode));
    }
  }
}

void Engine::pushControlLoop(DOMNodeWrapper* node) {
  std::string mControlLoopName = node->getAttribute("name");
  std::string mControlLoopLocation = System::getConfigurationResource("Engine/Default/ControlLoop/" + mControlLoopName + "/libControlLoop");
  void* mControlLoopSO = dlopen(mControlLoopLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
  if (!mControlLoopSO) {
    throw InitException("Cannot load library: " + std::string(dlerror()));
  }
  createControlLoop* createControlLoopFunction = cast_voidptr_to_funcptr<createControlLoop*>(dlsym(mControlLoopSO, "create"));
  const char* mDlsymError = dlerror();
  if (mDlsymError) {
    throw InitException("Cannot load symbol: " + std::string(mDlsymError));
  }
  IControlLoop* mControlLoop = createControlLoopFunction(node, &cEngineArguments);
  cControlLoop.push(mControlLoop);
}

void Engine::popControlLoop() {
  if (cControlLoop.size() == 1) {
    cTerminate = true;
  } else {
    cControlLoop.pop();
  }
}

void Engine::registerEngineCommand(const std::string& name, ICommand* command) {
  GenerateEngineCommand* mGenerateEngineCommand = new GenerateEngineCommand(&cPendingCommands, command);
  CommandManager::addCommand(name, mGenerateEngineCommand);
}

void Engine::run() {

  // Flush event buffer before we start
  while (SDL_PollEvent(&cEvent));
  if (cControlLoop.empty()) {
    throw InitException("No initial control loop was specified by the engine configuration");
  }
  int mOldTicks = SDL_GetTicks();
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
  std::cout << "Engine terminated" << std::endl;
}

extern "C" IEngine* create(DOMNodeWrapper* node) {
  return new Engine(node);
}

extern "C" void destroy(IEngine* engine) {
  delete engine;
}
