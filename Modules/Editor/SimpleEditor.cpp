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
const std::string SimpleEditor::COMMAND_OPEN             = "Open";
const std::string SimpleEditor::COMMAND_OPEN_TEMPLATE    = "OpenTemplate";
const std::string SimpleEditor::COMMAND_TEST             = "Test";
const std::string SimpleEditor::COMMAND_MODULES          = "Modules";
const std::string SimpleEditor::COMMAND_RESOURCE_BROWSER = "ResourceBrowser";

const float SimpleEditor::DOCKABLE_3D_MODEL_DEFAULT_WIDTH          = 0.285f;
const float SimpleEditor::DOCKABLE_BOUNDARIES_DEFAULT_WIDTH        = 0.35f;
const float SimpleEditor::DOCKABLE_CAMERA_DEFAULT_WIDTH            = 0.35f;
const float SimpleEditor::DOCKABLE_ELEMENT_TYPE_DEFAULT_WIDTH      = 0.35f;
const float SimpleEditor::DOCKABLE_FONT_DEFAULT_WIDTH              = 0.4f;
const float SimpleEditor::DOCKABLE_PRIMITIVE_DEFAULT_WIDTH         = 0.5f;
const float SimpleEditor::DOCKABLE_SCRIPT_DEFAULT_WIDTH            = 0.5f;
const float SimpleEditor::DOCKABLE_SOUND_DEFAULT_WIDTH             = 0.4f;
const float SimpleEditor::DOCKABLE_TEXTURE_DEFAULT_WIDTH           = 0.8f;
const float SimpleEditor::DOCKABLE_VERTEX_DEFAULT_WIDTH            = 0.4f;
const float SimpleEditor::DOCKABLE_OBJECT_PROPERTIES_DEFAULT_WIDTH = 0.62f;

void SimpleEditor::load(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceRegistry* runtimeContext, DOMNodeWrapper* options) {  
  cMapEditorMode = false;
  if (options != nullptr) {
    for (int i = 0; i < options->getChildCount(); i++) {
      DOMNodeWrapper *mNode = options->getChild(i);
      std::string mValueAsString = mNode->getNodeName();
      if (mValueAsString == "MapEditorMode") {
        cMapEditorMode = mNode->getBooleanAttribute("value");
      }
    }
  }

  ICommand* mCommand = CommandManager::getCommand("Pop");
  cExitCommands.push_back(mCommand);
  cSelectedLayer = nullptr;
  
  cProject = nullptr;
  cRunExitCommands = false;
  cConfirmExitCommands = false;
  cEditorFocus = true;

  // Register commands
  cEditorCommands[COMMAND_SAVE_AS]       = new SaveAsCommand(this, true, this);
  cEditorCommands[COMMAND_SAVE]          = new SaveAsCommand(this, false, this);
  cEditorCommands[COMMAND_OPEN]          = new CommandDialog<DialogProjectOpen>(this, this);
  cEditorCommands[COMMAND_OPEN_TEMPLATE] = new CommandDialog<DialogProjectOpenTemplate>(this, this);
  cEditorCommands[COMMAND_TEST]          = new TestCommand(this);
  cEditorCommands[COMMAND_MODULES]       = new CommandDialog<DialogModules>(this, this);
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
  cTesting = false;

  // Setup map for editing
  runtimeContext->add(this, "Editor", node);
}

void SimpleEditor::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resources) {
  // TODO
}

void SimpleEditor::initialiseResource(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources) {
  IResourceManager* mProjectResources = cProject->getResourceManager();
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Font") {
      std::string mFontName = mNode->getAttribute("name");
      cFont = resources->getFont(mFontName);
      LookAndFeel::setDefaultFont(cFont, 0.02f);
    } else if (mValueAsString == "MenuBar") {
      bool mMapEditor = mNode->getBooleanAttribute("mapEditor");
      if (cMapEditorMode == mMapEditor) {
        cMenuBar = new MenuBar(this, mNode, this, resources);
        addComponent(cMenuBar);
      }
    } else if (mValueAsString == "ResourcesIcon") {
      std::string mIconName = mNode->getAttribute("icon");
      std::string mIconModel = mNode->getAttribute("iconModel");
      I3DModelType* mModelType = resources->getModelType(mIconModel);
      Vertex* mVertex = new Vertex();
      I3DModel* mModelInstance = mModelType->createModel(mVertex, 1.0f);
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
  cDockableObjectProperties   = new DialogObjectProperties(  this, resources);

  if (cMapEditorMode) {
    cScreenEdgeManager.add(cDockableElementTypeManager, cResourceIcons["IconElementTypes"], DOCKABLE_ELEMENT_TYPE_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockableObjectProperties,   cResourceIcons["IconCustomTypes"],  DOCKABLE_OBJECT_PROPERTIES_DEFAULT_WIDTH);
  } else {
    cScreenEdgeManager.add(cDockableTextureManager,     cResourceIcons["IconTextures"],     DOCKABLE_TEXTURE_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockableElementTypeManager, cResourceIcons["IconElementTypes"], DOCKABLE_ELEMENT_TYPE_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockableSoundManager,       cResourceIcons["IconSounds"],       DOCKABLE_SOUND_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconFonts"],        DOCKABLE_FONT_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockableScriptManager,      cResourceIcons["IconScripts"],      DOCKABLE_SCRIPT_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockablePrimitiveManager,   cResourceIcons["IconPrimitives"],   DOCKABLE_PRIMITIVE_DEFAULT_WIDTH);
//    cScreenEdgeManager.add(cDockableCustomTypeManager,  cResourceIcons["IconCustomTypes"],  0.4f);
    cScreenEdgeManager.add(cDockableVertexManager,      cResourceIcons["IconVertices"],     DOCKABLE_VERTEX_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockable3DModelManager,     cResourceIcons["Icon3DModels"],     DOCKABLE_3D_MODEL_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockableCameraManager,      cResourceIcons["IconCameras"],      DOCKABLE_CAMERA_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockableBoundariesManager,  cResourceIcons["IconCollectables"], DOCKABLE_BOUNDARIES_DEFAULT_WIDTH);
    cScreenEdgeManager.add(cDockableObjectProperties,   cResourceIcons["IconCustomTypes"],  DOCKABLE_OBJECT_PROPERTIES_DEFAULT_WIDTH);
  }
  addComponent(&cScreenEdgeManager);
  
  cDockableElementTypeManager->addElementTypeSelectionListener(this);
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
//       if (mElement != nullptr) {
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
      if (keyDown(event.key.keysym.sym)) {
        return true;
      }
      break;
    }
  }
  
  if (cSelectedLayer != nullptr) {
    if (cSelectedLayer->inputEditor(event)) {
      return true;
    }
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

bool SimpleEditor::input(SDL_Event& event) {
  if (cTesting) {
    cProject->inputRuntime(event);
  } else {
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
      if (!mEventConsumed) {
        mEventConsumed = editorInput(event);
      }
    }

    // Passive input (e.g. mouse overs, etc.)
    if (!mEventConsumed) {
      for (int i = cHUDComponents.size() - 1; i >= 0; i--) {
        if (cHUDComponents[i]->input(event)) {
          return true;
        }
      }
    }
  }
  return false;
}

bool SimpleEditor::inputEditor(SDL_Event& event) {
  return false; // We can't edit the editor.
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
void SimpleEditor::resourceSelected(IElementType* elementType) {
  cSelectedLayer->resourceSelected(elementType);
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

void SimpleEditor::saveCurrentMap(const std::string& fileName) {
  cProject->save(fileName);
}

void SimpleEditor::testCurrentMap() {
  cTesting = true;
  cProject->initRuntime();
}
 
bool SimpleEditor::hasFileName() {
  return cProject->hasFileName();
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

ILayer* SimpleEditor::getLayer(DOMNodeWrapper* node, DOMNodeWrapper* cache, IResourceAccessor* resources, bool editing, bool asTemplate) {
  return this;
}

std::string SimpleEditor::getInstanceName(ILayer* layer) {
  return "<default>";
}

void SimpleEditor::destroy(ILayer* layer) {
  // Editor will not self terminate
}

void SimpleEditor::reset() {
  // Not supported
}

void SimpleEditor::addObjectSelectionListener(IObjectSelectionListener* listener) {
  // Not supported
}

void SimpleEditor::renderRuntime() {
  if (cTesting) {
    cProject->renderRuntime();
  } else {
    if (cSelectedLayer != nullptr) {
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
}

void SimpleEditor::renderEditing() {
  // Nothing to do
}

void SimpleEditor::updateRuntime(unsigned int milliseconds) {
  if (cTesting) {
    cProject->updateRuntime(milliseconds);
    if (cProject->hasCompleted()) {
      cTesting = false;
      cProject->reset();
      Configuration* mConfiguration = Configuration::getInstance();
      ScreenConfiguration* mScreen = mConfiguration->getScreenConfiguration();
      mScreen->setMode(mScreen->getScreenMode());
    }
  } else {
    for (unsigned int i = 0; i < cHUDComponents.size(); i++) {
      cHUDComponents[i]->update(milliseconds);
    }
    
    if (cSelectedLayer != nullptr) {
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
}

void SimpleEditor::updateEditing(unsigned int milliseconds) {
  // Nothing to do
}

void SimpleEditor::initRuntime() {
}

void SimpleEditor::initEditor() {
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

void SimpleEditor::addResourceSelectionListener(IResourceSelectionListener<I3DModelType>* listener) {
  cDockable3DModelManager->add3DModelSelectionListener(listener);
}

void SimpleEditor::addResourceSelectionListener(IResourceSelectionListener<IBoolean>* listener) {
  cDockablePrimitiveManager->addBooleanSelectionListener(listener);
}

void SimpleEditor::addResourceSelectionListener(IResourceSelectionListener<IColour>* listener) {
  cDockableTextureManager->addColourSelectionListener(listener);
}

void SimpleEditor::addResourceSelectionListener(IResourceSelectionListener<IFloat>* listener) {
  cDockablePrimitiveManager->addFloatSelectionListener(listener);
}

void SimpleEditor::addResourceSelectionListener(IResourceSelectionListener<IInteger>* listener) {
  cDockablePrimitiveManager->addIntegerSelectionListener(listener);
}

void SimpleEditor::addResourceSelectionListener(IResourceSelectionListener<IString>* listener) {
  cDockablePrimitiveManager->addStringSelectionListener(listener);
}

void SimpleEditor::addResourceSelectionListener(IResourceSelectionListener<ITexture>* listener) {
  cDockableTextureManager->addTextureSelectionListener(listener);
}

void SimpleEditor::removeResourceSelectionListener(IResourceSelectionListener<I3DModelType>* listener) {
  cDockable3DModelManager->remove3DModelSelectionListener(listener);
}

void SimpleEditor::removeResourceSelectionListener(IResourceSelectionListener<IBoolean>* listener) {
  cDockablePrimitiveManager->removeBooleanSelectionListener(listener);
}

void SimpleEditor::removeResourceSelectionListener(IResourceSelectionListener<IColour>* listener) {
  cDockableTextureManager->removeColourSelectionListener(listener);
}

void SimpleEditor::removeResourceSelectionListener(IResourceSelectionListener<IFloat>* listener) {
  cDockablePrimitiveManager->removeFloatSelectionListener(listener);
}

void SimpleEditor::removeResourceSelectionListener(IResourceSelectionListener<IInteger>* listener) {
  cDockablePrimitiveManager->removeIntegerSelectionListener(listener);
}

void SimpleEditor::removeResourceSelectionListener(IResourceSelectionListener<IString>* listener) {
  cDockablePrimitiveManager->removeStringSelectionListener(listener);
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

void SimpleEditor::objectSelected(IObjectWithProperties* object) {
  cDockableObjectProperties->objectSelected(object);
}

void SimpleEditor::openProject(const std::string& file, bool asTemplate) {
  DOMNodeWrapper* mConfigurationRootNode = new DOMNodeWrapper(file);
  std::string mCacheFileName = file.substr(0, file.length() - 10) + "/project.cache";
  DOMNodeWrapper* mCache = nullptr;
  if (System::fileExists(mCacheFileName)) {
    mCache = new DOMNodeWrapper(mCacheFileName);
  }
  for (int i = 0; i < mConfigurationRootNode->getChildCount(); i++) {
    DOMNodeWrapper *mNode = mConfigurationRootNode->getChild(i);
    std::string mValue = mNode->getNodeName();
    if (mValue == "Project") {
      if (cProject != nullptr) {
        clearUndoStack();
        delete cProject;
      }
      cProject = new Project(mNode, mCache, file, this, asTemplate, nullptr);
      cProject->initEditor();
      IResourceManager* mProjectResources = cProject->getResourceManager();
      std::vector<IDialogGenerator*> mProjectDialogGenerators = mProjectResources->getDialogGenerators();
      for (unsigned int i = 0; i < mProjectDialogGenerators.size(); i++) {
        Dialog* mProjectDialog = mProjectDialogGenerators[i]->createDialog(this, nullptr);
        cScreenEdgeManager.add(mProjectDialog, cResourceIcons["IconVertices"], 0.15f); // TODO: ICON!  SIZE!
      }
      cSelectedLayer = cProject->getDefaultLayer();
      for (unsigned int i = 0; i < cProjectManagerListeners.size(); i++) {
        cProjectManagerListeners[i]->projectOpened(cProject);
      }
      cProject->addObjectSelectionListener(this);
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
