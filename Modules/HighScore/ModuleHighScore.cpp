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
  cProject          = nullptr;
  cScriptQuit       = nullptr;
  cScriptOnComplete = nullptr;
}

void ModuleHighScore::load(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceRegistry* resources, IModuleOptions* options) {
  if (options != nullptr) {
    std::string mProjectFile = options->getOption("Project");
    std::string mProjectPath = System::getProgramResource(mProjectFile);
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
        resources->add(cProject, "Project");
        std::cout << "Project Started for Scoring: " << mProjectFile << std::endl;
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
      cScriptQuit       = resources->getScriptCall(mNode);
    } else if (mValue == "OnComplete") {
      cScriptOnComplete = resources->getScriptCall(mNode);
    } else if (mValue == "ReadProjectValue") {
      std::string mFrom = mNode->getAttribute("from");
      std::string mTo   = mNode->getAttribute("to");
      std::string mType = mNode->getAttribute("type");
      if      (mType == "Integer") {cReadIntegers[mFrom] = resources->getInteger(mTo);} 
      else if (mType == "String")  {cReadStrings[mFrom]  = resources->getString(mTo);}
      else    {
        std::cout << "Unknown type: " << mType << std::endl;
        exit(1);
      }
    } else if (mValue == "WriteValue") {
      std::string mFieldName  = mNode->getAttribute("name");
      std::string mFieldValue = mNode->getAttribute("value");
      std::string mFieldType  = mNode->getAttribute("type");
      if      (mFieldType == "Integer") {cWriteIntegers[mFieldName] = resources->getInteger(mFieldValue);} 
      else if (mFieldType == "String")  {cWriteStrings[mFieldName]  = resources->getString(mFieldValue);}
      else    {
        std::cout << "Unknown type: " << mFieldType << std::endl;
        exit(1);
      }
    }    
  }
  
  if (cScriptQuit == nullptr) {
    std::cout << "Warning: No 'Quit' script set.  Project won't terminate autamotically" << std::endl;
  }
  if (cScriptOnComplete == nullptr) {
    std::cout << "Warning: No 'OnComplete' script set.  No action will be taken when the project completes" << std::endl;
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

void ModuleHighScore::projectCompleted() {
  
  // Obtain Project values
  std::cout << "================== Game Results:" << std::endl;
  for (std::pair<std::string, IInteger*> mInteger : cReadIntegers) {
    std::string mStringValue = cProject->getReturnValue(mInteger.first);
    int mIntegerValue = std::stoi(mStringValue);
    std::cout << mInteger.first << ": " << mIntegerValue << std::endl;
    mInteger.second->setValue(mIntegerValue);
  }
  for (std::pair<std::string, IString*> mString : cReadStrings) {
    std::string mStringValue = cProject->getReturnValue(mString.first);
    std::cout << mString.first << ": " << mStringValue << std::endl;
    mString.second->setValue(mStringValue);
  }
  cScriptOnComplete->execute();
}

void ModuleHighScore::writeValues() {
  
  // Write Values to XML
  
  quit();
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

