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
#ifndef CHOOSE_ELEMENTS_COMMAND_H
#define CHOOSE_ELEMENTS_COMMAND_H

#include <vector>

#include <IsoRealms/ElementSetRegistry.h>
#include <IsoRealms/ICommand.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/IHUDComponent.h>

#include "ChooseElementsComponent.h"
#include "IElementSelectionListener.h"

class ChooseElementsCommand:public ICommand {
  private:
  ElementSetRegistry* cElementSetRegistry;
  IComponentContainer* cComponentContainer;
  IElementSelectionListener* cElementSelectionListener;

  public:
  ChooseElementsCommand(IComponentContainer*, IElementSelectionListener*, ElementSetRegistry*);

  /*************************************************************************\
   * Implemented methods of ICommand.h                                     *
  \*************************************************************************/
  void execute();  
};

#endif
