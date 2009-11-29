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
#ifndef SIMPLE_EDITOR_H
#define SIMPLE_EDITOR_H

#include <iostream>
#include <stack>
#include <SDL/SDL.h>

#include "../../IControlLoop.h"

#include "../../../../Global/CommandManager.h"
#include "../../../../Global/Configuration.h"
#include "../../../../Global/DOMNodeWrapper.h"
#include "../../../../Global/ElementSetRegistry.h"
#include "../../../../Global/GlobalConfiguration.h"
#include "../../../../Global/ICommandInfo.h"
#include "../../../../Global/IComponentContainer.h"
#include "../../../../Global/IElementFactory.h"
#include "../../../../Global/IElementGateway.h"
#include "../../../../Global/IElementRegistryListener.h"
#include "../../../../Global/IFont.h"
#include "../../../../Global/IFontEngine.h"
#include "../../../../Global/IHUDComponent.h"
#include "../../../../Global/IPluginRegistryListener.h"
#include "../../../../Global/Map.h"
#include "../../../../Global/ScreenConfiguration.h"

class OpenCommand;

#include "Button.h"
#include "Camera.h"
#include "ChooseElementsCommand.h"
#include "ChoosePluginInstanceComponent.h"
#include "EditorCommandManager.h"
#include "EditorCursor.h"
#include "ElementSetInstancesCommand.h"
#include "ElementSetInstancesComponent.h"
#include "IComponentFactory.h"
#include "IElementSelectionListener.h"
#include "IMapManager.h"
#include "ImplementationsListComponent.h"
#include "InstancesListComponent.h"
#include "ListSelectionBox.h"
#include "MenuBar.h"
#include "MenuItem.h"
#include "OpenCommand.h"
#include "PluginRequirementsComponent.h"
#include "SaveAsCommand.h"
#include "TerminateEditorCommand.h"
#include "TextFieldComponent.h"

class SimpleEditor:public IControlLoop,
                   public IComponentContainer,
                   public IElementSelectionListener,
                   public IElementRegistryListener,
                   public IElementGateway,
                   public IPluginRegistryListener,
                   public IMapManager {
  private:
  Map* cMap;
  Camera cViewPoint;
  EditorCursor* cCursor;
  IFont* cFont;
  bool cEditorFocus;
  MenuBar* cMenuBar;

  // TODO: Need undo stack per zone... or at least to empty it when changing zones.
  std::stack<IElement*> cUndoStack;

  std::vector<IHUDComponent*> cHUDComponents;

  /**
   * This flag is set when exit commands are to be run in the next cycle.  It
   * is reset to false after the exit commands have been executed.
   */
  bool cRunExitCommands;

  /**
   * This flag is set when we want to request the user to confirm that the exit
   * commands are to be run.  A message prompt is shown and the editor waits
   * for a response from the user.
   */
  bool cConfirmExitCommands;

  /**
   * Exit commands to run.
   */
  std::vector<ICommand*> cExitCommands;
  

  std::vector<ICommand*> parseCommands(DOMNodeWrapper*);

  /**
   * Clear the undo stack.  Called when the current undo stack is rendered
   * meaningless by some action (e.g. changed zone, loaded new map, etc.)
   */
  void clearUndoStack();

  int getComponentIndex(IHUDComponent*);
  bool keyDown(SDLKey& key);

  bool editorInput(SDL_Event&);
  bool componentInput(SDL_Event&);

  bool componentAt(float, float);

  class ElementInstancesComponentFactory:public IComponentFactory {
    private:
    SimpleEditor* cParent;

    public:
    ElementInstancesComponentFactory(SimpleEditor*);

    /********************************\
     * Implements IComponentFactory *
    \********************************/
    IHUDComponent* createComponent();
  };

  class ElementsPaletteComponentFactory:public IComponentFactory {
    private:
    SimpleEditor* cParent;

    public:
    ElementsPaletteComponentFactory(SimpleEditor*);

    /********************************\
     * Implements IComponentFactory *
    \********************************/
    IHUDComponent* createComponent();
  };

  public:
  SimpleEditor(DOMNodeWrapper*);

  /******************************\
   * Implements IElementGateway *
  \******************************/
  void saveCurrentMap();
  void setMap(Map*); // TODO: Not an interface yet.

  /******************************\
   * Implements IElementGateway *
  \******************************/
  void pushElement(IElement*);
  void setDirty(IElement*);

  /****************************************\
   * Implements IElementSelectionListener *
  \****************************************/
  void elementSelected(IElementFactory*);

  /***************************************\
   * Implements IElementRegistryListener *
  \***************************************/
  void elementSetInstantiated(IElementSet*);
  void elementSetDestroyed(IElementSet*);
  void elementSetChanged(IElementSet*);

  /***************************************\
   * Implements IElementRegistryListener *
  \***************************************/
  void pluginInstanceAdded(PluginRegistry*, std::string, std::string);
  void pluginInstanceRemoved(PluginRegistry*, std::string, std::string);

  /**********************************\
   * Implements IComponentContainer *
  \**********************************/
  void addComponent(IHUDComponent*);
  void removeComponent(IHUDComponent*);
  void bringComponentToFront(IHUDComponent*);

  /***************************\
   * Implements IControlLoop *
  \***************************/
  void input(SDL_Event&);
  void execute(int);  
};

#endif
