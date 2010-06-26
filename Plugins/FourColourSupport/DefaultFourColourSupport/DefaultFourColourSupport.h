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
#ifndef DEFAULT_FOUR_COLOUR_SUPPORT_H
#define DEFAULT_FOUR_COLOUR_SUPPORT_H

#include <map>
#include <vector>

#include <IsoRealms/ICommand.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IPlugin.h>

#include "../IFourColourSupport.h"

#include "PaletteConfigurationComponent.h"
#include "PaletteConfigurationCommandInfo.h"

class DefaultFourColourSupport:public IFourColourSupport {
  private:

  /**
   * This command is executed to show the palette editor component.
   */
  class PaletteConfigurationCommand:public ICommand {
    private:
    DefaultFourColourSupport* cParent;
    IComponentContainer* cComponentContainer;

    public:
    PaletteConfigurationCommand(DefaultFourColourSupport*);
    void setComponentContainer(IComponentContainer*);

    /***********************\
     * Implements ICommand *
    \***********************/
    void execute();
  };

  std::vector<ICommandInfo*> cPluginCommands;
  std::map<PaletteEntry, Colour*> cPalette;
  std::vector<IFourColourSupportListener*> cChangeListeners;

  /**
   * Instance of the command to show the palette editor component.
   */
  PaletteConfigurationCommand* cPaletteConfigurationCommand;

  public:
  DefaultFourColourSupport();

  /*********************************\
   * Implements IFourColourSupport *
  \*********************************/
  Colour* getColour(PaletteEntry);
  void addChangeListener(IFourColourSupportListener*);
  void removeChangeListener(IFourColourSupportListener*);

  /*************************************************\
   * Implements IPluginSupport (in IPluginAccesor) *
  \*************************************************/
  std::string getName();

  /**********************\
   * Implements IPlugin *
  \**********************/
  std::vector<ICommandInfo*> getCommandInfo();
  void setEditingInfo(IComponentContainer*);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);

  ~DefaultFourColourSupport();
};

#endif
