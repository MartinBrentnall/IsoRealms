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
 * GNU General Public Linpicense for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Iso-Realms.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef PALETTE_CONFIGURATION_COMPONENT_H
#define PALETTE_CONFIGURATION_COMPONENT_H

#include <map>
#include <vector>

#include <IsoRealms/Colour.h>
#include <IsoRealms/Configuration.h>
#include <IsoRealms/GUI/Dialog.h>
#include <IsoRealms/IComponentContainer.h>
#include <IsoRealms/ScreenConfiguration.h>

#include "../IFourColourSupport.h"
#include "../IFourColourSupportListener.h"

class PaletteConfigurationComponent:public Dialog {
  private:
  std::vector<IFourColourSupportListener*>* cChangeListeners;
  std::vector<Colour*> cPalette;
  std::vector<std::string> cPaletteEntries;
  unsigned int cSelectedField;
  unsigned int cSelectedEntry;

  void renderChannel(float, float, float, float, Colour&, Colour&, unsigned int);
  void adjustChannel(float);
  bool keyDown(SDLKey&);
  void fireChangeEvent();

  public:
  PaletteConfigurationComponent(IComponentContainer*, std::map<std::string, Colour*> palette, std::vector<IFourColourSupportListener*>&);

  /*********************\
   * Implements Dialog *
  \*********************/
  void renderContent();
  void updateContent(int);
  bool inputContent(SDL_Event&);
};

#endif
