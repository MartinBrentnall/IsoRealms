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
#ifndef INPUT_COMMANDS_H
#define INPUT_COMMANDS_H

#include <map>
#include <vector>

#include "BlockLocation.h"
#include "DigitalInput.h"
#include "IInteractiveElement.h"
#include "IComponentContainer.h"
#include "IResources.h"
#include "System.h"

class InputCommands {
  private:
  std::map<std::string, DigitalInput*> cDigitalInputs;
  
  DigitalInput* getDigitalInput(const std::string&);
  DOMNodeWrapper* findConfiguration(std::vector<DOMNodeWrapper*>, const std::string&);

  public:
  InputCommands();
  InputCommands(const std::string&);
  void loadConfiguration(DOMNodeWrapper*, std::vector<std::string>, IResources*);
  void save(DOMNodeWriter*);
  bool* registerDigitalInput(const std::string&);
  bool input(SDL_Event&);
  std::vector<std::string> getInputNames();
  DigitalInput* findDigitalInput(const std::string&);
};

#endif
