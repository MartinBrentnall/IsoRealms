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
#include "SequenceTrackFloatInstance.h"

#include "SequenceTrackFloat.h"

namespace IsoRealms::Basics {
  SequenceTrackFloatInstance::SequenceTrackFloatInstance(SequenceTrackFloat& parent, SequenceInstance& sequenceInstance) :
            SequenceTrackInstanceBase(parent.getRealEvents()),
            cParent(parent),
            cStateNotifier(nullptr) {
  }

  void SequenceTrackFloatInstance::positionChanged() {
    updateValue();
  }

  void SequenceTrackFloatInstance::stateChanged(IFloat* value) {
    IFloat* mUsedFloat = getPreviousValue();
    if (value == mUsedFloat) {
      updateValue();
      return;
    }

    unsigned int mRuntimeEvent = getRuntimeEvent();
    int mRuntimeEventPosition = getRuntimeEventPosition();
    if (mRuntimeEvent < cParent.getRealEvents().size() && (mRuntimeEvent > 0 || mRuntimeEventPosition > 0)) {
      mRuntimeEvent = mRuntimeEventPosition == 0 ? mRuntimeEvent - 1 : mRuntimeEvent;
      mUsedFloat = cParent.getRealEvents()[mRuntimeEvent]->getValue();
      if (value == mUsedFloat) {
        updateValue();
        return;
      }
    }
  }

  void SequenceTrackFloatInstance::registerAssets(ResourceAssetRegistry& assets, const std::string& parentID) {
    cStateNotifier = assets.add<IFloat>(this, parentID + "/" + cParent.getName(), "Sequences");
  }

  float SequenceTrackFloatInstance::getValue() const {
    return cRuntimeValue;
  }

  bool SequenceTrackFloatInstance::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackFloatInstance::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void SequenceTrackFloatInstance::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool SequenceTrackFloatInstance::isDefaultConfiguration() const {
    return true;
  }

  void SequenceTrackFloatInstance::updateValue() {
    float mPreviousEventValue = getPreviousValue()->getValue();
    float mPreviousValue = cRuntimeValue;
    const std::vector<std::unique_ptr<SequenceTrackFloatEvent>>& mEvents = cParent.getRealEvents();
    unsigned int mRuntimeEvent = getRuntimeEvent();
    int mRuntimeEventPosition = getRuntimeEventPosition();
    if (mRuntimeEvent < mEvents.size() && mEvents[mRuntimeEvent]->isFade()) {
      float mCurrentValue = mEvents[mRuntimeEvent]->getValue()->getValue();
      int mEventDuration = mEvents[mRuntimeEvent]->getTime() - (mRuntimeEvent == 0 ? 0 : mEvents[mRuntimeEvent - 1]->getTime());
      int mEventPosition = mRuntimeEventPosition             - (mRuntimeEvent == 0 ? 0 : mEvents[mRuntimeEvent - 1]->getTime());
      float mRelativePosition = mEventPosition / static_cast<float>(mEventDuration);
      cRuntimeValue = mPreviousEventValue + (mCurrentValue - mPreviousEventValue) * mRelativePosition;
    } else {
      cRuntimeValue = mPreviousEventValue;
    }

    if (cRuntimeValue != mPreviousValue) {
      cStateNotifier->stateChanged();
    }
  }

  IFloat* SequenceTrackFloatInstance::getPreviousValue() {
    return getRuntimeEvent() > 0 ? cParent.getRealEvents()[getRuntimeEvent() - 1]->getValue() : *cParent.getStartValue();
  }
}
