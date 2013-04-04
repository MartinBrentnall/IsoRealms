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
#include "TimeLimit.h"

TimeLimit::TimeLimit(ILock* lock) {
  cLock = lock;
  cMilliseconds = 0;
}

void TimeLimit::initialiseResource(DOMNodeWrapper* node, IResourceAccessor* resources) {
  cMilliseconds = node->getIntegerAttribute("value");
}

void TimeLimit::setValue(int value) {
  cMilliseconds = value;  
}

int TimeLimit::getValue() {
  return cMilliseconds;
}

std::vector<IDynamicElement*> TimeLimit::getPreLoopCommands() {
  std::vector<IDynamicElement*> mTimeCommand;
  mTimeCommand.push_back(this);
  return mTimeCommand;
}

void TimeLimit::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  node->addAttribute("value", cMilliseconds);
}

void TimeLimit::update(unsigned int milliseconds) {
  if (!cLock->isLocked()) {
    if (cMilliseconds > 0) {
      cMilliseconds -= milliseconds;
      if (cMilliseconds <= 0) {
        for (unsigned int i = 0; i < cTimeOutCommands.size(); i++) {
          cTimeOutCommands[i]->execute();
        }
        cMilliseconds = 0;
      }
    }
  }
}

