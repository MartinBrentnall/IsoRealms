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
#include "StateTileSet.h"

unsigned int StateTileSet::getContactTransition(unsigned int state) {
  std::map<unsigned int, TargetState*>::iterator i = cContactTransitions.find(state);
  return i != cContactTransitions.end() ? i->second->getState() : state;
}

StateTileStateController* StateTileSet::getStateController() {
  return NULL; // TODO: What to do here?
}

std::vector<IElementFactory*> StateTileSet::getElementFactories() {
  return cElementFactories;
}

void StateTileSet::destroy(IElement* element) {
  delete element;
}

void StateTileSet::setEditingContext(BlockLocation* blockLocation, IElementGateway* elementGateway, IComponentContainer* componentContainer, ICommandRegistry* commandRegistry) {
  // TODO: Do whatever needs doing
}

extern "C" IElementSet* create(DOMNodeWrapper* node) {
  return new StateTileSet();
}

extern "C" void destroy(IElementSet* elementSet) {
  delete elementSet;
}
