/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SequenceTrackScreenInstance.h"

#include "SequenceTrackScreen.h"

namespace IsoRealms::Basics {
  SequenceTrackScreenInstance::SequenceTrackScreenInstance(SequenceTrackScreen& parent, SequenceInstance& sequenceInstance) :
            SequenceTrackInstanceBase(parent.getRealEvents()),
            cParent(parent),
            cRuntimeScreen(nullptr) {
  }

  void SequenceTrackScreenInstance::positionChanged() {
    updateScreen();
  }

  void SequenceTrackScreenInstance::registerAssets(ComponentAssetRegistry& assets, const std::string& parentID) {
    assets.add<IScreen>(this, parentID + "/" + cParent.getName(), "Sequences");
  }

  void SequenceTrackScreenInstance::renderScreen(float scale, float aspectRatio) const {
    IScreen* mScreen = getCurrentScreen();
    if (mScreen != nullptr) {
      mScreen->renderScreen(scale, aspectRatio);
    }
  }

  const IFloat* SequenceTrackScreenInstance::getYaw() const {
    IScreen* mScreen = getCurrentScreen();
    if (mScreen != nullptr) {
      return mScreen->getYaw();
    }
    return nullptr;
  }

  const IFloat* SequenceTrackScreenInstance::getPitch() const {
    IScreen* mScreen = getCurrentScreen();
    if (mScreen != nullptr) {
      return mScreen->getPitch();
    }
    return nullptr;
  }

  bool SequenceTrackScreenInstance::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackScreenInstance::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void SequenceTrackScreenInstance::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool SequenceTrackScreenInstance::isDefaultConfiguration() const {
    return true;
  }

  void SequenceTrackScreenInstance::updateScreen() {
    cRuntimeScreen = getCurrentScreen();
  }

  IScreen* SequenceTrackScreenInstance::getCurrentScreen() const {
    unsigned int mRuntimeEvent = getRuntimeEvent();
    if (mRuntimeEvent > 0) {
      return cParent.getRealEvents()[mRuntimeEvent - 1]->getScreen();
    }
    return *cParent.getStartScreen();
  }
}

