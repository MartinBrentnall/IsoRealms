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
#ifndef TEXTURE_SET_PER_ZONE_H
#define TEXTURE_SET_PER_ZONE_H

#include <map>
#include <sstream>
#include <vector>

#include <IsoRealms/ICommand.h>
#include <IsoRealms/IHUDComponent.h>
#include <IsoRealms/IPluginRegistry.h>
#include <IsoRealms/Map.h>

#include "../../ZoneContext/IZoneContext.h"
#include "../../ZoneContext/IZoneContextListener.h"

#include "../ITextureSetChanger.h"

#include "ChooseTextureSetCommandInfo.h"
#include "TextureSetChooserComponent.h"

class TextureSetPerZone:public ITextureSetChanger,
                        public IZoneTextureSetter,
                        public IZoneContextListener,
                        public IDynamicElement {
  private:
  class ChooseTextureSetCommand:public ICommand {
    private:
    TextureSetPerZone* cParent;
    IComponentContainer* cComponentContainer;

    public:
    ChooseTextureSetCommand(TextureSetPerZone*);
    void setComponentContainer(IComponentContainer*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };
  
  class DefaultTextureSetCommand:public IDynamicElement {
    private:
    TextureSetPerZone* cParent;
    
    public:
    DefaultTextureSetCommand(TextureSetPerZone*);
    
    /******************************\
     * Implements IDynamicElement *
    \******************************/
    void update(int);
  };

  DefaultTextureSetCommand* cDefaultTextureSetCommand;
  IMap* cCurrentMap;
  IZone* cCurrentZone;
  BlockLocation* cBlockLocation;
  ChooseTextureSetCommand* cChooseTextureSetCommand;
  std::vector<ICommandInfo*> cPluginCommands;
  std::vector<IChangeableTextureSet*> cControlledObjects;
  std::vector<ITextureSet*> cTexturePalette;
  std::map<IZone*, ITextureSet*> cZoneMapping;
  IZoneContext* cZoneContext;
  Colour cPreviousBackgroundColour;
  Colour cTargetBackgroundColour;
  float cProgressBackgroundColour;

  public:
  TextureSetPerZone();

  /*********************************\
   * Implements IZoneTextureSetter *
  \*********************************/
  void setTextureSet(ITextureSet*);

  /***********************************\
   * Implements IZoneContextListener *
  \***********************************/
  void zoneContextChanged(IZone*);

  /******************************\
   * Implements IDynamicElement *
  \******************************/
  void update(int);

  /*********************************\
   * Implements ITextureSetChanger *
  \*********************************/
  void addControlObject(IChangeableTextureSet*);
  void removeControlObject(IChangeableTextureSet*);

  /******************************************\
   * Implements IPluginSupport (in IPlugin) *
  \******************************************/
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /*********************************************\
   * Implements IPlugin (in ITextureSetChanger *
  \*********************************************/
  void renderPreZone(IZone*);
  void zoneContextChanged(IMap*, IZone*);
  std::vector<ICommandInfo*> getCommandInfo();
  void setEditingContext(BlockLocation*, IComponentContainer*);
  void saveData(DOMNodeWriter*, IMap*, IZone*);
  void loadData(DOMNodeWrapper*, IPluginRegistry*, IZone*);
  std::vector<IDynamicElement*> getPreLoopCommands();
  std::vector<IDynamicElement*> getPostLoopCommands();
};

#endif
