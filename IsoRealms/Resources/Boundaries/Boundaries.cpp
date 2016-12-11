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
#include "Boundaries.h"

Boundaries::Boundaries() {
  cAccessMutex = SDL_CreateMutex();
}

void Boundaries::initialiseResource(DOMNodeWrapper*, DOMNodeWrapper*, IResourceAccessor*) {
  // Nothing to do
}

Icon<IBoundaries>* Boundaries::getResourceIcon(IResourceBrowser<IBoundaries>* browser) {
  return nullptr;
}

void Boundaries::registerBoundary(IBoundary* boundary) {
  while (SDL_mutexP(cAccessMutex) == -1);
  cBoundaries.add(boundary);
  SDL_mutexV(cAccessMutex);
}

void Boundaries::notifyAppearance(IBoundaryPenetrator* boundaryPenetrator, Vertex& location, BoundaryHandler* boundaryHandler) {
  while (SDL_mutexP(cAccessMutex) == -1);
  std::vector<IBoundary*> mBoundaries = cBoundaries.getElements(location.getX(), location.getY());
  SDL_mutexV(cAccessMutex);
  for (int i = mBoundaries.size() - 1; i >= 0; i--) {
    if (mBoundaries[i]->contains(location)) {
      boundaryPenetrator->entered(mBoundaries[i]);
      mBoundaries[i]->setArguments();
      boundaryPenetrator->setArguments();
      boundaryHandler->entered();
      mBoundaries[i]->unsetArguments();
      boundaryPenetrator->unsetArguments();
    }
  }
}

void Boundaries::notifyDisappearance(IBoundaryPenetrator* boundaryPenetrator, Vertex& location, BoundaryHandler* boundaryHandler) {
  while (SDL_mutexP(cAccessMutex) == -1);
  std::vector<IBoundary*> mBoundaries = cBoundaries.getElements(location.getX(), location.getY());
  SDL_mutexV(cAccessMutex);
  for (int i = mBoundaries.size() - 1; i >= 0; i--) {
    if (mBoundaries[i]->contains(location)) {
      boundaryPenetrator->entered(mBoundaries[i]);
      mBoundaries[i]->setArguments();
      boundaryPenetrator->setArguments();
      boundaryHandler->exited();
      mBoundaries[i]->unsetArguments();
      boundaryPenetrator->unsetArguments();
    }
  }
}

void Boundaries::notifyMovement(IBoundaryPenetrator* boundaryPenetrator, Vertex& start, Vertex& end, BoundaryHandler* boundaryHandler) {
  int mSouth = std::floor(std::min(start.getY(), end.getY())) - 1;
  int mNorth = std::ceil(std::max(start.getY(), end.getY())) + 1;
  int mWest  = std::floor(std::min(start.getX(), end.getX())) - 1;
  int mEast  = std::ceil(std::max(start.getX(), end.getX())) + 1;
  while (SDL_mutexP(cAccessMutex) == -1);
  std::vector<IBoundary*> mBoundaries = cBoundaries.getElements(mSouth, mNorth, mWest, mEast);
  SDL_mutexV(cAccessMutex);
  for (int i = mBoundaries.size() - 1; i >= 0; i--) {
    if (mBoundaries[i]->isEntered(start, end)) {
      boundaryPenetrator->entered(mBoundaries[i]);
      mBoundaries[i]->setArguments();
      boundaryPenetrator->setArguments();
      boundaryHandler->entered();
      mBoundaries[i]->unsetArguments();
      boundaryPenetrator->unsetArguments();
    }
    if (mBoundaries[i]->isExited(start, end)) {
      mBoundaries[i]->setArguments();
      boundaryPenetrator->setArguments();
      boundaryHandler->exited();
      mBoundaries[i]->unsetArguments();
      boundaryPenetrator->unsetArguments();
    }
  }
}

void Boundaries::reinitialise() {
//   for (std::map<IZone*, std::vector<ICollectable*>*>::iterator i = cBoundaries.begin(); i != cBoundaries.end(); ++i) {
//     for (unsigned int j = 0; j < i->second->size(); j++) {
//       (*i->second)[j]->setDirty();
//     }
//     delete i->second;
//   }
//   cBoundaries.clear();
}

// bool ZoneCollectables::isMet() {
//   return cCollectedCount == cCollectablesCount;
// }

std::string Boundaries::getPath(IArgumentValue* value) {
//   return value == &cArgumentZoneRemaining ? "zoneRemaining"
//        : value == &cArgumentZone          ? "zone"
//        :                                    "";
  return "";
}

void Boundaries::connectArguments(std::vector<IArgumentValueCollection*> argumentValueCollection, const std::string& entity) {
  for (std::map<std::string, ArgumentValueProxy*>::iterator i = cArgumentValues.begin(); i != cArgumentValues.end(); i++) {
    std::size_t mPointerLocation = i->first.find("->");
    if (mPointerLocation != std::string::npos) {
      std::string mEntity = i->first.substr(0, mPointerLocation);
      std::string mPath = i->first.substr(mPointerLocation + 2);
      if (mEntity == entity) {
        for (unsigned int j = 0; j < argumentValueCollection.size(); j++) {
          IArgumentValue* mArgumentValue = argumentValueCollection[j]->getArgumentValue(mPath);
          if (mArgumentValue != nullptr) {
            i->second->setArgumentValue(mArgumentValue);
          }
        }
      }
    }
  }
}

void Boundaries::registerArgumentValuesBoundaries(IArgumentValueCollection* argumentValueRegistry) {
  cArgumentValuesBondaries.push_back(argumentValueRegistry);
  connectArguments(cArgumentValuesBondaries, "boundary");
}

void Boundaries::registerArgumentValuesBoundaryPenetrator(IArgumentValueCollection* argumentValueRegistry) {
  cArgumentValuesBoundaryPenetrators.push_back(argumentValueRegistry);
  connectArguments(cArgumentValuesBoundaryPenetrators, "boundaryPenetrator");
}

ArgumentValueProxy* Boundaries::getArgumentValue(const std::string& path) {
  std::map<std::string, ArgumentValueProxy*>::iterator i = cArgumentValues.find(path);
  if (i == cArgumentValues.end()) {
    cArgumentValues[path] = new ArgumentValueProxy();
    connectArguments(cArgumentValuesBondaries, "boundary");
    connectArguments(cArgumentValuesBoundaryPenetrators, "boundaryPenetrator");
  }
  return cArgumentValues[path];
}

IArgumentValue* Boundaries::getArgumentValue(DOMNodeWrapper* node) {
  std::string mType = node->getAttribute("type");
  std::string mValue = node->getAttribute("value").substr(1);
  IArgumentValue* mArgumentValue = getArgumentValue(mValue);
  std::cout << "  ==================================================================== Obtained argument " << mArgumentValue << std::endl;
  return mArgumentValue;
}
