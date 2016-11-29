/*
 * Copyright 2016 Martin Brentnall
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
#include "ModuleHighScore.h"

ModuleHighScore::ModuleHighScore(IResourceTypeRegistry* resourceTypeRegistry) : cLayerTypeHighScore(this) {
  cProject = nullptr;
  cScriptQuit = nullptr;
}

void ModuleHighScore::load(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceRegistry* resources, IModuleOptions* options) {
  if (options != nullptr) {
    std::string mProjectFile = options->getOption("Project");
    std::string mProjectPath = System::getProgramResource(mProjectFile);
    std::cout << "Got project to score: " << mProjectPath << std::endl;
    std::string mCacheFileName = mProjectFile.substr(0, mProjectFile.length() - 10) + "/project.cache";
    DOMNodeWrapper* mCache = nullptr;
    if (System::fileExists(mCacheFileName)) {
      mCache = new DOMNodeWrapper(mCacheFileName);
    }

    DOMNodeWrapper* mProjectNode = new DOMNodeWrapper(mProjectFile);
    for (int i = 0; i < mProjectNode->getChildCount(); i++) {
      DOMNodeWrapper *mNode = mProjectNode->getChild(i);
      std::string mValue = mNode->getNodeName();
      if (mValue == "Project") {
        IProjectOptions* mProjectOptions = options->getProjectOptions("ProjectOptions");
        cProject = new Project(mNode, mCache, mProjectFile, nullptr, false, mProjectOptions);
        cProject->initRuntime();
        std::cout << "Project Started for Scoring" << std::endl;
      }
    }
  }

  if (cProject == nullptr) {
    std::cout << "No project specified for high score module" << std::endl;
    exit(1);
  }
  
  resources->add(&cLayerTypeHighScore, "HighScore", node);
  resources->add(this, node);
}

void ModuleHighScore::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Quit") {
      cScriptQuit = resources->getScriptCall(mNode);
    }
  }
  if (cScriptQuit == nullptr) {
    std::cout << "Warning: No 'Quit' script set.  Project won't terminate autamotically" << std::endl;
  }
}

void ModuleHighScore::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  // Not supported
}

void ModuleHighScore::projectInitialised() {
  // Nothing to do
}

Project* ModuleHighScore::getProject() {
  return cProject;
}

void ModuleHighScore::quit() {
  cScriptQuit->execute();
}

extern "C" IModule* create(IResourceTypeRegistry* resourceTypeRegistry) {
  return new ModuleHighScore(resourceTypeRegistry);
}

extern "C" void destroy(IModule* module) {
  delete module;
}

