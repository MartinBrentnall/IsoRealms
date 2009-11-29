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

#include <vector>

#include "DOMNodeWriter.h"
#include "ICommandInfo.h"
#include "IComponentContainer.h"
#include "IPluginSupport.h"

class Zone;

class IPlugin:public IPluginSupport {
  public:
  virtual ~IPlugin() {}

  /**
   * This allows the plugin to know that an editor action is being performed on
   * the specified zone.  It will be called immediately prior to the editing
   * action being carried out.
   */
  virtual void notifyZoneAction(Zone*) = 0;

  /**
   * This allows the plugin to know that initialization is being performed for
   * the specified zone.  It may be called multiple times depending on how many
   * initialization passes are required for the zone.
   */
  virtual void initPlugin(Zone*) = 0;

  virtual void setEditingInfo(IComponentContainer*) = 0;

  /**
   * Return the command info associated with this plugin.
   */
  virtual std::vector<ICommandInfo*> getCommandInfo() = 0;

  /**
   * Save the configuration of the plug-in.
   */
  virtual void save(DOMNodeWriter*) = 0;

  /**
   * Load the configuration of the plug-in.
   */
  virtual void load(DOMNodeWrapper*) = 0;
};

#endif
