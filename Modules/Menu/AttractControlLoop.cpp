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

AttractControlLoop::AttractControlLoop() {
}  
  
void AttractControlLoop::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  runtimeContext->add(this, "Menu", node);
}

void AttractControlLoop::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cRunningProject = NULL;
  cFrontEndActive = false;
  int mCurrentLayer = 0;
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Font") {
      std::string mFontPath = mNode->getAttribute("name");
      cFont = resources->getFont(mFontPath);
      std::cout << "GOT FONT: " << cFont << std::endl;
    } else if (mValueAsString == "FrontEnd") {
      std::string mFrontEndName = mNode->getAttribute("name");
      std::string mFrontEndLocation = System::getConfigurationResource("Modules/Menu/FrontEnd/" + mFrontEndName + "/libFrontEnd");
      void* mFrontEndSO = dlopen(mFrontEndLocation.c_str(), RTLD_LAZY | RTLD_GLOBAL);
      if (!mFrontEndSO) {
        throw InitException("Cannot load library: " + std::string(dlerror()));
      }
      createFrontEnd* createFrontEndFunction = cast_voidptr_to_funcptr<createFrontEnd*>(dlsym(mFrontEndSO, "create"));
      const char* mDlsymError = dlerror();
      if (mDlsymError) {
        throw InitException("Cannot load symbol: " + std::string(mDlsymError));
      }
      cFrontEnd = createFrontEndFunction(mNode, cFont, this);
    } else if (mValueAsString == "AttractScene") {
      // TODO: Use relative path
      try {
        std::string mAttractName = mNode->getAttribute("name");
        std::string mAttractLocation = System::getConfigurationResource("Modules/Menu/Attract/" + mAttractName + "/libAttract");
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

bool AttractControlLoop::input(SDL_Event& event) {
  if (cRunningProject != NULL) {
    cRunningProject->input(event);
  } else {
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
  return true;
}

void AttractControlLoop::update(unsigned int milliseconds) {
  if (cRunningProject != NULL) {
    cRunningProject->executePreLoopCommands(milliseconds);
    cRunningProject->updateRuntime(milliseconds);
    cRunningProject->executePostLoopCommands(milliseconds);
  } else {
    cAttractSceneManager.update(milliseconds);
    if (cFrontEndActive) {
      cFrontEnd->update(milliseconds);
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
}

void AttractControlLoop::render() {
  if (cRunningProject != NULL) {
    cRunningProject->executePreLoopRenderers();
    cRunningProject->render();
    cRunningProject->executePostLoopRenderers();
  } else {
    cAttractSceneManager.render();
    if (cFrontEndActive) {
      cFrontEnd->render();
    }
  }
}

IPlugin* AttractControlLoop::getElementSet() {
  return this;
}

IElement* AttractControlLoop::getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*) {
  return this;
}

void AttractControlLoop::setEditingContext(BlockLocation*, IComponentContainer*) {
  // Not editable.  Nothing to do
}

void AttractControlLoop::configureElement() {
  // Not editable.  Nothing to do
}

void AttractControlLoop::renderEditingPreview() {
  // Not editable.  Nothing to do
}

void AttractControlLoop::renderIcon() {
  // Not editable.  Nothing to do
}

void AttractControlLoop::updateIcon(int) {
  // Not editable.  Nothing to do
}

void AttractControlLoop::destroy(IElement* element) {
  // Front-end will not self terminate
}

IElementHandler* AttractControlLoop::getElementHandler() {
  return NULL;
}

IElementType* AttractControlLoop::getElementType() {
  return this;
}

bool AttractControlLoop::initElement(unsigned int) {
  return true;
}

void AttractControlLoop::renderStatic() {
  // Nothing to do
}

void AttractControlLoop::renderStaticEditing() {
  // Nothing to do
}

std::vector<IVisualElement*> AttractControlLoop::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> AttractControlLoop::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IDynamicElement*> AttractControlLoop::getDynamicElementsRuntime() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IInteractiveElement*> AttractControlLoop::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  mInteractiveElements.push_back(this);
  return mInteractiveElements;
}

void AttractControlLoop::save(DOMNodeWriter*, IResourceLocator*, BlockLocation&) {
  // Not editable.  Nothing to do
}

void AttractControlLoop::setDirty() {
  // Nothing to do
}

void AttractControlLoop::startProject(const std::string& project) {
  std::string mProjectPath = System::getProgramResource(project);
  DOMNodeWrapper* mProjectNode = new DOMNodeWrapper(mProjectPath);
  for (int i = 0; i < mProjectNode->getChildCount(); i++) {
    DOMNodeWrapper *mNode = mProjectNode->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Project") {
      cRunningProject = new Project(mNode, project, NULL);
      cRunningProject->initRuntime();
      break;
    }
  }
}

extern "C" IPlugin* create() {
  return new AttractControlLoop();
}

extern "C" void destroy(IPlugin* module) {
  delete module;
}
