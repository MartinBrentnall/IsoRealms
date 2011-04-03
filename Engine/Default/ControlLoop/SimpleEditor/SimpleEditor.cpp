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

const std::string SimpleEditor::COMMAND_SAVE_AS        = "SaveAs";
const std::string SimpleEditor::COMMAND_SAVE           = "Save";
const std::string SimpleEditor::COMMAND_EXIT           = "Exit";
const std::string SimpleEditor::COMMAND_OPEN           = "Open";
const std::string SimpleEditor::COMMAND_ELEMENT_SETS   = "ElementSets";
const std::string SimpleEditor::COMMAND_ELEMENTS       = "Elements";

SimpleEditor::SimpleEditor(DOMNodeWrapper* node) {
  cMap = NULL;
  cRunExitCommands = false;
  cConfirmExitCommands = false;
  cEditorFocus = true;
  BlockLocation mLocation(-7, 0, 0);
  BlockLocation mSize(0, 7, 7);
  Zone* mInitZone = new Zone(mLocation, mSize);
  Map* mMap = new Map();
  mMap->addZone(mInitZone);

  // Create dialog factories
  IComponentFactory* mElementPaletteDialogFactory = new ElementsPaletteComponentFactory(this);
  cElementSetsFactory                             = new EntityClassDialogFactory(this, cElementSetEntityClass);
  
  // Setup map for editing
  setMap(mMap);
  cCursor = new EditorCursor(cMap);
  Vertex mNormalDistance(0.0f, 0.0f, -20.0f);
  cViewPoint.addViewPoint(0, mNormalDistance, 315.0f, -50.0f, 0.0f);
  cViewPoint.setViewPoint(0);
  PluginRegistry* mPluginRegistry = cMap->getPluginRegistry();
  mPluginRegistry->addListener(this);
  
  // Register commands
  EditorCommandManager::registerCommand(COMMAND_EXIT,           new TerminateEditorCommand(cConfirmExitCommands));
  EditorCommandManager::registerCommand(COMMAND_SAVE_AS,        new SaveAsCommand(this, true));
  EditorCommandManager::registerCommand(COMMAND_SAVE,           new SaveAsCommand(this, false));
  EditorCommandManager::registerCommand(COMMAND_OPEN,           new OpenCommand(this));
  EditorCommandManager::registerCommand(COMMAND_ELEMENTS,       new ElementSetInstancesCommand(this, mElementPaletteDialogFactory));
  EditorCommandManager::registerCommand(COMMAND_ELEMENT_SETS,   new ElementSetInstancesCommand(this, cElementSetsFactory));

  assignDummyPlugin(&cFont, "Font");
  LookAndFeel::setDefaultFont(cFont, 0.02f);
  cFontSocket.push_back(new PlugSocket("Font"));

  // TODO: This three pass code path seems quite common for loading plugins; used here, in the editor and in the map.  Maybe we can reduce duplication.
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      // TODO: Deallocate plugins on destruction of the attract control loop
      cPluginRegistry.registerPlugin(mNode, &cCommandRegistry, NULL);
    }
  }
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "Plugin") {
      cPluginRegistry.connectPlugin(mNode);
    }
  }
  
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "OnExit") {
      cExitCommands = parseCommands(mNode);
    } else if (mValueAsString == "Plugin") {
      cPluginRegistry.loadConfiguration(mNode);
    } else if (mValueAsString == "UsePlugin") {
      cPluginRegistry.setPlugin(this, mNode);
    } else if (mValueAsString == "MenuBar") {
      cMenuBar = new MenuBar(this, mNode, this);
      addComponent(cMenuBar);
    }
  }
}

std::vector<PlugSocket*> SimpleEditor::getPlugSockets() {
  return cFontSocket;
}

void SimpleEditor::setPlugin(PlugSocket* socket, IPlugin* plugin) {
  if (socket->getType() == "Font") {
    assignPlugin(plugin, &cFont, *socket);
    // TODO: Make font into local variable!  // TODO: What does the todo to the left of this one mean?
    LookAndFeel::setDefaultFont(cFont, 0.02f);
  } else {
    // TODO: Throw
  }
}

IPlugin* SimpleEditor::getPlugin(PlugSocket* socket) {
  if (socket->getType() == "Font") {return cFont;}
  // TODO: Throw
  return NULL;
}

std::vector<ICommand*> SimpleEditor::parseCommands(DOMNodeWrapper* node) {
  std::vector<ICommand*> cParsedCommands;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "ExecuteCommand") {
      std::string mCommandType = mNode->getAttribute("type");
      ICommand* mCommand = CommandManager::getCommand(mCommandType);
      if (mCommand == NULL) { 
        std::cout << "Oh noes for " << mCommandType << std::endl;
      }
      cParsedCommands.push_back(mCommand);
    }
  }
  return cParsedCommands;
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
      // TODO: Duplicate code: 59217
      Zone* mZone = cCursor->getZone();
      Map* mMap = cCursor->getMap();
      mMap->notifyZoneAction(mZone); // TODO: Should this be here (immediately before the action), or when the zone is selected?
      if (mZone != NULL) {
        IElement* mElement = mZone->popElement();
        if (mElement != NULL) {
          cUndoStack.push(mElement);
          mElement->removed();
          mMap->zoneChanged(mZone);
        }
      }
      return true;
    }

    case SDLK_r: {
      if (!cUndoStack.empty()) {
        // TODO: Duplicate code: 59217
        Zone* mZone = cCursor->getZone();
        Map* mMap = cCursor->getMap();
        mMap->notifyZoneAction(mZone); // TODO: Should this be here (immediately before the action), or when the zone is selected?
        IElement* mElement = cUndoStack.top();
        cUndoStack.pop();
        mElement->added();
        cCursor->pushElement(mElement);
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

IElementContainer* SimpleEditor::pushElement(IElement* element) {
  clearUndoStack();
  return cCursor->pushElement(element);
}

IElementContainer* SimpleEditor::pushMapElement(IElement* element) {
  return cCursor->pushMapElement(element);
  // TODO: Need a map stack!
}

Zone* SimpleEditor::notifyDestruction(IElement* element) {
  return cMap->removeElement(element);
}

void SimpleEditor::elementSelected(IElementFactory* elementFactory) {
  cCursor->setElementFactory(elementFactory);
}

void SimpleEditor::elementSetInstantiated(IElementSet* elementSet) {
  std::vector<IElementFactory*> mElementFactories = elementSet->getElementFactories();
  for (unsigned int i = 0; i < mElementFactories.size(); i++) {
    mElementFactories[i]->setEditingContext(cCursor, this, this);
  }
}

void SimpleEditor::elementSetDestroyed(IElementSet* elementSet) {
  // Nothing to do.
}

void SimpleEditor::elementSetChanged(IElementSet* elementSet) {
  // Nothing to do.
}

void SimpleEditor::pluginInstanceAdded(PluginRegistry* pluginRegistry, std::string type, std::string instance) {
  IPlugin* mPlugin = pluginRegistry->getPlugin(type, instance);
  CommandDirectory* mCommandRegistry = cMap->getCommandRegistry();
  std::vector<std::string> mDirectory;
  mDirectory.push_back("Plugin");
  mDirectory.push_back(type);
  mDirectory.push_back(instance);
  CommandRegistryProxy* mCommandRegistryProxy = new CommandRegistryProxy(mCommandRegistry, mDirectory);
  mPlugin->setEditingContext(cCursor, this, mCommandRegistryProxy);
  std::vector<ICommandInfo*> mCommandInfo = mPlugin->getCommandInfo();
  for (unsigned int i = 0; i < mCommandInfo.size(); i++) {
    cMenuBar->addCommand(mCommandInfo[i]);
  }
}

void SimpleEditor::pluginInstanceRemoved(IPlugin* instance, std::string type) {
  // TODO: Remove plug-in supported commands
}

void SimpleEditor::execute(int milliseconds) {
  for (unsigned int i = 0; i < cHUDComponents.size(); i++) {
    cHUDComponents[i]->update(milliseconds);
  }
  cMap->update(milliseconds);
  if (cRunExitCommands) {
    for (unsigned int i = 0; i < cExitCommands.size(); i++) {
      cExitCommands[i]->execute();
    }
    cRunExitCommands = false;
  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
    cViewPoint.place();
    cCursor->moveToCamera();
    cMap->render();
    cMap->renderEditing();
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
    IElementSet* mElementSet = mElement->getElementSet();
    mElementSet->destroy(mElement);
    cUndoStack.pop();
  }
}

void SimpleEditor::saveCurrentMap() {
  cMap->save();
}

void SimpleEditor::setMap(Map* map) {
  if (cMap != NULL) {
    clearUndoStack();
    delete cMap;
    delete cCursor;
    delete cElementSetEntityClass;
    cElementSetEntityClass = NULL;
    cCursor = NULL;
  }
  cMap = map;
  if (cMap != NULL) {
    cCursor = new EditorCursor(cMap);
    ElementSetRegistry* mElementSetRegistry = cMap->getElementSetRegistry();
    CommandDirectory* mCommandRegistry = cMap->getCommandRegistry();
    mElementSetRegistry->setEditingInfo(cCursor, this, this, mCommandRegistry);
    mElementSetRegistry->addElementRegistryListener(this);
    cElementSetEntityClass = new ElementSetEntityClass(mElementSetRegistry, this, this);
    cElementSetsFactory->setEntityClass(cElementSetEntityClass);
  }
}

PluginRegistry* SimpleEditor::getPluginRegistry() {
  return cMap->getPluginRegistry();
}

SimpleEditor::EntityClassDialogFactory::EntityClassDialogFactory(SimpleEditor* parent, IEntityClass* entityClass) {
  cParent = parent;
  setEntityClass(entityClass);
}

void SimpleEditor::EntityClassDialogFactory::setEntityClass(IEntityClass* entityClass) {
  cEntityClass = entityClass;
}

IHUDComponent* SimpleEditor::EntityClassDialogFactory::createComponent() {
  return new EntityClassInstancesComponent(cEntityClass, cParent);
}

SimpleEditor::ElementsPaletteComponentFactory::ElementsPaletteComponentFactory(SimpleEditor* parent) {
  cParent = parent;
}

IHUDComponent* SimpleEditor::ElementsPaletteComponentFactory::createComponent() {
  ElementSetRegistry* mElementSetRegistry = cParent->cMap->getElementSetRegistry();
  ChooseElementsComponent* mComponent = new ChooseElementsComponent(cParent, mElementSetRegistry);
  mComponent->addElementSelectionListener(cParent);
  return mComponent;
}

extern "C" IControlLoop* create(DOMNodeWrapper* node) {
  return new SimpleEditor(node);
}

extern "C" void destroy(IControlLoop* controlLoop) {
  delete controlLoop;
}
