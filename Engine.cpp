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

Engine::Engine() {
  cRuntime = NULL;
  try {
    std::string mConfigurationFileLocation = System::getConfigurationFileLocation();
    DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper(mConfigurationFileLocation);
    for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
      DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
      std::string mValue = mNode->getNodeName();
      if (mValue == "IsoRealmsConfiguration") {
        loadProject(mNode);
      }
    }
  } catch (ParseException &e) {
    throw InitException("Parsing exception occurred during initalization\n" + e.getMessage());
  }
}

void Engine::loadProject(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper* mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Engine") {
      std::string mRootProject = mNode->getAttribute("project");
      std::string mRootProjectFullPath = System::getProgramResource(mRootProject + ".isorealms");
      DOMNodeWrapper* mProjectNode = new DOMNodeWrapper(mRootProjectFullPath);
      cRuntime = new Runtime(mProjectNode, mRootProject);
    }
  }
}

void Engine::run() {

  // Flush event buffer before we start
  while (SDL_PollEvent(&cEvent));
  if (cRuntime == NULL) {
    throw InitException("No project specified by the engine configuration");
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
      while (SDL_PollEvent(&cEvent)) {
        KeyStates::input(cEvent);
        cRuntime->input(cEvent);
      }
      glLoadIdentity();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      cRuntime->execute(mTicksPassed);
      SDL_GL_SwapBuffers();
    }
 
    mOldTicks = mNewTicks;
  } while (!cRuntime->terminated());
  std::cout << "Engine terminated" << std::endl;
}
