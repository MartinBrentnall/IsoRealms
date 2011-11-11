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

#include <IsoRealms/CommandManager.h>
#include <IsoRealms/CommandProxy.h>
#include <IsoRealms/Configuration.h>
#include <IsoRealms/DOMNodeWrapper.h>
#include <IsoRealms/ElementSetRegistry.h>
#include <IsoRealms/GUI/OpenDialogCommand.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IEditingContext.h>
#include <IsoRealms/IElementFactory.h>
#include <IsoRealms/IElementGateway.h>
#include <IsoRealms/IElementRegistryListener.h>
#include <IsoRealms/IFont.h>
#include <IsoRealms/IHUDComponent.h>
#include <IsoRealms/IPluginRegistryListener.h>
#include <IsoRealms/IPluginSupport.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/Map.h>
#include <IsoRealms/PlugSocket.h>
#include <IsoRealms/Registry.h>
#include <IsoRealms/ScreenConfiguration.h>

class OpenCommand;

#include "Camera.h"
#include "ChooseElementsCommand.h"
#include "EditorCommandManager.h"
#include "EditorCursor.h"
#include "ElementSetEntityClass.h"
#include "ElementSetInstancesCommand.h"
#include "EntityClassInstancesComponent.h"
#include "IComponentFactory.h"
#include "IElementSelectionListener.h"
#include "IMapManager.h"
#include "MenuBar.h"
#include "OpenCommand.h"
#include "SaveAsCommand.h"
#include "TerminateEditorCommand.h"

class SimpleEditor:public IControlLoop,
                   public IComponentContainer,
                   public IEditingContext,
                   public IElementSelectionListener,
                   public IElementRegistryListener,
                   public IElementGateway,
                   public IMapManager,
                   public IPluginRegistryAccessor,
                   public IPluginRegistryListener,
                   public IPluginSupport {
  private:
  class EntityClassDialogFactory;
    
  static const std::string COMMAND_SAVE_AS;
  static const std::string COMMAND_SAVE;
  static const std::string COMMAND_EXIT;
  static const std::string COMMAND_OPEN;
  static const std::string COMMAND_ELEMENT_SETS;
  static const std::string COMMAND_ELEMENTS;
  static const std::string COMMAND_ZONE_RENDERERS;
  
  Map* cMap;
  Camera cViewPoint;
  ElementSetEntityClass* cElementSetEntityClass;
  EditorCursor* cCursor;
  Registry<ICommand, CommandProxy> cCommandRegistry;
  Registry<IColour, ColourProxy> cColourRegistry;
  Registry<ITexture, TextureProxy> cTextureRegistry;
  PluginRegistry cPluginRegistry;
  std::vector<PlugSocket*> cFontSocket;
  IFont* cFont;
  bool cEditorFocus;
  MenuBar* cMenuBar;
  EntityClassDialogFactory* cElementSetsFactory;

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
  
  /**
   * Clear the undo stack.  Called when the current undo stack is rendered
   * meaningless by some action (e.g. changed zone, loaded new map, etc.)
   */
  void clearUndoStack();

  int getComponentIndex(IHUDComponent*);
  bool keyDown(SDLKey& key);

  bool editorInput(SDL_Event&);
  void testFocusChange(SDL_Event&);

  bool componentAt(float, float);

  class EntityClassDialogFactory:public IComponentFactory {
    private:
    SimpleEditor* cParent;
    IEntityClass* cEntityClass;

    public:
    EntityClassDialogFactory(SimpleEditor*, IEntityClass*);
    void setEntityClass(IEntityClass*);

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
  SimpleEditor(DOMNodeWrapper*, IEngineArguments*);

  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /******************************\
   * Implements IElementGateway *
  \******************************/
  void saveCurrentMap();
  void setMap(Map*); // TODO: Not an interface yet.

  /******************************\
   * Implements IElementGateway *
  \******************************/
  IElementContainer* pushElement(IElement*);
  IElementContainer* pushMapElement(IElement*);
  void removeElement(IElement*);

  /****************************************\
   * Implements IElementSelectionListener *
  \****************************************/
  void elementSelected(IElementFactory*);

  /******************************************\
   * Implements IElementSetRegistryListener *
  \******************************************/
  void elementSetInstantiated(IElementSet*);
  void elementSetDestroyed(IElementSet*);
  void elementSetChanged(IElementSet*);

  /**************************************\
   * Implements IPluginRegistryListener *
  \**************************************/
  void pluginInstanceAdded(PluginRegistry*, std::string, std::string);
  void pluginInstanceRemoved(IPlugin*, std::string);

  /**************************************\
   * Implements IPluginRegistryAccessor *
  \**************************************/
  PluginRegistry* getPluginRegistry();

  /**********************************\
   * Implements IComponentContainer *
  \**********************************/
  void addComponent(IHUDComponent*);
  void removeComponent(IHUDComponent*);
  void bringComponentToFront(IHUDComponent*);
  void relinquishFocus(IHUDComponent*);

  /***************************\
   * Implements IControlLoop *
  \***************************/
  void input(SDL_Event&);
  void execute(int);
  
  /******************************\
   * Implements IEditingContext *
  \******************************/
  void registerCommand(ICommandInfo*);
  IComponentContainer* getComponentContainer();
  BlockLocation* getBlockLocation();
};

#endif
