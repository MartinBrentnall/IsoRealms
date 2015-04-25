/*
 * Copyright 2015 Martin Brentnall
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
#ifndef DIALOG_COLOUR_FIXED_H
#define DIALOG_COLOUR_FIXED_H

#include <IsoRealms/GUI/Dialogs/ComponentColourSelector.h>
#include <IsoRealms/GUI/Dialogs/DialogOKCancelUndo.h>
#include <IsoRealms/IConfirmationListener.h>
#include <IsoRealms/IEditingContext.h>
#include <IsoRealms/Resources/Colour/Colour.h>
#include <IsoRealms/Resources/IResourceUseListener.h>

class DialogColourFixed:public DialogOKCancelUndo {
  private:
  ComponentColourSelector* cColourSelector;
  Colour* cColour;
  float cRed;
  float cGreen;
  float cBlue;
  float cAlpha;

  public:
  DialogColourFixed(IEditingContext*, Colour*, IResourceAccessor*, const std::string&);
  
  Colour* getResource();
  
  void undo();
};

#endif
