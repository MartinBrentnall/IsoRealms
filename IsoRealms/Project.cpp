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
}

Project::Project(DOMNodeWrapper* node, const std::string& projectName, IEditingContext* editingContext, bool asTemplate, DOMNodeWrapper* options) {
  cResources.setEditing(editingContext != NULL, this);
  cInitScript = NULL;
  if (!asTemplate) {
    cFileName = projectName;
  }
  std::size_t mExtensionPosition = projectName.find_last_of('.');
  std::string mProjectName = projectName.substr(0, mExtensionPosition);

  std::map<std::string, std::string> mDefaultElementGroups;
  
  IArgumentValue* mProjectArgument = new ArgumentValueCustomType<Project>(this);
  std::vector<std::string> mRootPath;
  cResources.add(mProjectArgument, mRootPath, "Project", "Project");
  
  /*
   * First pass only loads module instances; we need to make sure all modules
   * are available before we start connecting them together
   */
  std::cout << "Loading modules and Creating resources..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Module") {
      cModuleRegistry.registerModule(mNode, &cResources, editingContext != NULL ? &cResources : NULL, options);
//     } else if (mValueAsString == "EngineData") {
//       cResources.loadInstances(mNode);
    } else {
      // TODO: Throw something
    }
  }
  cResources.initialise();
  
  std::cout << "Configuring modules (old method)..." << std::endl;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "InputConfiguration") {
      std::string mProjectConfigurationFile = mProjectName + "/controls.config";
      std::string mGlobalConfigurationFile = "controls.config";
      std::vector<std::string> mConfigFiles;
      mConfigFiles.push_back(mProjectConfigurationFile);
      mConfigFiles.push_back(mGlobalConfigurationFile);
      cResources.loadInputConfiguration(mNode, mConfigFiles);
    } else if (mValueAsString == "InitScript") {
      cInitScript = cResources.getScriptCall(mNode, NULL);
    } else if (mValueAsString == "Layer") {
      std::string mLayerTypeName = mNode->getAttribute("type");
      bool mDefaultLayer = mNode->getBooleanAttribute("default");
      ILayerType* mLayerType = cResources.getLayerType(mLayerTypeName);
      ILayer* mLayer = mLayerType->getLayer(mNode, &cResources, editingContext != nullptr, asTemplate);
      // TODO: Do we need to add object selection listeners here!?
      cLayers.push_back(mLayer);
      if (mDefaultLayer) {
        cDefaultLayer = mLayer;
      }
    } else {
      // TODO: Throw something
    }
  }
  cDynamicElements = cResources.getDynamicElements();
  if (cInitScript != NULL) {
    cInitScript->execute();
  }
  cCompleted = false;
  
  std::cout << "Loaded Project!" << std::endl;
}

void Project::renderRuntime() {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->renderRuntime();
  }
}

void Project::updateRuntime(unsigned int ticks) {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->updateRuntime(ticks);
  }
  for (unsigned int i = 0; i < cDynamicElements.size(); i++) {
    cDynamicElements[i]->update(ticks);
  }
}

ILayer* Project::getDefaultLayer() {
  return cDefaultLayer;
}

void Project::finish() {
  cCompleted = true;
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
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->input(event);
  }
}

void Project::initEditor() {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->initEditor();
  }
}

void Project::initRuntime() {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->initRuntime();
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
    for (unsigned int i = 0; i < cLayers.size(); i++) {
      DOMNodeWriter* mLayerNode = mProjectNode->addBranch("Layer");
      cLayers[i]->save(mLayerNode, &cResources);
    }
    mProjectNode->save(cFileName);
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
  for (unsigned int i = 0; i < cLayers.size(); i++) {
//    cLayers[i]->removeElement(element);
  }
}

ModuleRegistry* Project::getModuleRegistry() {
  return &cModuleRegistry;
}

IResourceManager* Project::getResourceManager() {
  return &cResources;
}

void Project::staticChanged() {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->staticChanged();
  }
}

