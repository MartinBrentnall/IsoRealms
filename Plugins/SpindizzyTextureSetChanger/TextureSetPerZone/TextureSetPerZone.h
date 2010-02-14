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

#include "../../../Global/ICommand.h"
#include "../../../Global/IHUDComponent.h"
#include "../../../Global/IPluginRegistry.h"
#include "../../../Global/Map.h"

#include "../ISpindizzyTextureSetChanger.h"

#include "ChooseTextureSetCommandInfo.h"
#include "TextureSetChooserComponent.h"

class TextureSetPerZone:public ISpindizzyTextureSetChanger,
                        public IZoneTextureSetter {
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

  Map* cCurrentMap;
  Zone* cCurrentZone;
  ChooseTextureSetCommand* cChooseTextureSetCommand;
  std::vector<ICommandInfo*> cPluginCommands;
  IChangeableTextureSet* cControlledObject;
  std::vector<ISpindizzyTextureSet*> cTexturePalette;
  std::map<Zone*, ISpindizzyTextureSet*> cZoneMapping;

  public:
  TextureSetPerZone();

  /*********************************\
   * Implements IZoneTextureSetter *
  \*********************************/
  void setTextureSet(ISpindizzyTextureSet*);

  /******************************************\
   * Implements ISpindizzyTextureSetChanger *
  \******************************************/
  void setControlObject(IChangeableTextureSet*);

  /******************************************\
   * Implements IPluginSupport (in IPlugin) *
  \******************************************/
  std::string getName();
  std::vector<PlugSocket*> getPlugSockets();
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /******************************************************\
   * Implements IPlugin (in ISpindizzyTextureSetChanger *
  \******************************************************/
  void notifyZoneAction(Zone*);
  void initPlugin(Zone*);
  void renderPreZone(Zone*);
  void zoneContextChanged(Map*, Zone*);
  std::vector<ICommandInfo*> getCommandInfo();
  void setEditingInfo(IComponentContainer*);
  void save(DOMNodeWriter*);
  void saveData(DOMNodeWriter*, Map*, Zone*);
  void loadData(DOMNodeWrapper*, IPluginRegistry*, Zone*);
  void load(DOMNodeWrapper*);
};

#endif
