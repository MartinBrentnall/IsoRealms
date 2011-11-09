/*
 * Copyright 2009,2010 Martin Brentnall
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
#ifndef SWITCH_H
#define SWITCH_H

#include <string>
#include <vector>

#include <IsoRealms/DOMNodeWrapper.h>
#include <IsoRealms/Script.h>

#include "../../3DModel/ISimpleModel.h"

class Switch {
  private:
  std::string cName;
  ISimpleModel* cHUDRepresentation;
  Script* cOnScript;
  Script* cOffScript;

  public:
  Switch(const std::string&);
  Switch(const std::string&, Script*, Script*);

  std::string getName();
  void switchOn();
  void switchOff();
};

#endif