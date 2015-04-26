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
#include "SimpleEditor.h"

const std::string SimpleEditor::COMMAND_SAVE_AS          = "SaveAs";
const std::string SimpleEditor::COMMAND_SAVE             = "Save";
const std::string SimpleEditor::COMMAND_EXIT             = "Exit";
const std::string SimpleEditor::COMMAND_OPEN             = "Open";
const std::string SimpleEditor::COMMAND_MODULES          = "Modules";
const std::string SimpleEditor::COMMAND_RESOURCE_BROWSER = "ResourceBrowser";

void SimpleEditor::load(DOMNodeWrapper* node, IResourceRegistry* runtimeContext) {  
  ICommand* mCommand = CommandManager::getCommand("Pop");
  cExitCommands.push_back(mCommand);
  cSelectedLayer = NULL;
  
  cProject = NULL;
  cRunExitCommands = false;
  cConfirmExitCommands = false;
  cEditorFocus = true;

  // Register commands
  cEditorCommands[COMMAND_EXIT]    = new TerminateEditorCommand(cConfirmExitCommands);
  cEditorCommands[COMMAND_SAVE_AS] = new SaveAsCommand(this, true);
  cEditorCommands[COMMAND_SAVE]    = new SaveAsCommand(this, false);
  cEditorCommands[COMMAND_OPEN]    = new CommandDialog<DialogProjectOpen>(this, this);
  cEditorCommands[COMMAND_MODULES] = new CommandDialog<DialogModules>(this, this);
//  EditorCommandManager::registerCommand(COMMAND_MODULES,          new CommandModules());
//  EditorCommandManager::registerCommand(COMMAND_RESOURCE_BROWSER, new CommandResourceBrowser());

  IProject* mProject;
//  if (mMapName == "") {
    mProject = new Project();
//     mProject->addZone(mInitZone);
//   } else {
//     std::string mProjectFile = System::getProgramResource("Data/Projects/" + mMapName); // TODO: Should we hard code Data/Projects/ here?
//     DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper(mProjectFile);
//     for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
//       DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
//       std::string mValue = mNode->getNodeName();
//       if (mValue == "Project") {
//         mProject = new Project(mNode, this, mMapName, true);
//       }
//     }
//   }  
  cProject = mProject;

  // Setup map for editing
  runtimeContext->add(this, "Editor", node);
}

void SimpleEditor::save(DOMNodeWriter* node, IResourceLocator* resources) {
  // TODO
}

void SimpleEditor::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  IResourceManager* mProjectResources = cProject->getResourceManager();
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Font") {
      std::string mFontName = mNode->getAttribute("name");
      cFont = resources->getFont(mFontName);
      LookAndFeel::setDefaultFont(cFont, 0.02f);
    } else if (mValueAsString == "MenuBar") {
      cMenuBar = new MenuBar(this, mNode, this);
      addComponent(cMenuBar);
    } else if (mValueAsString == "ResourcesIcon") {
      std::string mIconName = mNode->getAttribute("icon");
      std::string mIconModel = mNode->getAttribute("iconModel");
      I3DModelType* mModelType = resources->getModelType(mIconModel);
      Vertex* mVertex = new Vertex();
      I3DModel* mModelInstance = mModelType->createModel(mVertex, 1.0f);
      std::cout << "=================================================> " << mIconName << std::endl;
      cResourceIcons[mIconName] = new GUIIcon(mModelInstance);
    }
  }
  cDockableTextureManager     = new DialogTextureManager(    this, resources, mProjectResources, this, mProjectResources, this);
  cDockableElementTypeManager = new DialogElementTypeManager(this, resources, mProjectResources, this, mProjectResources, this);
  cDockableSoundManager       = new DialogSoundManager(      this, resources, mProjectResources, this, mProjectResources, this);
  cDockableFontManager        = new DialogFontManager(       this, resources, mProjectResources, this, mProjectResources, this);
  cDockableScriptManager      = new DialogScriptManager(     this, resources, mProjectResources, this, mProjectResources, this);
  cDockablePrimitiveManager   = new DialogPrimitiveManager(  this, resources, mProjectResources, this, mProjectResources, this);
  cDockableCustomTypeManager  = new DialogCustomTypeManager( this, resources, mProjectResources, this, mProjectResources, this);
  cDockableVertexManager      = new DialogVertexManager(     this, resources, mProjectResources, this, mProjectResources, this);
  cDockable3DModelManager     = new Dialog3DModelManager(    this, resources, mProjectResources, this, mProjectResources, this);
  cDockableCameraManager      = new DialogCameraManager(     this, resources, mProjectResources, this, mProjectResources, this);
  cDockableBoundariesManager  = new DialogBoundariesManager( this, resources, mProjectResources, this, mProjectResources, this);

  cScreenEdgeManager.add(cDockableTextureManager,     cResourceIcons["IconTextures"]);
  cScreenEdgeManager.add(cDockableElementTypeManager, cResourceIcons["IconElementTypes"]);
  cScreenEdgeManager.add(cDockableSoundManager,       cResourceIcons["IconSounds"]);
  cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconFonts"]);
  cScreenEdgeManager.add(cDockableScriptManager,      cResourceIcons["IconScripts"]);
  cScreenEdgeManager.add(cDockablePrimitiveManager,   cResourceIcons["IconPrimitives"]);
  cScreenEdgeManager.add(cDockableCustomTypeManager,  cResourceIcons["IconCustomTypes"]);
  cScreenEdgeManager.add(cDockableVertexManager,      cResourceIcons["IconVertices"]);
  cScreenEdgeManager.add(cDockable3DModelManager,     cResourceIcons["Icon3DModels"]);
  cScreenEdgeManager.add(cDockableCameraManager,      cResourceIcons["IconCameras"]);
  cScreenEdgeManager.add(cDockableBoundariesManager,  cResourceIcons["IconCollectables"]);
  addComponent(&cScreenEdgeManager);
}

bool SimpleEditor::componentAt(float x, float y) {
  for (int i = cHUDComponents.size() - 1; i >= 0; i--) {
    if (cHUDComponents[i]->contains(x, y)) {
      return true;
    }
  }
  return false;
}

bool SimpleEditor::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_u: {
//       IElement* mElement = cCursor->popElement();
//       if (mElement != NULL) {
//         cUndoStack.push(mElement);
//       }
      return true;
    }

    case SDLK_r: {
      if (!cUndoStack.empty()) {
//        IElement* mElement = cUndoStack.top();
        cUndoStack.pop();
//        cCursor->pushElement(mElement);
      }
      return true;
    }

    default: {
      return false;
    }
  }
}

bool SimpleEditor::editorInput(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      Configuration* mConfiguration = Configuration::getInstance();
      ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
      float mX = mScreen->getXLocation(event.button.x);
      float mY = mScreen->getYLocation(event.button.y);
      if (componentAt(mX, mY)) {
        return false;
      }
      break;
    }

    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  
  if (cSelectedLayer != NULL) {
    cSelectedLayer->input(event);
  }
  return false;
}

void SimpleEditor::testFocusChange(SDL_Event& event) {
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      Configuration* mConfiguration = Configuration::getInstance();
      ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
      float mX = mScreen->getXLocation(event.button.x);
      float mY = mScreen->getYLocation(event.button.y);
      for (int i = cHUDComponents.size() - 1; i >= 0; i--) {
        if (cHUDComponents[i]->contains(mX, mY)) {
          cEditorFocus = false;
          bringComponentToFront(cHUDComponents[i]);
          return;
        }
      }
      cEditorFocus = true;
      break;
    }
  }
}

void SimpleEditor::input(SDL_Event& event) {
  testFocusChange(event);
  bool mEventConsumed = false;
  if (cConfirmExitCommands) {
    switch (event.type) {
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
          case SDLK_n: {
            cConfirmExitCommands = false;
            break;
          }

          case SDLK_ESCAPE: {
            cConfirmExitCommands = false;
            break;
          }

          case SDLK_y: {
            cRunExitCommands = true;
            cConfirmExitCommands = false;
            break;
          }

          default: {
            // Nothing to do
          }
        }
      }

      case SDL_QUIT: {
        cConfirmExitCommands = true;
      }
    }
  } else if (cEditorFocus) {
    mEventConsumed = editorInput(event);
  } else {
    mEventConsumed = cHUDComponents[cHUDComponents.size() - 1]->input(event);
  }

  // Passive input (e.g. mouse overs, etc.)
  if (!mEventConsumed) {
    for (int i = cHUDComponents.size() - 1; i >= 0; i--) {
      if (cHUDComponents[i]->input(event)) {
        return;
      }
    }
  }
}

// IElementContainer* SimpleEditor::pushElement(IElement* element) {
//   clearUndoStack();
//   return cCursor->pushElement(element);
// }
// 
// IElementContainer* SimpleEditor::pushMapElement(IElement* element) {
//   return cCursor->pushMapElement(element);
//   // TODO: Need a map stack!
// }
// 
// void SimpleEditor::removeElement(IElement* element) {
//   cProject->removeElement(element);
// }
// 
void SimpleEditor::elementSelected(IElementType* elementType) {
//  cCursor->setElementType(elementType);
}

int SimpleEditor::getComponentIndex(IHUDComponent* component) {
  for (unsigned int i = 0; i < cHUDComponents.size(); i++) {
    if (component == cHUDComponents[i]) {
      return i;
    }
  }
  return -1;
}

void SimpleEditor::addComponent(IHUDComponent* component) {
  cHUDComponents.push_back(component);
}

void SimpleEditor::removeComponent(IHUDComponent* component) {
  bringComponentToFront(component);
  cHUDComponents.pop_back();
}

void SimpleEditor::bringComponentToFront(IHUDComponent* component) {
  int mComponentIndex = getComponentIndex(component);
  if (mComponentIndex >= 0) {
    for (unsigned int i = mComponentIndex; i < cHUDComponents.size() - 1; i++) {
      IHUDComponent* mTemp = cHUDComponents[i];
      cHUDComponents[i] = cHUDComponents[i + 1];
      cHUDComponents[i + 1] = mTemp;
    }
  }  
}

void SimpleEditor::relinquishFocus(IHUDComponent*) {
  cEditorFocus = true;
}

void SimpleEditor::clearUndoStack() {
  while (!cUndoStack.empty()) {
    IElement* mElement = cUndoStack.top();
    IElementType* mElementType = mElement->getElementType();
    mElementType->destroy(mElement);
    cUndoStack.pop();
  }
}

void SimpleEditor::saveCurrentMap() {
  cProject->save();
}
 
void SimpleEditor::registerCommand(ICommandInfo* commandInfo) {
  cMenuBar->addCommand(commandInfo);
}

IComponentContainer* SimpleEditor::getComponentContainer() {
  return this;
}

void SimpleEditor::staticChanged() {
  cProject->staticChanged();
}

ILayer* SimpleEditor::getLayer(DOMNodeWrapper*, IResourceAccessor*) {
  return this;
}

void SimpleEditor::destroy(ILayer* layer) {
  // Editor will not self terminate
}

void SimpleEditor::renderRuntime() {
  if (cSelectedLayer != NULL) {
    cSelectedLayer->renderEditing();
  }
  
  // Render UI components
  for (unsigned int i = 0; i < cHUDComponents.size(); i++) {
    cHUDComponents[i]->render();
  }
  if (cConfirmExitCommands) {
    glColor3f(0.0f, 0.0f, 0.0f);
    cFont->print(0.004f, -0.004f, 0.03f, 1, "Are you sure you want to quit?");
    glColor3f(1.0f, 1.0f, 1.0f);
    cFont->print(0.0f, 0.0f, 0.03f, 1, "Are you sure you want to quit?");
  }
}

void SimpleEditor::renderEditing() {
  // Nothing to do
}

void SimpleEditor::updateRuntime(unsigned int milliseconds) {
  for (unsigned int i = 0; i < cHUDComponents.size(); i++) {
    cHUDComponents[i]->update(milliseconds);
  }
  
  if (cSelectedLayer != NULL) {
    cSelectedLayer->updateEditing(milliseconds);
  }
  if (cRunExitCommands) {
    for (unsigned int i = 0; i < cExitCommands.size(); i++) {
      cExitCommands[i]->execute();
    }
    cRunExitCommands = false;
  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void SimpleEditor::updateEditing(unsigned int milliseconds) {
  // Nothing to do
}

ILayerType* SimpleEditor::getLayerType() {
  return this;
}

void SimpleEditor::initRuntime() {
}

void SimpleEditor::addProjectListener(IProjectManagerListener* listener) {
  cProjectManagerListeners.push_back(listener);
}

ICommand* SimpleEditor::getCommand(const std::string& name) {
  return cEditorCommands[name];
}

IResourceSelector* SimpleEditor::getResourceSelector() {
  return this;
}

void SimpleEditor::addResourceSelectionListener(IResourceSelectionListener<IColour>* listener) {
  cDockableTextureManager->addColourSelectionListener(listener);
}

void SimpleEditor::addResourceSelectionListener(IResourceSelectionListener<ITexture>* listener) {
  cDockableTextureManager->addTextureSelectionListener(listener);
}

void SimpleEditor::removeResourceSelectionListener(IResourceSelectionListener<IColour>* listener) {
  cDockableTextureManager->removeColourSelectionListener(listener);
}

void SimpleEditor::removeResourceSelectionListener(IResourceSelectionListener<ITexture>* listener) {
  cDockableTextureManager->removeTextureSelectionListener(listener);
}

void SimpleEditor::notifyResourceReleased(IColour* colour) {
  cDockableTextureManager->notifyResourceReleased(colour);
}

void SimpleEditor::notifyResourceOwned(IColour* colour) {
  cDockableTextureManager->notifyResourceOwned(colour);
}

void SimpleEditor::openProject(const std::string& file) {
  DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper(file);
  for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
    DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Project") {
      if (cProject != NULL) {
        clearUndoStack();
        delete cProject;
      }
      cProject = new Project(mNode, file, this);
      cSelectedLayer = cProject->getDefaultLayer();
      for (unsigned int i = 0; i < cProjectManagerListeners.size(); i++) {
        cProjectManagerListeners[i]->projectOpened(cProject);
      }
      return;
    }
  }
}

extern "C" IModule* create() {
  return new SimpleEditor();
}

extern "C" void destroy(IModule* module) {
  delete module;
}
