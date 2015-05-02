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
 
void AttractControlLoop::initRuntime() {
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
      cFrontEnd = createFrontEnd(mNode, mFrontEndName);
    } else if (mValueAsString == "AttractScene") {
      std::string mAttractName = mNode->getAttribute("name");
      cAttractServices[mAttractName] = createAttract(mAttractName);
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

IFrontEnd* AttractControlLoop::createFrontEnd(DOMNodeWrapper* node, const std::string& name) {
  return name == "Menu" ? new FrontEnd(node, cFont, this)
                        : NULL;
}

IAttract* AttractControlLoop::createAttract(const std::string& name) {
  return name == "Intro"   ? static_cast<IAttract*>(new AttractIntro(cFont))
       : name == "Title"   ? static_cast<IAttract*>(new AttractTitle(cFont))
       : name == "HiScore" ? static_cast<IAttract*>(new AttractHiScore(cFont))
       : name == "Help"    ? static_cast<IAttract*>(new AttractHelp())
       : name == "Demo"    ? static_cast<IAttract*>(new AttractDemo())
       : name == "FadeOut" ? static_cast<IAttract*>(new AttractFadeOut())
       :                     NULL;
}

void AttractControlLoop::load(DOMNodeWrapper* node, IResourceRegistry* resources) {
  resources->add(this, "Menu", node);
}

void AttractControlLoop::save(DOMNodeWriter* node, IResourceLocator* resources) {
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

ILayer* AttractControlLoop::getLayer(DOMNodeWrapper*, IResourceAccessor*) {
  return this;
}

void AttractControlLoop::resourceSelected(IElementType* elementType) {
  // Not supported
}

ILayerType* AttractControlLoop::getLayerType() {
  return this;
}

void AttractControlLoop::staticChanged() {
  // Nothing to do
}

void AttractControlLoop::renderRuntime() {
  if (cRunningProject != NULL) {
    cRunningProject->renderRuntime();
  } else {
    cAttractSceneManager.render();
    if (cFrontEndActive) {
      cFrontEnd->render();
    }
  }
}

void AttractControlLoop::renderEditing() {
  // Nothing to do
}

void AttractControlLoop::updateRuntime(unsigned int milliseconds) {
  if (cRunningProject != NULL) {
    cRunningProject->updateRuntime(milliseconds);
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

void AttractControlLoop::updateEditing(unsigned int) {
  // Nothing to do
}

void AttractControlLoop::input(SDL_Event& event) {
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

extern "C" IModule* create() {
  return new AttractControlLoop();
}

extern "C" void destroy(IModule* module) {
  delete module;
}
