/*
 * Copyright 2015 Martin Brentnall
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
#include "Project.h"

Project::Project() {
  cResources.setEditing(true, this);
  cCompleted = false;
  cFirstInitialised = false;
}

Project::Project(const std::string& file, bool user, IEditingContext* editingContext, bool asTemplate, IProjectOptions* options) {
  std::size_t mExtensionPosition = file.find_last_of('.');
  cProjectDataPath = System::getUserResource((user ? "User/" : "Program/") + file.substr(0, mExtensionPosition));
  if (!asTemplate && user) {
    cFileName = file;
  }
  cInitScript = nullptr;
  cResources.setEditing(editingContext != nullptr, this);
  std::string mFullPath = (user ? System::getUserResource(file) : System::getProgramResource(file));
  DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper(mFullPath);
  std::string mFullPathCache = cProjectDataPath + "/project.cache";
  DOMNodeWrapper* mCache = System::fileExists(mFullPathCache) ? new DOMNodeWrapper(mFullPathCache) : nullptr;
  for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
    DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Project") {
      loadProject(mNode, mCache, editingContext, asTemplate, options);
      
      // Expsoe the project filename and data path in a string
      std::string mProjectNamePath = mNode->getAttribute("filename");
      if (mProjectNamePath != "") {
        IString* mProjectNameString = cResources.getString(mProjectNamePath);
        mProjectNameString->setValue(file);
      }
      std::string mProjectDataPath = mNode->getAttribute("datapath");
      if (mProjectDataPath != "") {
        IString* mProjectDataPathString = cResources.getString(mProjectDataPath);
        mProjectDataPathString->setValue(cProjectDataPath);
      }
      break;
    }
  }

  cDynamicElements = cResources.getDynamicElements();
  if (cInitScript != nullptr) {
    cInitScript->execute();
  }
  cCompleted = false;
  cFirstInitialised = false;
  
  std::cout << "Project Ready." << std::endl;
  
/*  
  if (mCache == nullptr) {
    updateCache();
  }*/
}

void Project::updateCache() {
  DOMNodeWriter* mCacheNode = new DOMNodeWriter("ProjectCache");
  cModuleRegistry.saveCache(mCacheNode);
  for (ILayer* mLayer : cLayers) {
    mLayer->saveCache(mCacheNode);
  }
  System::makeDirectory(cProjectDataPath);
  mCacheNode->save(cProjectDataPath + "/project.cache");
}

void Project::loadProject(DOMNodeWrapper* node, DOMNodeWrapper* cache, IEditingContext* editingContext, bool asTemplate, IProjectOptions* options) {
  std::map<std::string, std::string> mDefaultElementGroups;
  
  IArgumentValue* mProjectArgument = new ArgumentValueCustomType<Project>(this);
  std::vector<std::string> mRootPath;
  cResources.add(mProjectArgument, mRootPath, "Project", "Project");
  
  // Get cache node
  DOMNodeWrapper* mCacheNode = nullptr;
  if (cache != nullptr) {
    for (int i = 0; i < cache->getChildCount(); i++) {
      DOMNodeWrapper* mNode = cache->getChild(i);
      std::string mValueAsString = mNode->getNodeName();
      if (mValueAsString == "ProjectCache") {
        // Verify version!
        mCacheNode = mNode;
        break;
      }
    }
  }
  
  /*
   * First pass only loads module instances; we need to make sure all modules
   * are available before we start connecting them together
   */
  std::cout << "Loading Modules and creating Resources..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Module") {
      cModuleRegistry.registerModule(mNode, mCacheNode, &cResources, editingContext != nullptr ? &cResources : nullptr, options);
    } else {
      // TODO: Throw something
    }
  }
  cResources.initialise();
  
  std::cout << "Configuring Project..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "InputConfiguration") {
      std::string mProjectConfigurationFile = cProjectDataPath + "/controls.config";
      std::string mGlobalConfigurationFile = System::getUserResource("controls.config");
      std::vector<std::string> mConfigFiles;
      mConfigFiles.push_back(mProjectConfigurationFile);
      mConfigFiles.push_back(mGlobalConfigurationFile);
      cResources.loadInputConfiguration(mNode, mConfigFiles);
    } else if (mValueAsString == "InitScript") {
      cInitScript = cResources.getScriptCall(mNode, nullptr);
    } else if (mValueAsString == "Layer") {
      std::string mLayerTypeName = mNode->getAttribute("type");
      bool mDefaultLayer = mNode->getBooleanAttribute("default");
      ILayerType* mLayerType = cResources.getLayerType(mLayerTypeName);
      ILayer* mLayer = mLayerType->getLayer(mNode, mCacheNode, &cResources, editingContext != nullptr, asTemplate);
      // TODO: Do we need to add object selection listeners here!?
      cLayers.push_back(mLayer);
      if (mDefaultLayer) {
        cDefaultLayer = mLayer;
      }
    } else if (mValueAsString == "ReturnValue") {
      std::string mName  = mNode->getAttribute("name");
      std::string mValue = mNode->getAttribute("value");
      std::string mType  = mNode->getAttribute("type");
      if (mType == "Project") {
        cReturnProjects[mName] = new ReturnProject(mValue);
      } else {
        cReturnValues[mName] = new ReturnValue(mType, mValue);
      }
    } else {
      // TODO: Throw something
    }
  }
}

void Project::renderRuntime() {
  for (ILayer* mLayer : cLayers) {
    mLayer->renderRuntime();
  }
}

void Project::updateRuntime(unsigned int ticks) {
  for (ILayer* mLayer : cLayers) {
    mLayer->updateRuntime(ticks);
  }
  for (IDynamicElement* mDynamicElement : cDynamicElements) {
    mDynamicElement->update(ticks);
  }
  
  if (!cFirstInitialised) {
    cModuleRegistry.projectInitialised();
    cFirstInitialised = true;
  }
}

void Project::updateEditing(unsigned int milliseconds) {
  for (IDynamicElement* mDynamicElement : cDynamicElements) {
    mDynamicElement->update(milliseconds);
  }
}

void Project::initialised() {
  if (!cFirstInitialised) {
    cModuleRegistry.projectInitialised();
    cFirstInitialised = true;
  }
}

ILayer* Project::getDefaultLayer() {
  return cDefaultLayer;
}

std::vector<ILayer*> Project::getAllLayers() {
  return cLayers;
}

std::string Project::getName(ILayer* layer) {
  return "<Layer Name Goes Here>";
}

void Project::finish() {
  updateCache(); // TODO: Probably should do this when the project loads, not when finished.  We do it here now because we need to initialise the surfaces after loading before doing this
  cCompleted = true;
  for (std::pair<std::string, ReturnValue*> mReturnValue : cReturnValues) {
    if (mReturnValue.second->cType == "String") {
      IString* mString = cResources.getString(mReturnValue.second->cReference);
      mReturnValue.second->cValue = mString->getValue();
    } else if (mReturnValue.second->cType == "Integer") {
      int mValue = cResources.getInteger(mReturnValue.second->cReference)->getValue();
      mReturnValue.second->cValue = std::to_string(mValue);
    } else {
      // TODO: Floats and bools
      std::cout << "Unknown return type \"" << mReturnValue.second->cType << "\"" << std::endl;
      exit(1);
    }
  }
  for (std::pair<std::string, ReturnProject*> mReturnProject : cReturnProjects) {
    mReturnProject.second->cProject = cResources.getProject(mReturnProject.second->cReference);
  }
}

bool Project::hasCompleted() {
  return cCompleted;
}

void Project::reset() {
  cCompleted = false;
  for (ILayer* mLayer : cLayers) {
    mLayer->reset();
  }
  cResources.reset();
}

void Project::addObjectSelectionListener(IObjectSelectionListener* listener) {
  cObjectSelectionListeners.push_back(listener);
  for (ILayer* mLayer : cLayers) {
    mLayer->addObjectSelectionListener(listener);
  }
}

void Project::inputRuntime(SDL_Event& event) {
  cResources.input(event);
  for (ILayer* mLayer : cLayers) {
    mLayer->input(event);
  }
}

void Project::initEditor() {
  for (ILayer* mLayer : cLayers) {
    mLayer->initEditor();
  }
}

void Project::initRuntime() {
  for (ILayer* mLayer : cLayers) {
    mLayer->initRuntime();
  }
}

void Project::save() {
  if (!cFileName.empty()) {
    DOMNodeWriter* mProjectNode = new DOMNodeWriter("Project");
    DOMNodeWriter* mInputConfigurationNode = mProjectNode->addBranch("InputConfiguration");
    cModuleRegistry.save(mProjectNode, &cResources);
    cResources.saveInputConfiguration(mInputConfigurationNode);
    DOMNodeWriter* mInitScriptNode = mProjectNode->addBranch("InitScript");
    cInitScript->save(mInitScriptNode, &cResources);
    for (ILayer* mLayer : cLayers) {
      DOMNodeWriter* mLayerNode = mProjectNode->addBranch("Layer");
      mLayer->save(mLayerNode, &cResources);
    }
    mProjectNode->save(cFileName);
    updateCache();
  }
}

void Project::save(const std::string& fileName) {
  cFileName = fileName;
  save();
}

bool Project::hasFileName() {
  return !cFileName.empty();
}

void Project::removeElement(IElement* element) {
//  for (ILayer* mLayer : cLayers) {
//    mLayer->removeElement(element);
//  }
}

ModuleRegistry* Project::getModuleRegistry() {
  return &cModuleRegistry;
}

IResourceManager* Project::getResourceManager() {
  return &cResources;
}

void Project::staticChanged() {
  for (ILayer* mLayer : cLayers) {
    mLayer->staticChanged();
  }
}

InputCommands* Project::getInputConfiguration() {
  return cResources.getInputConfiguration();
}

std::string Project::getFileName() {
  return cFileName;
}

std::string Project::getReturnValue(const std::string& name) {
  if (!cCompleted) {
    std::cout << "Cannot obtain return value because project hasn't finished" << std::endl;
    return "!ERROR:PROJECT_NOT_COMPLETE";
  }
  
  std::size_t mSplit = name.find('/');
  if (mSplit != std::string::npos) {
    std::string mProject   = name.substr(0, mSplit);
    std::string mValueName = name.substr(mSplit + 1);
    return cReturnProjects[mProject]->cProject->getReturnValue(mValueName);
  }
  std::map<std::string, ReturnValue*>::iterator mReturnValue = cReturnValues.find(name);
  if (mReturnValue == cReturnValues.end()) {
    return "!ERROR:DOES_NOT_EXIST";
  }
  return mReturnValue->second->cValue;
}

Project::ReturnValue::ReturnValue(const std::string& type, const std::string& reference) {
  cType      = type;
  cReference = reference;
}

Project::ReturnProject::ReturnProject(const std::string& reference) {
  cReference = reference;
}
