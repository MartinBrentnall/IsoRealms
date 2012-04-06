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
#ifndef PLUGIN_REQUIREMENTS_COMPONENT_H
#define PLUGIN_REQUIREMENTS_COMPONENT_H

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <vector>
#include <string>

#include <IsoRealms/GUI/ComponentEdgeLayout.h>
#include <IsoRealms/GUI/EdgeRelation.h>
#include <IsoRealms/GUI/FlexibleGridLayoutComponent.h>
#include <IsoRealms/GUI/ResizableDialog.h>
#include <IsoRealms/GUI/ScrollableContainer.h>
#include <IsoRealms/GUI/TextLabelComponent.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IFont.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/IPluginSupport.h>
#include <IsoRealms/PluginRegistry.h>
#include <IsoRealms/System.h>

#include "ChoosePluginImplementationCommand.h"
#include "EntityClassInstancesComponent.h"
#include "IPluginRegistryAccessor.h"

// TODO: Overall, class is too hackish.  Has too much in common with ManageLibrariesComponent
class PluginRequirementsComponent:public ResizableDialog {
  private:
  static const int TYPE_COLUMN = 0;
  static const int INSTANCE_COLUMN = 1;
  static const int CHOOSE_BUTTON_COLUMN = 2;
  static const int RESET_BUTTON_COLUMN = 3;

  /**
   * The plugin support to satisfy.
   */
  IPluginSupport* cPluginSupport;

  /**
   * Plugin supported by the element set.
   */
  std::vector<PlugSocket*> cSupportedPlugins;
  IComponentContainer* cComponentContainer;

  bool keyDown(SDLKey&);

  class CloseCommand:public ICommand {
    private:
    PluginRequirementsComponent* cParent;

    public:
    CloseCommand(PluginRequirementsComponent* parent);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  class ResetSocketCommand:public ICommand {
    private:
    IPluginSupport* cPluginSupport;
    PlugSocket* cPlugSocket;

    public:
    ResetSocketCommand(IPluginSupport*, PlugSocket*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  std::string getTitle(IPluginSupport*);

  public:
  PluginRequirementsComponent(IComponentContainer*, IPluginRegistryAccessor*, IPluginSupport*, float, float);

  /****************************\
   * Implements IHUDComponent *
  \****************************/
  void updateResizableDialogContent(int);
  void renderResizableDialogContent();
  bool inputResizableDialogContent(SDL_Event&);

  /***********************************\
   * Implements IItemSelectedCommand *
  \***********************************/
  void itemSelected(std::string);
};

#endif
