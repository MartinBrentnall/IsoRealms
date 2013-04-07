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
#include "IComponentContainer.h"
#include "IDynamicElement.h"
#include "IEditingContext.h"
#include "IInteractiveElement.h"
#include "IPluginRegistry.h"
#include "IResourceSource.h"
#include "IVisualElement.h"
#include "IZone.h"
#include "Persistence/DOMNodeWriter.h"
#include "Resources/IResourceManager.h"
#include "Resources/IRuntimeContext.h"

class IMap;
class IZoneHandler;

class IPlugin:public IResourceSource {
  private:
  IPluginRegistry* cPluginRegistry;
  
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
  virtual void setEditingContext(IEditingContext*, IResourceManager*);

  /**
   * Save the configuration of the plug-in.
   */
  virtual void save(DOMNodeWriter*, IResourceLocator*);

  /**
   * Write plugin data for the specified zone.
   */
  virtual void saveData(DOMNodeWriter*, IMap*, IZone*);

  /**
   * Load plugin data for the specified zone.
   */
  virtual void loadData(DOMNodeWrapper*, IPluginRegistry*, IZone*);
  
  virtual void initZone(IZone*);
  
  /**
   * Retrieve the zone renderer of the specified name.
   */
  virtual IZoneHandler* getZoneRenderer(const std::string&);
  
  // TODO: I'm not sure I like this being here
  void setPluginRegistry(IPluginRegistry*);

  /**
   * The plug-in should clean up any resources that it allocated itself.  It
   * can be assumed that resources created by the plug-in are no longer in use
   * by the time this destructor is reached.
   */
  virtual ~IPlugin() {}
};

typedef IPlugin* createPlugin();
typedef void destroyPlugin(IPlugin*);

#endif
