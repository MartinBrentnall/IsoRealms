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
#ifndef FOUR_COLOUR_SUPPORT_DUMMY_H
#define FOUR_COLOUR_SUPPORT_DUMMY_H

#include <map>

#include "../../Global/Colour.h"
#include "../../Global/DummyPlugin.h"

#include "IFourColourSupport.h"

/**
 * This dummy implementation uses four fixed grayscales as the palette entries.
 */
class FourColourSupportDummy:public IFourColourSupport,
                             public DummyPlugin {
  private:
  std::map<PaletteEntry, Colour*> cColours;

  public:
  FourColourSupportDummy();

  /*********************************\
   * Implements IFourColourSupport *
  \*********************************/
  Colour* getColour(PaletteEntry);
  void addChangeListener(IFourColourSupportListener*);
  void removeChangeListener(IFourColourSupportListener*);

  ~FourColourSupportDummy();
};

#endif
