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

SimpleEditor::SimpleEditor(DOMNodeWrapper* node) {
  cRunExitCommands = false;
  cConfirmExitCommands = false;
  cEditorFocus = true;
  BlockLocation mLocation(-7, 0, 0);
  BlockLocation mSize(0, 7, 7);
  Zone* mInitZone = new Zone(mLocation, mSize);
  cMap = new Map();
  cMap->addZone(mInitZone);
  cCursor = new EditorCursor(cMap);
  Vertex mNormalDistance(0.0f, 0.0f, -20.0f);
  cViewPoint.addViewPoint(0, mNormalDistance, 315.0f, -50.0f, 0.0f);
  cViewPoint.setViewPoint(0);
  ElementSetRegistry* mElementSetRegistry = cMap->getElementSetRegistry();
  mElementSetRegistry->addElementRegistryListener(this);
  PluginRegistry* mPluginRegistry = cMap->getPluginRegistry();
  mPluginRegistry->addListener(this);
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "OnExit") {
      cExitCommands = parseCommands(mNode);
    } else if (mValueAsString == "EditorCommand") {
      std::string mCommandName = mNode->getAttribute("name");
      std::string mCommandType = mNode->getAttribute("type");
      // TODO: Convert below into function
      ICommand* mCommand = NULL;
      if (mCommandType == "TerminateEditorCommand") {
        mCommand = new TerminateEditorCommand(cConfirmExitCommands);
      } else if (mCommandType == "ElementSetInstancesCommand") {
        IComponentFactory* mComponentFactory = new ElementInstancesComponentFactory(this);
        mCommand = new ElementSetInstancesCommand(this, mComponentFactory);
      } else if (mCommandType == "SaveAsCommand") {
        mCommand = new SaveAsCommand(this, true);
      } else if (mCommandType == "SaveCommand") {
        mCommand = new SaveAsCommand(this, false);
      } else if (mCommandType == "OpenCommand") {
        mCommand = new OpenCommand(this);
      } else if (mCommandType == "ChooseElementsCommand") {
        IComponentFactory* mComponentFactory = new ElementsPaletteComponentFactory(this);
        mCommand = new ElementSetInstancesCommand(this, mComponentFactory);
      } else {
        std::cout << "Warning: type not recognised: " << mCommandType << std::endl;
        // TODO: Throw init exception!
      }
      EditorCommandManager::registerCommand(mCommandName, mCommand);
    } else if (mValueAsString == "Font") {
      IFontEngine* mFontEngine = GlobalConfiguration::getFontEngine();
      std::string mFontName = mNode->getAttribute("name");
      cFont = mFontEngine->getFont(mFontName);
      // TODO: Have a reference to the font!  Use that instead!
      // TODO: Make font common to IHUDComponent!
      Button::setFont(cFont);
      MenuItem::setFont(cFont);
      PluginRequirementsComponent::setFont(cFont);
      ChoosePluginInstanceComponent::setFont(cFont);
      ListSelectionBox::setFont(cFont);
      ImplementationsListComponent::setFont(cFont);
      InstancesListComponent::setFont(cFont);
      TextFieldComponent::setFont(cFont);
    } else if (mValueAsString == "MenuBar") {
      cMenuBar = new MenuBar(this, mNode, cFont);
      addComponent(cMenuBar);
    }
  }
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

bool SimpleEditor::componentInput(SDL_Event& event) {
  for (int i = cHUDComponents.size() - 1; i >= 0; i--) {
    if (cHUDComponents[i]->input(event)) {
      return true;
    }
  }
  return false;
}

void SimpleEditor::input(SDL_Event& event) {
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
    }
  } else {
    if (cEditorFocus) {
      if (!editorInput(event)) {
        if (componentInput(event)) {
          cEditorFocus = false;
        }
      }
    } else {
      if (!componentInput(event)) {
        if (editorInput(event)) {
          cEditorFocus = true;
        }
      }
    }
  }
}

void SimpleEditor::pushElement(IElement* element) {
  cCursor->pushElement(element);
  clearUndoStack();
}

void SimpleEditor::notifyDestruction(IElement* element) {
  cMap->removeElement(element);
}

void SimpleEditor::elementSelected(IElementFactory* elementFactory) {
  cCursor->setElementFactory(elementFactory);
}

void SimpleEditor::elementSetInstantiated(IElementSet* elementSet) {
  std::vector<IElementFactory*> mElementFactories = elementSet->getElementFactories();
  for (unsigned int i = 0; i < mElementFactories.size(); i++) {
    mElementFactories[i]->setEditingInfo(cCursor, this, this);
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
  mPlugin->setEditingInfo(this);
  std::vector<ICommandInfo*> mCommandInfo = mPlugin->getCommandInfo();
  for (unsigned int i = 0; i < mCommandInfo.size(); i++) {
    cMenuBar->addCommand(mCommandInfo[i]);
  }
}

void SimpleEditor::pluginInstanceRemoved(PluginRegistry* pluginRegistry, std::string type, std::string instance) {
  // TODO: Remove plug-in supported commands
}

/*  if (editMode == EDIT_COLOUR) {
    glLoadIdentity();
    glTranslatef(1.45, 2.75, -6.0);
    cColourEditor.render();
  }
  if (editMode == EDIT_BLOCK) {
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(-55.0, 1.0, 0.0, 0.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    cBlockEditor.render(*cTextureSet, angle, tilt);
  }
  if (editMode == EDIT_CONDITION) {
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -6.0);
    cConditionEditor.render();
  }*/

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
  clearUndoStack();
  delete cMap;
  delete cCursor;
  cMap = map;
  cCursor = new EditorCursor(cMap);
  ElementSetRegistry* mElementRegistry = cMap->getElementSetRegistry();
  mElementRegistry->setEditingInfo(cCursor, this, this);
}

SimpleEditor::ElementInstancesComponentFactory::ElementInstancesComponentFactory(SimpleEditor* parent) {
  cParent = parent;
}

IHUDComponent* SimpleEditor::ElementInstancesComponentFactory::createComponent() {
  PluginRegistry* mPluginRegistry = cParent->cMap->getPluginRegistry();
  ElementSetRegistry* mElementSetRegistry = cParent->cMap->getElementSetRegistry();
  return new ElementSetInstancesComponent(cParent, mPluginRegistry, mElementSetRegistry, -0.9f, -0.1f);
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
