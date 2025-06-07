/*
 * Copyright 2023 Martin Brentnall
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
#include "SequenceTrackFloat.h"

namespace IsoRealms::Basics {
  const std::string SequenceTrackFloat::JSON_DURATION = "duration";
  const std::string SequenceTrackFloat::JSON_EVENTS   = "events";
  const std::string SequenceTrackFloat::JSON_FADE     = "fade";
  const std::string SequenceTrackFloat::JSON_OUTPUT   = "outputs";
  const std::string SequenceTrackFloat::JSON_START    = "start";
  const std::string SequenceTrackFloat::JSON_VALUE    = "value";

  SequenceTrackFloat::SequenceTrackFloat(IProject& project, Sequence& sequence) :
            cDefName("TODO"),
            cDefStartValue(project, 0.0f, [this](float value) {stateChanged(*cDefStartValue);}) {
  }

  SequenceTrackFloat::SequenceTrackFloat(IProject& project, Sequence& sequence, JSONObject object) :
            cDefName(object.getString(JSON_OUTPUT)),
            cDefStartValue(project, 0.0f, [this](float value) {stateChanged(*cDefStartValue);}) {
    cDefStartValue.init(object, JSON_START);
    for (JSONObject mEventObject : object.getArray(JSON_EVENTS)) {
      cDefEvents.push_back(std::make_unique<Event>(project, mEventObject));
    }
  }

  unsigned int SequenceTrackFloat::getDuration() const {
    return cDefEvents.empty() ? 0 : cDefEvents[cDefEvents.size() - 1]->getTime();
  }

  void SequenceTrackFloat::setName(const std::string& name) {
    cDefName = name;
  }

  std::string SequenceTrackFloat::getName() const {
    return cDefName;
  }

  ISequenceTrackEvent* SequenceTrackFloat::createEvent(IProject& project, unsigned int time) {
    if (time == 0) {
      return this;
    }
    for (unsigned int i = 0; i < cDefEvents.size(); i++) {
      if (cDefEvents[i]->getTime() == time) {
        return cDefEvents[i].get();
      } else if (cDefEvents[i]->getTime() > time) {
        return cDefEvents.insert(cDefEvents.begin() + i, std::make_unique<Event>(project, time, true))->get();
      }
    }
    return cDefEvents.emplace_back(std::make_unique<Event>(project, time, true)).get();
  }

  void SequenceTrackFloat::deleteEvent(ISequenceTrackEvent* event) {
    Utils::removeElementUnique(cDefEvents, event);
  }

  void SequenceTrackFloat::setEventTime(ISequenceTrackEvent* event, unsigned int time) {
    // TODO: Implement this.
  }

  std::vector<ISequenceTrackEvent*> SequenceTrackFloat::getEvents() {
    std::vector<ISequenceTrackEvent*> mEvents;
    mEvents.emplace_back(this);
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      mEvents.emplace_back(mEvent.get());
    }
    return mEvents;
  }

  void SequenceTrackFloat::renderIcon() const {
    Utils::renderIconBranch();
  }

  void SequenceTrackFloat::render(float left, float bottom, float right, float top, double startTime, double endTime) const {
    unsigned int mViewDuration = endTime - startTime;
    glColor3f(0.6f, 0.3f, 1.0f);

    float mHighest = std::numeric_limits<float>::min();
    float mLowest  = std::numeric_limits<float>::max();
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      mHighest = std::max(mHighest, mEvent->getValue()->getValue());
      mLowest  = std::min(mLowest,  mEvent->getValue()->getValue());
    }
    float mRange = mHighest - mLowest;
    float mHeight = top - bottom;

    glBegin(GL_QUADS);
    float mLeft = left;
    float mPosition = bottom + mHeight * (cDefStartValue->getValue() - mLowest) / mRange;
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      float mRight = (right - left) * (mEvent->getTime() / static_cast<float>(mViewDuration)) + left;
      glVertex2f(mLeft,  mPosition);
      glVertex2f(mLeft,  bottom);
      if (mEvent->isFade()) {
        mPosition = bottom + mHeight * (mEvent->getValue()->getValue() - mLowest) / mRange;
      }
      glVertex2f(mRight, bottom);
      glVertex2f(mRight, mPosition);
      mPosition = bottom + mHeight * (mEvent->getValue()->getValue() - mLowest) / mRange;
      mLeft = mRight;
    }
    glEnd();
  }

  ISequenceTrackInstance* SequenceTrackFloat::createTrackInstance(SequenceInstance& sequenceInstance) {
    return cInstances.emplace_back(std::make_unique<Instance>(*this)).get();
  }

  bool SequenceTrackFloat::renderAssetIcon() const {
    return false;
  }

  unsigned int SequenceTrackFloat::getTime() const {
    return 0U;
  }

  void SequenceTrackFloat::setTime(unsigned int time) {
    // Cannot change.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackFloat::getEventProperties(IProject& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>("Start Value", "TODO", cDefStartValue));
    return mProperties;
  }

  void SequenceTrackFloat::saveAsset(JSONObject object) const {
    object.addString(JSON_OUTPUT, cDefName);
    cDefStartValue.save(object, JSON_START);
    JSONArray mEventsArray = object.addArray(JSON_EVENTS);
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      JSONObject mEventObject = mEventsArray.addObject();
      mEvent->save(mEventObject);
    }
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackFloat::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackFloat::isDefaultConfiguration() const {
    return true;
  }

  void SequenceTrackFloat::stateChanged(IFloat* value) {
    for (std::unique_ptr<Instance>& mInstance : cInstances) {
      mInstance->stateChanged(value);
    }
  }

  SequenceTrackFloat::Instance::Instance(SequenceTrackFloat& parent) :
            cParent(parent) {
  }

  void SequenceTrackFloat::Instance::stateChanged(IFloat* value) {
    IFloat* mUsedFloat = getPreviousValue();
    if (value == mUsedFloat) {
      updateValue();
      return;
    }

    if (cRuntimeEvent < cParent.cDefEvents.size() && (cRuntimeEvent > 0 || cRuntimeEventPosition > 0)) {
      int mRuntimeEvent = cRuntimeEventPosition == 0 ? cRuntimeEvent - 1 : cRuntimeEvent;
      mUsedFloat = cParent.cDefEvents[mRuntimeEvent]->getValue();
      if (value == mUsedFloat) {
        updateValue();
        return;
      }
    }
  }

  void SequenceTrackFloat::Instance::registerAssets(IAssetRegistry& assets) {
    cStateNotifier = assets.add(this, cParent.cDefName, "Sequences");
  }

  void SequenceTrackFloat::Instance::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(this, relinquish);
    cStateNotifier = nullptr;
  }

  bool SequenceTrackFloat::Instance::play(unsigned int milliseconds) {
    bool mStillPlaying = false;
    if (cRuntimeEvent < cParent.cDefEvents.size()) {
      cRuntimeEventPosition += milliseconds;
      int mNextEventTime = cParent.cDefEvents[cRuntimeEvent]->getTime();
      while (cRuntimeEvent < cParent.cDefEvents.size() && cRuntimeEventPosition >= mNextEventTime) {
        cRuntimeEvent++;
        if (cRuntimeEvent < cParent.cDefEvents.size()) {
          mNextEventTime = cParent.cDefEvents[cRuntimeEvent]->getTime();;
        }
      }
      mStillPlaying = true;
    }
    updateValue();
    return mStillPlaying;
  }

  void SequenceTrackFloat::Instance::reset() {
    cRuntimeEvent         = 0;
    cRuntimeEventPosition = 0;
    updateValue();
  }

  void SequenceTrackFloat::Instance::stopPreview() {
    cRuntimeEvent = 0;
    cRuntimeEventPosition = 0;
  }

  void SequenceTrackFloat::Instance::setPreviewPosition(long position) {
    cRuntimeEvent = 0;
    cRuntimeEventPosition = 0;
    play(position);
  }

  float SequenceTrackFloat::Instance::getValue() const {
    return cRuntimeValue;
  }

  bool SequenceTrackFloat::Instance::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackFloat::Instance::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackFloat::Instance::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackFloat::Instance::isDefaultConfiguration() const {
    return true;
  }

  void SequenceTrackFloat::Instance::updateValue() {
    float mPreviousEventValue = getPreviousValue()->getValue();
    float mPreviousValue = cRuntimeValue;
    if (cRuntimeEvent < cParent.cDefEvents.size() && cParent.cDefEvents[cRuntimeEvent]->isFade()) {
      float mCurrentValue = cParent.cDefEvents[cRuntimeEvent]->getValue()->getValue();
      int mEventDuration = cParent.cDefEvents[cRuntimeEvent]->getTime() - (cRuntimeEvent == 0 ? 0 : cParent.cDefEvents[cRuntimeEvent - 1]->getTime());
      int mEventPosition = cRuntimeEventPosition                        - (cRuntimeEvent == 0 ? 0 : cParent.cDefEvents[cRuntimeEvent - 1]->getTime());
      float mRelativePosition = mEventPosition / static_cast<float>(mEventDuration);
      cRuntimeValue = mPreviousEventValue + (mCurrentValue - mPreviousEventValue) * mRelativePosition;
    } else {
      cRuntimeValue = mPreviousEventValue;
    }

    if (cRuntimeValue != mPreviousValue) {
      cStateNotifier->stateChanged(this);
    }
  }

  IFloat* SequenceTrackFloat::Instance::getPreviousValue() {
    return cRuntimeEvent > 0 ? cParent.cDefEvents[cRuntimeEvent - 1]->getValue() : *cParent.cDefStartValue;
  }

  SequenceTrackFloat::Event::Event(IProject& project, unsigned int time, bool fade) :
            cDefTime(time),
            cDefValue(project, 0.0f),
            cDefFade(fade) {
  }

  SequenceTrackFloat::Event::Event(IProject& project, JSONObject object) :
            Event(project, object.getInteger(JSON_DURATION), object.getBoolean(JSON_FADE, true)) {
    cDefValue.init(object, JSON_VALUE);
  }

  void SequenceTrackFloat::Event::save(JSONObject object) const {
    object.addInteger(JSON_DURATION, cDefTime);
    object.addBoolean(JSON_FADE, cDefFade, true);
    cDefValue.save(object, JSON_VALUE);
  }

  unsigned int SequenceTrackFloat::Event::getTime() const {
    return cDefTime;
  }

  void SequenceTrackFloat::Event::setTime(unsigned int time) {
    cDefTime = time;
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackFloat::Event::getEventProperties(IProject& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Float>>("Value", "TODO", cDefValue));
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>("Fade", "TODO", [this]() {return cDefFade;}, [this](bool value) {cDefFade = value;}, project));
    return mProperties;
  }

  IFloat* SequenceTrackFloat::Event::getValue() const {
    return *cDefValue;
  }

  bool SequenceTrackFloat::Event::isFade() const {
    return cDefFade;
  }
}
