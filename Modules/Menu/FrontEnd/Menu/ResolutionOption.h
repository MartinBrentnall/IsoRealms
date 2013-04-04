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
#ifndef RESOLUTION_OPTION_H
#define RESOLUTION_OPTION_H

#include <string>

#include <IsoRealms/ScreenConfiguration.h>

#include "IOption.h"

class ResolutionOption:public IOption {
  private:
  ScreenConfiguration* cScreenConfiguration;
  ScreenMode* cScreenMode;

  public:
  ResolutionOption(ScreenConfiguration*, ScreenMode*);

  /**********************\
   * Implements IOption *
  \**********************/
  std::string getOption();
  void apply();
};

#endif
