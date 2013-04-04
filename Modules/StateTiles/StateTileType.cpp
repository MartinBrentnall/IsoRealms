/*
 * Copyright 2011 Martin Brentnall
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
#include "StateTileType.h"

void StateTileType::addToggleCommand(unsigned int onState, unsigned int offState) {
  cCommands.push_back(new ToggleState(this, onState, offState));
}
  
bool StateTileType::canChange(unsigned int state) {
  for (unsigned int i = 0; i < cCommands.size(); i++) {
    if (cCommands[i]->cOnState == state || cCommands[i]->cOffState == state) {
      return true;
    }
  }
  return false;
}
  
void StateTileType::zoneContextChanged(IZone* zone) {
  cCurrentZone = zone;
}

StateTileType::ToggleState::ToggleState(StateTileType* parent, unsigned int onState, unsigned int offState) {
  if (cOnState == cOffState) {
    // TODO: Throw
  }
  cParent = parent;
  cOnState = onState;
  cOffState = offState;
}
    
void StateTileType::ToggleState::execute() {
  StateTileStateController* mStateController = cParent->cStateControllers[cParent->cCurrentZone];
  mStateController->toggleStates(cOnState, cOffState);
}

