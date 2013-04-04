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

#include <IsoRealms/CommandManager.h>
#include <IsoRealms/Configuration.h>
#include <IsoRealms/Persistence/DOMNodeWrapper.h>
#include <IsoRealms/GUI/ComponentCustomType.h>
#include <IsoRealms/GUI/Dialogs/ComponentColourSelector.h>
#include <IsoRealms/GUI/MenuBar.h>
#include <IsoRealms/GUI/OpenDialogCommand.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IEditingContext.h>
#include <IsoRealms/IHUDComponent.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/Map.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/Registry.h>
#include <IsoRealms/Resources/Resources.h>
#include <IsoRealms/ScreenConfiguration.h>

class OpenCommand;

#include "Camera.h"
#include "ComponentCustomTypeResources.h"
#include "EditorCursor.h"
#include "ElementSetInstancesCommand.h"
#include "EntityClassInstancesComponent.h"
#include "IComponentFactory.h"
#include "IElementSelectionListener.h"
#include "IMapManager.h"
#include "OpenCommand.h"
#include "ResourceManagerDialogs/ComponentResourceBrowser.h"
#include "ResourceManagerDialogs/DialogElementTypeManager.h"
#include "ResourceManagerDialogs/DialogFontManager.h"
#include "ResourceManagerDialogs/DialogSoundManager.h"
#include "ResourceManagerDialogs/DialogTextureManager.h"
#include "SaveAsCommand.h"
#include "ScreenEdge/ScreenEdgeManager.h"
#include "TerminateEditorCommand.h"

class SimpleEditor:public IPlugin,
                   public IComponentContainer,
                   public IEditingContext,
                   public IElementSelectionListener,
                   public IMapManager,
                   public IDynamicElement,
                   public IVisualElement,
		   public IInteractiveElement,
                   public IElementType,
                   public IElement,
		   public IProjectManager,
		   public ICommandSource,
                   public IResourceSelector {
  private:
  class EntityClassDialogFactory;
    
  static const std::string COMMAND_SAVE_AS;
  static const std::string COMMAND_SAVE;
  static const std::string COMMAND_EXIT;
  static const std::string COMMAND_OPEN;
  static const std::string COMMAND_ZONE_RENDERERS;
  static const std::string COMMAND_MODULES;
  static const std::string COMMAND_RESOURCE_BROWSER;
  
  IProject* cProject;
  std::map<std::string, ICommand*> cEditorCommands;
  std::vector<IProjectManagerListener*> cProjectManagerListeners;
  ScreenEdgeManager cScreenEdgeManager;
  Camera cViewPoint;
  EditorCursor* cCursor;
  IFont* cFont;
  bool cEditorFocus;
  MenuBar* cMenuBar;
  EntityClassDialogFactory* cElementSetsFactory;
  
  DialogTextureManager* cDockableTextureManager;
  DialogElementTypeManager* cDockableElementTypeManager;
  DialogSoundManager* cDockableSoundManager;
  DialogFontManager* cDockableFontManager;

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

  public:
  void createResources(DOMNodeWrapper*, IRuntimeContext*);
  
  void initialiseResource(DOMNodeWrapper*, IResourceAccessor*);

  IResourceSelector* getResourceSelector();
  
  /******************************\
   * Implements IElementGateway *
  \******************************/
  void saveCurrentMap();
  void setProject(IProject*); // TODO: Not an interface yet.

  /******************************\
   * Implements IElementGateway *
  \******************************/
//   IElementContainer* pushElement(IElement*);
//   IElementContainer* pushMapElement(IElement*);
//   void removeElement(IElement*);

  /****************************************\
   * Implements IElementSelectionListener *
  \****************************************/
  void elementSelected(IElementType*);

  /**************************************\
   * Implements IPluginRegistryListener *
  \**************************************/
  void pluginInstanceAdded(PluginRegistry*, std::string);
  void pluginInstanceRemoved(IPlugin*, std::string);

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
  void render();
  void update(unsigned int);
  
  /******************************\
   * Implements IEditingContext *
  \******************************/
  void registerCommand(ICommandInfo*);
  IComponentContainer* getComponentContainer();
  BlockLocation* getBlockLocation();
  void staticChanged();
  
  /***************************\
   * Implements IElementType *
  \***************************/
  IPlugin* getElementSet();
  IElement* getElement(DOMNodeWrapper*, BlockLocation*, IElementContainer*);
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void configureElement();
  bool input(SDL_Event&);
  void renderEditingPreview();
  void renderIcon();
  void updateIcon(int);
  void destroy(IElement*);
  IElementHandler* getElementHandler();

  /***********************\
   * Implements IElement * 
  \***********************/
  IElementType* getElementType();
  bool initElement(unsigned int);
  void renderStatic();
  void renderStaticEditing();
  std::vector<IVisualElement*> getVisualElements();
  std::vector<IDynamicElement*> getDynamicElements();
  std::vector<IDynamicElement*> getDynamicElementsRuntime();
  std::vector<IInteractiveElement*> getInteractiveElements();
  void save(DOMNodeWriter*, IResourceLocator*, BlockLocation&);
  void setDirty();
  
  /******************************\
   * Implements IProjectManager *
  \******************************/
  void addProjectListener(IProjectManagerListener*);
  
  /*****************************\
   * Implements ICommandSource *
  \*****************************/
  ICommand* getCommand(const std::string&);
  
  /********************************\
   * Implements IResourceSelector *
  \********************************/
  void addResourceSelectionListener(IResourceSelectionListener<IColour>*);
  void addResourceSelectionListener(IResourceSelectionListener<ITexture>*);
  void removeResourceSelectionListener(IResourceSelectionListener<IColour>*);
  void removeResourceSelectionListener(IResourceSelectionListener<ITexture>*);
  void notifyResourceReleased(IColour*);
  void notifyResourceOwned(IColour*);
};

#endif
