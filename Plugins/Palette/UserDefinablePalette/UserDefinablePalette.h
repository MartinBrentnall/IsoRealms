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
#ifndef USER_DEFINABLE_PALETTE_H
#define USER_DEFINABLE_PALETTE_H

#include <map>
#include <vector>

#include <IsoRealms/DefaultCommandInfo.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/ICommandInfo.h>
#include <IsoRealms/IComponentSource.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IPalette.h>
#include <IsoRealms/IPlugin.h>
#include <IsoRealms/OpenDialogCommand.h>

#include "PaletteConfigurationComponent.h"

class UserDefinablePalette:public IPlugin,
                           public IComponentSource {
  private:
  static Colour* DEFAULT_COLOUR; // TODO: Should be const?
    
  IRuntimeContext* cRuntimeContext;
  std::map<std::string, Colour*> cPalette;
  std::vector<IPaletteListener*> cChangeListeners;
  IComponentContainer* cComponentContainer;

  public:
  UserDefinablePalette(IRuntimeContext*);

  /***********************\
   * Implements IPalette *
  \***********************/
  Colour* getColour(const std::string&);
  void addChangeListener(IPaletteListener*);
  void removeChangeListener(IPaletteListener*);

  /**********************\
   * Implements IPlugin *
  \**********************/
  void setEditingContext(IEditingContext*);
  void save(DOMNodeWriter*);
  void load(DOMNodeWrapper*);

  /*******************************\
   * Implements IComponentSource *
  \*******************************/
  IHUDComponent* createComponent();
  
  ~UserDefinablePalette();
};

#endif
