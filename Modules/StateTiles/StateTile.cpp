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
#include "StateTile.h"

void StateTile::notifyContact() {
  IStateTileSet* mTileSet = getElementSet();
  unsigned int mNewState = mTileSet->getContactTransition(*cState);
  StateTileStateController* mStateController = mTileSet->getStateController();
  IStateTileType* mTileType = getElementType();
  mTileType->notifyContact(cState);
  cTheme->executeContactScript();
  mTileType->executeContactScript();
  cState = mStateController->getStateAddress(mNewState);
}
  
std::vector<IVisualElement*> StateTile::getVisualElements() {
  std::vector<IVisualElement*> mVisualElements;
  IStateTileType* mType = getElementType();
  if (mType->canChange(*cState)) {
    mVisualElements.push_back(this);
  }
  return mVisualElements;
}
  
void StateTile::renderStatic() {
  IStateTileType* mType = getElementType();
  if (!mType->canChange(*cState)) {
    render();
  }
}
  
void StateTile::render() {
//  if (*cState != INVISIBLE) {
    // TODO: Render the scene.
//  }
}
