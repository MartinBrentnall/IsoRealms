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
#include "SequenceTrackColourInstance.h"

#include "Modules/Basics/Basics.h"

#include "SequenceTrackColour.h"

namespace IsoRealms::Basics {
  SequenceTrackColourInstance::SequenceTrackColourInstance(SequenceTrackColour& parent, SequenceInstance& sequenceInstance) :
            SequenceTrackInstanceBase(parent.getRealEvents()),
            cParent(parent),
            cRuntimeColour(),
            cStateNotifier(nullptr) {
  }

  void SequenceTrackColourInstance::positionChanged() {
    updateColour();
  }

  void SequenceTrackColourInstance::stateChanged(IColour* colour) {
    const IColour* mUsedColour = getPreviousColour();
    if (colour == mUsedColour) {
      updateColour();
      return;
    }

    unsigned int mRuntimeEvent = getRuntimeEvent();
    int mRuntimeEventPosition = getRuntimeEventPosition();
    if (mRuntimeEvent < cParent.getRealEvents().size() && (mRuntimeEvent > 0 || mRuntimeEventPosition > 0)) {
      mRuntimeEvent = mRuntimeEventPosition == 0 ? mRuntimeEvent - 1 : mRuntimeEvent;
      mUsedColour = cParent.getRealEvents()[mRuntimeEvent]->getColour();
      if (colour == mUsedColour) {
        updateColour();
        return;
      }
    }
  }

  void SequenceTrackColourInstance::registerAssets(ComponentAssetRegistry& assets, const std::string& parentID) {
    cStateNotifier = assets.add<IColour>(this, parentID + "/" + cParent.getName(), "Colours animated from Sequences"); // TODO: Localize this.
  }

  void SequenceTrackColourInstance::set() const {
    cRuntimeColour.set();
  }

  float SequenceTrackColourInstance::getRed() const {
    return cRuntimeColour.getRed();
  }

  float SequenceTrackColourInstance::getGreen() const {
    return cRuntimeColour.getGreen();
  }

  float SequenceTrackColourInstance::getBlue() const {
    return cRuntimeColour.getBlue();
  }

  float SequenceTrackColourInstance::getAlpha() const {
    return cRuntimeColour.getAlpha();
  }

  void SequenceTrackColourInstance::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void SequenceTrackColourInstance::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool SequenceTrackColourInstance::isDefaultConfiguration() const {
    return true;
  }

  void SequenceTrackColourInstance::updateColour() {
    const IColour* mPreviousEventColour = getPreviousColour();
    LiteralColour mPreviousColour = cRuntimeColour;
    const std::vector<std::unique_ptr<SequenceTrackColourEvent>>& mEvents = cParent.getRealEvents();
    unsigned int mRuntimeEvent = getRuntimeEvent();
    int mRuntimeEventPosition = getRuntimeEventPosition();
    if (mRuntimeEvent < mEvents.size() && mEvents[mRuntimeEvent]->isFade()) {
      const IColour* mCurrentColour = mEvents[mRuntimeEvent]->getColour();
      int mEventDuration = mEvents[mRuntimeEvent]->getTime() - (mRuntimeEvent == 0 ? 0 : mEvents[mRuntimeEvent - 1]->getTime());
      int mEventPosition = mRuntimeEventPosition             - (mRuntimeEvent == 0 ? 0 : mEvents[mRuntimeEvent - 1]->getTime());
      float mRelativePosition = mEventPosition / static_cast<float>(mEventDuration);
      cRuntimeColour = LiteralColour(*mPreviousEventColour, *mCurrentColour, mRelativePosition);
    } else if (mPreviousEventColour != nullptr) {
      cRuntimeColour = LiteralColour(*mPreviousEventColour);
    } else {
      cRuntimeColour = LiteralColour(1.0f, 0.0f, 0.0f);
    }

    if (cRuntimeColour != mPreviousColour && cStateNotifier != nullptr) { // TODO: State notifier should never be nullptr!!!  Need to make sure "registerAssets" is called after adding a track in editor.
      cStateNotifier->stateChanged();
    }
  }

  const IColour* SequenceTrackColourInstance::getPreviousColour() {
    for (int i = getRuntimeEvent(); i > 0; i--) {
      const IColour* mPreviousEventColour = cParent.getRealEvents()[i - 1]->getColour();
      if (mPreviousEventColour != nullptr) {
        return mPreviousEventColour;
      }
    }
    return *cParent.getStartColour();
  }
}
