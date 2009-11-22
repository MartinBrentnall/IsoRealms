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
#ifndef CHOOSE_PLUGIN_INSTANCE_COMPONENT_H
#define CHOOSE_PLUGIN_INSTANCE_COMPONENT_H

#include <GL/gl.h>
#include <SDL/SDL.h>

#include "../../../../Global/IPluginRegistryListener.h"
#include "../../../../Global/IPluginSupport.h"
#include "../../../../Global/PluginRegistry.h"
#include "../../../../Global/PlugSocket.h"
#include "../../../../Global/RectangleComponent.h"

#include "Button.h"
#include "ChoosePluginImplementationCommand.h"
#include "PluginRequirementsComponent.h"

/**
 * This component allows the user to select an instance of a plugin type.
 * It also allows the user to invoke the component for creating a new plugin
 * instance.
 */
class ChoosePluginInstanceComponent:public RectangleComponent,
                                    public IPluginRegistryListener {
  private:
  static IFont* cFont;

  PluginRegistry* cPluginRegistry;
  IRectangularComponent* cFocusedComponent;
  IComponentContainer* cTopContainer;

  ICommand* cOKCommand;
  ICommand* cCancelCommand;
  ICommand* cConfigureCommand;

  IPluginSupport* cPluginSupport;

  /**
   * The socket for which we are selecting a plug-in instance.
   */
  PlugSocket* cPlugSocket;
  std::vector<std::string> cPluginInstances;
  unsigned int cSelectedInstance;

  Button* cNewInstanceButton;
  Button* cOKButton;
  Button* cCancelButton;
  Button* cConfigureButton;

  bool keyDown(SDLKey&);
  void makeSelection();
  void configureSelectedPlugin();

  class OKCommand:public ICommand {
    private:
    ChoosePluginInstanceComponent* cParent;

    public:
    OKCommand(ChoosePluginInstanceComponent*);

    /************************************************************************\
     * Implemented methods of ICommand.h                                    *
    \************************************************************************/
    void execute();
  };

  class CancelCommand:public ICommand {
    private:
    ChoosePluginInstanceComponent* cParent;

    public:
    CancelCommand(ChoosePluginInstanceComponent*);

    /************************************************************************\
     * Implemented methods of ICommand.h                                    *
    \************************************************************************/
    void execute();
  };

  class ConfigurePluginCommand:public ICommand {
    private:
    ChoosePluginInstanceComponent* cParent;

    public:
    ConfigurePluginCommand(ChoosePluginInstanceComponent*);

    /************************************************************************\
     * Implemented methods of ICommand.h                                    *
    \************************************************************************/
    void execute();
  };

  public:
  void static setFont(IFont*);

  /**
   * Construct the component to choose an instance of the specified plugin type.
   * 
   * @param IComponentContainer*
   * @param IPluginSupport*
   * @param string  The plugin type to choose an instance of.
   * @param float
   * @param float
   */
  ChoosePluginInstanceComponent(IComponentContainer*, PluginRegistry*, IPluginSupport*, PlugSocket*, float, float);

  /**************************************************************************\
   * Implemented methods of RectangleComponent.h                            *
  \**************************************************************************/
  void updateContent(int);
  void renderContent();
  bool inputContent(SDL_Event&);

  /**************************************************************************\
   * Implemented methods of IPluginRegistryListener.h                        *
  \**************************************************************************/
  void pluginInstanceAdded(PluginRegistry*, std::string, std::string);
  void pluginInstanceRemoved(PluginRegistry*, std::string, std::string);
};

#endif
