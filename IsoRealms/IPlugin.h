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
#ifndef I_PLUGIN_H
#define I_PLUGIN_H

#include <SDL/SDL.h>
#include <vector>

#include "BlockLocation.h"
#include "DOMNodeWriter.h"
#include "ICommandRegistry.h"
#include "IComponentContainer.h"
#include "ICommandInfo.h"
#include "IDynamicElement.h"
#include "IInteractiveElement.h"
#include "IPluginSupport.h"
#include "IZone.h"
#include "IZoneRenderer.h"

class IPluginRegistry;
class IMap;

class IPlugin:public IPluginSupport {
  private:
  static std::vector<ICommandInfo*> cNoCommands;
  static std::vector<IDynamicElement*> cNoDynamicElements;
  static std::vector<IInteractiveElement*> cNoInteractiveElements;

  public:

  /**
   * This allows the plugin to know that an editor action is being performed on
   * the specified zone.  It will be called immediately prior to the editing
   * action being carried out.
   */
  virtual void notifyZoneAction(IZone*);

  /**
   * This allows the plugin to know that initialization is being performed for
   * the specified zone.  It may be called multiple times depending on how many
   * initialization passes are required for the zone.
   */
  virtual void initPlugin(IZone*, unsigned int);

  /**
   * Retrieve commands to execute from this plugin before entering a game loop.
   * This function is useful for plugins that perform tasks such as camera
   * positioning and background rendering.
   * 
   * @returns  Commands to execute before entering game loop.
   */
  virtual std::vector<IDynamicElement*> getPreLoopCommands();

  /**
   * Retrieve commands to execute from this plugin before entering a game loop.
   * This function is useful for plugins that perform tasks such as foreground
   * rendering (e.g. score display, etc.)
   * 
   * @returns  Commands to execute before entering game loop.
   */
  virtual std::vector<IDynamicElement*> getPostLoopCommands();

  /**
   * TODO
   */
  virtual std::vector<IInteractiveElement*> getInteractiveElements();

  /**
   * This function is called immediately before the content of the specified
   * zone is rendered.
   * 
   * @param Zone*  The zone thaw will be rendered after this.
   */
  virtual void renderPreZone(IZone*);

  /**
   * This function is called when the zone context changes.
   */
  virtual void zoneContextChanged(IMap*, IZone*);

  /**
   * Set the editing context.
   */
  virtual void setEditingContext(BlockLocation*, IComponentContainer*, ICommandRegistry*);

  /**
   * Return the command info associated with this plugin.
   */
  virtual std::vector<ICommandInfo*> getCommandInfo();

  /**
   * Save the configuration of the plug-in.
   */
  virtual void save(DOMNodeWriter*);

  /**
   * Write plugin data for the specified zone.
   */
  virtual void saveData(DOMNodeWriter*, IMap*, IZone*);

  /**
   * Load the configuration of the plug-in.
   */
  virtual void load(DOMNodeWrapper*);

  /**
   * Load plugin data for the specified zone.
   */
  virtual void loadData(DOMNodeWrapper*, IPluginRegistry*, IZone*);

  /**
   * Retrieve the zone renderer of the specified name.
   */
  virtual IZoneRenderer* getZoneRenderer(const std::string&);
  
  /**
   * The plug-in should clean up any resources that it allocated itself.  It
   * can be assumed that resources created by the plug-in are no longer in use
   * by the time this destructor is reached.
   */
  virtual ~IPlugin() {}
};

typedef IPlugin* createPlugin(DOMNodeWrapper*);
typedef void destroyPlugin(IPlugin*);

#endif
