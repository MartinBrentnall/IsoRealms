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
#include <IsoRealms/GUI/Dialogs/ComponentColourSelector.h>
#include <IsoRealms/GUI/GUIIcon.h>
#include <IsoRealms/GUI/IDialogGenerator.h>
#include <IsoRealms/GUI/MenuBar.h>
#include <IsoRealms/GUI/OpenDialogCommand.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IEditingContext.h>
#include <IsoRealms/IHUDComponent.h>
#include <IsoRealms/IObjectSelectionListener.h>
#include <IsoRealms/IObjectWithProperties.h>
#include <IsoRealms/Project.h>
#include <IsoRealms/Resources/ElementType/IElementType.h>
#include <IsoRealms/Resources/Font/IFont.h>
#include <IsoRealms/Resources/Registry.h>
#include <IsoRealms/Resources/Resources.h>
#include <IsoRealms/ScreenConfiguration.h>

class OpenCommand;

#include "CommandDialog.h"
#include "CommandSelectLayer.h"
#include "DialogModules.h"
#include "DialogObjectProperties.h"
#include "DialogProjectOpen.h"
#include "DialogProjectOpenTemplate.h"
#include "IComponentFactory.h"
#include "IMapManager.h"
#include "ResourceManagerDialogs/ComponentResourceBrowser.h"
#include "ResourceManagerDialogs/Dialog3DModelManager.h"
#include "ResourceManagerDialogs/DialogBoundariesManager.h"
#include "ResourceManagerDialogs/DialogCameraManager.h"
#include "ResourceManagerDialogs/DialogElementTypeManager.h"
#include "ResourceManagerDialogs/DialogFontManager.h"
#include "ResourceManagerDialogs/DialogPrimitiveManager.h"
#include "ResourceManagerDialogs/DialogScriptManager.h"
#include "ResourceManagerDialogs/DialogSoundManager.h"
#include "ResourceManagerDialogs/DialogTextureManager.h"
#include "ResourceManagerDialogs/DialogVertexManager.h"
#include "SaveAsCommand.h"
#include "ScreenEdge/ScreenEdgeManager.h"
#include "TestCommand.h"

class SimpleEditor:public IModule,
                   public IComponentContainer,
                   public IEditingContext,
                   public virtual IResourceSelectionListener<IElementType>,
                   public IMapManager,
                   public ILayerType,
                   public ILayer,
                   public IProjectManager,
                   public ICommandSource,
                   public IResourceSelector,
                   public IObjectSelectionListener,
                   public IEditor {
  private:
  class EntityClassDialogFactory;
    
  static const std::string COMMAND_SAVE_AS;
  static const std::string COMMAND_SAVE;
  static const std::string COMMAND_OPEN;
  static const std::string COMMAND_OPEN_TEMPLATE;
  static const std::string COMMAND_TEST;
  static const std::string COMMAND_MODULES;
  static const std::string COMMAND_RESOURCE_BROWSER;
  
  static const float DOCKABLE_3D_MODEL_DEFAULT_WIDTH;
  static const float DOCKABLE_BOUNDARIES_DEFAULT_WIDTH;
  static const float DOCKABLE_CAMERA_DEFAULT_WIDTH;
  static const float DOCKABLE_ELEMENT_TYPE_DEFAULT_WIDTH;
  static const float DOCKABLE_FONT_DEFAULT_WIDTH;
  static const float DOCKABLE_PRIMITIVE_DEFAULT_WIDTH;
  static const float DOCKABLE_SCRIPT_DEFAULT_WIDTH;
  static const float DOCKABLE_SOUND_DEFAULT_WIDTH;
  static const float DOCKABLE_TEXTURE_DEFAULT_WIDTH;
  static const float DOCKABLE_VERTEX_DEFAULT_WIDTH;
  static const float DOCKABLE_OBJECT_PROPERTIES_DEFAULT_WIDTH;
  
  IProject* cProject;
  ILayer* cSelectedLayer;
  std::map<std::string, ICommand*> cEditorCommands;
  std::map<std::string, GUIIcon*> cResourceIcons;
  std::vector<IProjectManagerListener*> cProjectManagerListeners;
  ScreenEdgeManager cScreenEdgeManager;
  IFont* cFont;
  bool cEditorFocus;
  MenuBar* cMenuBar;
  bool cTesting;
  EntityClassDialogFactory* cElementSetsFactory;
  bool cMapEditorMode;

  Dialog3DModelManager* cDockable3DModelManager;
  DialogBoundariesManager* cDockableBoundariesManager;
  DialogCameraManager* cDockableCameraManager;
  DialogElementTypeManager* cDockableElementTypeManager;
  DialogFontManager* cDockableFontManager;
  DialogPrimitiveManager* cDockablePrimitiveManager;
  DialogScriptManager* cDockableScriptManager;
  DialogSoundManager* cDockableSoundManager;
  DialogTextureManager* cDockableTextureManager;
  DialogVertexManager* cDockableVertexManager;
  DialogObjectProperties* cDockableObjectProperties;
  
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
  void load(DOMNodeWrapper*, DOMNodeWrapper*, IResourceRegistry*, DOMNodeWrapper*);

  void projectInitialised();
  
  void initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*);

  IResourceSelector* getResourceSelector();
  
  ICamera* getCamera();
  
  /**************************\
   * Implements IMapManager *
  \**************************/
  void saveCurrentMap();
  void saveCurrentMap(const std::string&);
  void testCurrentMap();
  bool hasFileName();
  void setProject(IProject*); // TODO: Not an interface yet.
  void selectLayer(ILayer*);

  /*******************************************************\
   * Implements IResourceSelectionListener<IElementType> *
  \*******************************************************/
  void resourceSelected(IElementType*);

  /**************************************\
   * Implements IModuleRegistryListener *
  \**************************************/
  void moduleInstanceAdded(ModuleRegistry*, std::string);
  void moduleInstanceRemoved(IModule*, std::string);

  /**********************************\
   * Implements IComponentContainer *
  \**********************************/
  void addComponent(IHUDComponent*);
  void removeComponent(IHUDComponent*);
  void bringComponentToFront(IHUDComponent*);
  void relinquishFocus(IHUDComponent*);

  /******************************\
   * Implements IEditingContext *
  \******************************/
  void registerCommand(ICommandInfo*);
  IComponentContainer* getComponentContainer();
  BlockLocation* getBlockLocation();
  void staticChanged();
  
  /*************************\
   * Implements ILayerType *
  \*************************/
  ILayer* getLayer(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*, bool, bool);
  std::string getInstanceName(ILayer*);
  
  /*********************\
   * Implements ILayer * 
  \*********************/
  void renderRuntime();
  void renderEditing();
  void updateRuntime(unsigned int);
  void updateEditing(unsigned int);
  bool input(SDL_Event&);
  bool inputEditor(SDL_Event&);
  ElementPickRay* getPickRay(float, float);
  void save(DOMNodeWriter*, DOMNodeWriter*, IResourceLocator*);
  void staticCnanged();
  void initRuntime();
  void initEditor();
  void destroy(ILayer*);
  void reset();
  void addObjectSelectionListener(IObjectSelectionListener*);
  
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
  void addResourceSelectionListener(IResourceSelectionListener<I3DModelType>*);
  void addResourceSelectionListener(IResourceSelectionListener<IBoolean>*);
  void addResourceSelectionListener(IResourceSelectionListener<IColour>*);
  void addResourceSelectionListener(IResourceSelectionListener<IFloat>*);
  void addResourceSelectionListener(IResourceSelectionListener<IInteger>*);
  void addResourceSelectionListener(IResourceSelectionListener<IString>*);
  void addResourceSelectionListener(IResourceSelectionListener<ITexture>*);
  void removeResourceSelectionListener(IResourceSelectionListener<I3DModelType>*);
  void removeResourceSelectionListener(IResourceSelectionListener<IBoolean>*);
  void removeResourceSelectionListener(IResourceSelectionListener<IColour>*);
  void removeResourceSelectionListener(IResourceSelectionListener<IFloat>*);
  void removeResourceSelectionListener(IResourceSelectionListener<IInteger>*);
  void removeResourceSelectionListener(IResourceSelectionListener<IString>*);
  void removeResourceSelectionListener(IResourceSelectionListener<ITexture>*);
  void notifyResourceReleased(IColour*);
  void notifyResourceOwned(IColour*);
  
  /***************************************\
   * Implements IObjectSelectionListener *
  \***************************************/
  void objectSelected(IObjectWithProperties*);
  
  /**********************\
   * Implements IEditor *
  \**********************/
  void openProject(const std::string&, bool);
};

#endif
