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

void SimpleEditor::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {  
  ICommand* mCommand = CommandManager::getCommand("Pop");
  cExitCommands.push_back(mCommand);
  
  cProject = NULL;
  cRunExitCommands = false;
  cConfirmExitCommands = false;
  cEditorFocus = true;
  BlockLocation mLocation(-7, 0, 0);
  BlockLocation mSize(0, 7, 7);
  Zone* mInitZone = new Zone(mLocation, mSize);

  // Register commands
  cEditorCommands[COMMAND_EXIT]    = new TerminateEditorCommand(cConfirmExitCommands);
  cEditorCommands[COMMAND_SAVE_AS] = new SaveAsCommand(this, true);
  cEditorCommands[COMMAND_SAVE]    = new SaveAsCommand(this, false);
  cEditorCommands[COMMAND_OPEN]    = new OpenCommand(this);
//  EditorCommandManager::registerCommand(COMMAND_MODULES,          new CommandModules());
//  EditorCommandManager::registerCommand(COMMAND_RESOURCE_BROWSER, new CommandResourceBrowser());

  IProject* mProject;
//  if (mMapName == "") {
    mProject = new Project();
    mProject->addZone(mInitZone);
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
  cCursor = new EditorCursor(cProject);
  Vertex mNormalDistance(0.0f, 0.0f, -20.0f);
  cViewPoint.addViewPoint(0, mNormalDistance, 315.0f, -50.0f, 0.0f);
  cViewPoint.setViewPoint(0);
  runtimeContext->add(&cViewPoint, "Camera", NULL);
  runtimeContext->add(this, "Editor", node);
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
      I3DModelFactory* mModelType = resources->getModelType(mIconModel);
      Vertex* mVertex = new Vertex();
      I3DModel* mModelInstance = mModelType->createModel(mVertex, 1.0f);
      std::cout << "=================================================> " << mIconName << std::endl;
      cResourceIcons[mIconName] = new GUIIcon(mModelInstance);
    }
  }
  cDockableTextureManager     = new DialogTextureManager(    this, resources, mProjectResources, this, mProjectResources);
  cDockableElementTypeManager = new DialogElementTypeManager(this, resources, mProjectResources, this, mProjectResources);
  cDockableSoundManager       = new DialogSoundManager(      this, resources, mProjectResources, this, mProjectResources);
  cDockableFontManager        = new DialogFontManager(       this, resources, mProjectResources, this, mProjectResources);
  cScreenEdgeManager.add(cDockableTextureManager,     cResourceIcons["IconTextures"]);
  cScreenEdgeManager.add(cDockableElementTypeManager, cResourceIcons["IconElementTypes"]);
  cScreenEdgeManager.add(cDockableSoundManager,       cResourceIcons["IconSounds"]);
  cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconFonts"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconScripts"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconPrimitives"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconSurfaceProcessors"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconCustomTypes"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconHUDComponents"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconZoneHandlers"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconVertices"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["Icon3DModels"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconCameras"]);
//   cScreenEdgeManager.add(cDockableFontManager,        cResourceIcons["IconCollectables"]);
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
      IElement* mElement = cCursor->popElement();
      if (mElement != NULL) {
        cUndoStack.push(mElement);
      }
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
  if (cCursor->input(event)) {
    return true;
  }

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

  if (cViewPoint.input(event)) {
    return true;
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
        return false;
      }
    }
  }
  return false;
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
  cCursor->setElementType(elementType);
}

void SimpleEditor::render() {
  cCursor->moveToCamera();
  cProject->render();
  cProject->renderEditing();
  cCursor->render();
  
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

void SimpleEditor::update(unsigned int milliseconds) {
  for (unsigned int i = 0; i < cHUDComponents.size(); i++) {
    cHUDComponents[i]->update(milliseconds);
  }
  cProject->update(milliseconds);
  if (cRunExitCommands) {
    for (unsigned int i = 0; i < cExitCommands.size(); i++) {
      cExitCommands[i]->execute();
    }
    cRunExitCommands = false;
  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
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
 
void SimpleEditor::setProject(IProject* project) {
  if (cProject != NULL) {
    clearUndoStack();
    delete cProject;
    delete cCursor;
    cCursor = NULL;
  }
  cProject = project;
  if (cProject != NULL) {
    cCursor = new EditorCursor(cProject);
  /* TODO: What did this do?    ElementSetRegistry* mElementSetRegistry = cProject->getElementSetRegistry();
//     mElementSetRegistry->setEditingInfo(cCursor, this, this, this);
//     mElementSetRegistry->addElementRegistryListener(this);
//     cElementSetEntityClass = new ElementSetEntityClass(mElementSetRegistry, this, this);
//     cElementSetsFactory->setEntityClass(cElementSetEntityClass);*/
  }
  for (unsigned int i = 0; i < cProjectManagerListeners.size(); i++) {
    cProjectManagerListeners[i]->projectOpened(cProject);
  }
  cProject->setEditingContext(this);
}

void SimpleEditor::registerCommand(ICommandInfo* commandInfo) {
  cMenuBar->addCommand(commandInfo);
}

IComponentContainer* SimpleEditor::getComponentContainer() {
  return this;
}

BlockLocation* SimpleEditor::getBlockLocation() {
  return cCursor;
}

void SimpleEditor::staticChanged() {
  cProject->staticChanged();
}

IPlugin* SimpleEditor::getElementSet() {
  return this;
}

IElement* SimpleEditor::getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*) {
  return this;
}

void SimpleEditor::setEditingContext(BlockLocation*, IComponentContainer*) {
  // Not editable.  Nothing to do
}

void SimpleEditor::configureElement() {
  // Not editable.  Nothing to do
}

void SimpleEditor::renderEditingPreview() {
  // Not editable.  Nothing to do
}

void SimpleEditor::renderIcon() {
  // Not editable.  Nothing to do
}

void SimpleEditor::updateIcon(int) {
  // Not editable.  Nothing to do
}

void SimpleEditor::destroy(IElement* element) {
  // Editor will not self terminate
}

IElementHandler* SimpleEditor::getElementHandler() {
  return NULL;
}

IElementType* SimpleEditor::getElementType() {
  return this;
}

bool SimpleEditor::initElement(unsigned int) {
  return true;
}

void SimpleEditor::renderStatic() {
  // Nothing to do
}

void SimpleEditor::renderStaticEditing() {
  // Nothing to do
}

std::vector<IVisualElement*> SimpleEditor::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  mVisualElements.push_back(this);
  return mVisualElements;
}

std::vector<IDynamicElement*> SimpleEditor::getDynamicElements() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IDynamicElement*> SimpleEditor::getDynamicElementsRuntime() {
  std::vector<IDynamicElement*> mDynamicElements;
  mDynamicElements.push_back(this);
  return mDynamicElements;
}

std::vector<IInteractiveElement*> SimpleEditor::getInteractiveElements() {
  std::vector<IInteractiveElement*> mInteractiveElements;
  mInteractiveElements.push_back(this);
  return mInteractiveElements;
}

void SimpleEditor::save(DOMNodeWriter*, IResourceLocator*, BlockLocation&) {
  // Not editable.  Nothing to do
}

void SimpleEditor::setDirty() {
  // Nothing to do
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

extern "C" IPlugin* create() {
  return new SimpleEditor();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
