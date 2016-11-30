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
#include "ModuleInputPersistence.h"

ModuleInputPersistence::ModuleInputPersistence(IResourceTypeRegistry* resourceTypeRegistry) : cLayerTypeInputPlayer(this),
                                                                                              cLayerTypeInputRecorder(this) {
  cProject = nullptr;
  cScriptQuit = nullptr;
  cRecordingFile = "";
}

void ModuleInputPersistence::load(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceRegistry* resources, IModuleOptions* options) {
  if (options != nullptr) {
    cRecordingFile = options->getOption("Recording");
    bool mUserRecording = options->getOption("User") == "true";
    std::string mProjectFile;
    if (cRecordingFile != "") {
      cRecordingFile = mUserRecording ? System::getUserResource(cRecordingFile) : System::getProgramResource(cRecordingFile);
      std::cout << "Got recording: " << cRecordingFile << std::endl;
      std::ifstream mRecordingFile(cRecordingFile);
      std::getline(mRecordingFile, mProjectFile);
      mRecordingFile.close();
    } else {
      mProjectFile = options->getOption("Project");
    }
    std::string mProjectPath = System::getProgramResource(mProjectFile);
    std::cout << "Got project file: " << mProjectPath << std::endl;
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
        IProjectOptions* mProjectOptions = options->getProjectOptions("Project");
        cProject = new Project(mNode, mCache, mProjectFile, nullptr, false, mProjectOptions);
        cProject->initRuntime();
        resources->add(cProject, "Project");
        std::cout << "Project Started for Recording/Playback" << std::endl;
      }
    }
  }
  
  if (cProject == nullptr) {
    std::cout << "No project specified for input persistence module" << std::endl;
    exit(1);
  }
  
  resources->add(&cFilename,               "Filename");
  resources->add(&cLayerTypeInputPlayer,   "Player",   node);
  resources->add(&cLayerTypeInputRecorder, "Recorder", node);
  resources->add(this, node);
}

void ModuleInputPersistence::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Quit") {
      cScriptQuit = resources->getScriptCall(mNode);
    } 
  }
  if (cScriptQuit == nullptr) {
    std::cout << "Warning: No 'Quit' script set.  Project cannot be terminated" << std::endl;
  }
}

void ModuleInputPersistence::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resourceLocator) {
  // Not supported
}

void ModuleInputPersistence::projectInitialised() {
  // Nothing to do
}

Project* ModuleInputPersistence::getProject() {
  return cProject;
}

std::ifstream* ModuleInputPersistence::getRecording() {
  std::ifstream* mRecording = new std::ifstream(cRecordingFile);
  return mRecording;
}

void ModuleInputPersistence::setFilename(const std::string& filename) {
  cFilename.setValue(filename);
}

void ModuleInputPersistence::quit() {
  cScriptQuit->execute();
}

void ModuleInputPersistence::Filename::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  // Nothing to do.
}

void ModuleInputPersistence::Filename::setValue(const std::string& value) {
  cFilename = value;
}

std::string ModuleInputPersistence::Filename::getValue() {
  return cFilename;
}

extern "C" IModule* create(IResourceTypeRegistry* resourceTypeRegistry) {
  return new ModuleInputPersistence(resourceTypeRegistry);
}

extern "C" void destroy(IModule* module) {
  delete module;
}
