/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#ifndef SPINDIZZY_SWITCHES_H
#define SPINDIZZY_SWITCHES_H

#include <map>

#include <IsoRealms/ICommandRegistry.h>
#include <IsoRealms/IUserCommand.h>

#include "../../3DModel/ISimpleModelFactory.h"
#include "../../Camera/ICamera.h"
#include "../../HUD/IHUD.h"
#include "../../HUD/IHUDComponentFactory.h"

#include "../IUtilities.h"

#include "HUDComponent.h"
#include "Switch.h"

class SpindizzySwitches:public IUtilities,
                        public IHUDComponentFactory {
  private:
  class ResetCommand:public IUserCommand {
    private:
    SpindizzySwitches* cParent;
    Script* cResetScript;

    public:
    ResetCommand(SpindizzySwitches*);
    void setScript(Script*);

    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getName();
  };

  class SwitchCommand:public IUserCommand {
    private:
    SpindizzySwitches* cParent;
    ISimpleModel* cHUDModel;
    Switch* cSwitch;
    bool cPrimary;

    public:
    SwitchCommand(SpindizzySwitches*, Switch*, bool);
    SwitchCommand(SpindizzySwitches*, DOMNodeWrapper*);
    void deactivate();
    ISimpleModel* getModel();

    /***************************\
     * Implements IUserCommand *
    \***************************/
    void execute();
    std::string getName();
  };

  IRuntimeContext* cRuntimeContext;
  IHUD* cHUD;
  ICamera* cCamera;
  HUDComponent cHUDSwitchA;
  HUDComponent cHUDSwitchB;
  Vertex cDefaultVertex;
  std::vector<ISimpleModelFactory*> cHUDModels;
  std::vector<SwitchCommand*> cSwitchCommands;
  std::vector<PlugSocket*> cCommandRegistrySocket;
  ResetCommand* cResetCommand;
  SwitchCommand* cActiveSwitchA;
  SwitchCommand* cActiveSwitchB;
  SwitchCommand** cNextSwitch;

  void updateHUD();
  
  public:
  SpindizzySwitches(IRuntimeContext*);

  void addSwitch(const std::string&, bool);

  /***********************************\
   * Implements IHUDComponentFactory *
  \***********************************/
  IHUDGameComponent* getHUDComponent(const std::string&);
  
  /*****************************\
   * Implements IPluginSupport *
  \*****************************/
  void setPlugin(PlugSocket*, IPlugin*);
  IPlugin* getPlugin(PlugSocket*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void load(DOMNodeWrapper*);
  void save(DOMNodeWriter*);
};

#endif
