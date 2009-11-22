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

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <vector>
#include <string>

#include "../../../../Global/IComponentContainer.h"
#include "../../../../Global/IFont.h"
#include "../../../../Global/IPlugin.h"
#include "../../../../Global/IPluginSupport.h"
#include "../../../../Global/PluginRegistry.h"
#include "../../../../Global/RectangleComponent.h"
#include "../../../../Global/System.h"

#include "ChoosePluginInstanceComponent.h"

// TODO: Overall, class is too hackish.  Has too much in common with ManageLibrariesComponent
class PluginRequirementsComponent:public RectangleComponent {
  private:
  static IFont* cFont;
  unsigned int cSelectedPlugin;
  PluginRegistry* cPluginRegistry;

  /**
   * The plugin support to satisfy.
   */
  IPluginSupport* cPluginSupport;

  /**
   * Plugin supported by the element set.
   */
  std::vector<PlugSocket*> cSupportedPlugins;
  IHUDComponent* cChooseInstanceComponent;
  IComponentContainer* cComponentContainer;

  bool keyDown(SDLKey&);

  public:
  void static setFont(IFont*);

  PluginRequirementsComponent(IComponentContainer*, PluginRegistry*, IPluginSupport*, float, float);

  /**************************************************************************\
   * Implemented methods of IHUDComponent.h                                 *
  \**************************************************************************/
  void updateContent(int);
  void renderContent();
  bool inputContent(SDL_Event&);

  /**************************************************************************\
   * Implemented methods of IItemSelectedCommand.h                          *
  \**************************************************************************/
  void itemSelected(std::string);
};

#endif
