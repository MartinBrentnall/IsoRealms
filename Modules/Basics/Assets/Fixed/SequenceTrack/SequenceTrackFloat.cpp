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
            cOutput(*this),
            cDefName("TODO"),
            cDefStartValue(project, 0.0f, [this](float value) {stateChanged(cDefStartValue->getValue());}),
            cRuntimeEvent(0),
            cRuntimeEventPosition(0),
            cStateNotifier(nullptr) {
  }

  SequenceTrackFloat::SequenceTrackFloat(IProject& project, Sequence& sequence, JSONObject object) :
            cOutput(*this),
            cDefName(object.getString(JSON_OUTPUT)),
            cDefStartValue(project, 0.0f, [this](float value) {stateChanged(cDefStartValue->getValue());}),
            cRuntimeEvent(0),
            cRuntimeEventPosition(0),
            cStateNotifier(nullptr) {
    cDefStartValue.init(object, JSON_START);
    for (JSONObject mEventObject : object.getArray(JSON_EVENTS)) {
      cDefEvents.push_back(std::make_unique<Event>(project, mEventObject));
    }
  }

  void SequenceTrackFloat::registerAssets(IAssetRegistry& assets) {
    cStateNotifier = assets.add(&cOutput, cDefName, "Sequences");
  }

  void SequenceTrackFloat::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(&cOutput, relinquish);
    cStateNotifier = nullptr;
  }

  bool SequenceTrackFloat::play(unsigned int milliseconds) {
    bool mStillPlaying = false;
    if (cRuntimeEvent < cDefEvents.size()) {
      cRuntimeEventPosition += milliseconds;
      int mNextEventTime = cDefEvents[cRuntimeEvent]->getTime();
      while (cRuntimeEvent < cDefEvents.size() && cRuntimeEventPosition >= mNextEventTime) {
        cRuntimeEvent++;
        if (cRuntimeEvent < cDefEvents.size()) {
          mNextEventTime = cDefEvents[cRuntimeEvent]->getTime();;
        }
      }
      mStillPlaying = true;
    }
    updateValue();
    return mStillPlaying;
  }

  void SequenceTrackFloat::reset() {
    cRuntimeEvent         = 0;
    cRuntimeEventPosition = 0;
    updateValue();
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

  void SequenceTrackFloat::stopPreview() {
    cRuntimeEvent = 0;
    cRuntimeEventPosition = 0;
  }

  void SequenceTrackFloat::setPreviewPosition(long position) {
    cRuntimeEvent = 0;
    cRuntimeEventPosition = 0;
    play(position);
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
      mHighest = std::max(mHighest, mEvent->getValue());
      mLowest  = std::min(mLowest,  mEvent->getValue());
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
        mPosition = bottom + mHeight * (mEvent->getValue() - mLowest) / mRange;
      }
      glVertex2f(mRight, bottom);
      glVertex2f(mRight, mPosition);
      mPosition = bottom + mHeight * (mEvent->getValue() - mLowest) / mRange;
      mLeft = mRight;
    }
    glEnd();
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

  void SequenceTrackFloat::updateValue() {
    float mPreviousEventValue = getPreviousValue();
    float mPreviousValue = cRuntimeValue;
    if (cRuntimeEvent < cDefEvents.size() && cDefEvents[cRuntimeEvent]->isFade()) {
      float mCurrentValue = cDefEvents[cRuntimeEvent]->getValue();
      int mEventDuration = cDefEvents[cRuntimeEvent]->getTime() - (cRuntimeEvent == 0 ? 0 : cDefEvents[cRuntimeEvent - 1]->getTime());
      int mEventPosition = cRuntimeEventPosition                - (cRuntimeEvent == 0 ? 0 : cDefEvents[cRuntimeEvent - 1]->getTime());
      float mRelativePosition = mEventPosition / static_cast<float>(mEventDuration);
      cRuntimeValue = mPreviousEventValue + (mCurrentValue - mPreviousEventValue) * mRelativePosition;
    } else {
      cRuntimeValue = mPreviousEventValue;
    }

    if (cRuntimeValue != mPreviousValue) {
      cStateNotifier->stateChanged(&cOutput);
    }
  }

  float SequenceTrackFloat::getPreviousValue() {
    return cRuntimeEvent > 0 ? cDefEvents[cRuntimeEvent - 1]->getValue() : cDefStartValue->getValue();
  }

  void SequenceTrackFloat::stateChanged(float value) {
    float mUsedFloat = getPreviousValue();
    if (value == mUsedFloat) {
      updateValue();
      return;
    }

    if (cRuntimeEvent < cDefEvents.size() && (cRuntimeEvent > 0 || cRuntimeEventPosition > 0)) {
      int mRuntimeEvent = cRuntimeEventPosition == 0 ? cRuntimeEvent - 1 : cRuntimeEvent;
      mUsedFloat = cDefEvents[mRuntimeEvent]->getValue();
      if (value == mUsedFloat) {
        updateValue();
        return;
      }
    }
  }

  SequenceTrackFloat::Output::Output(SequenceTrackFloat& parent) :
            cParent(parent) {
  }

  float SequenceTrackFloat::Output::getValue() const {
    return cParent.cRuntimeValue;
  }

  bool SequenceTrackFloat::Output::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackFloat::Output::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackFloat::Output::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackFloat::Output::isDefaultConfiguration() const {
    return true;
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

  float SequenceTrackFloat::Event::getValue() const {
    return cDefValue->getValue();
  }

  bool SequenceTrackFloat::Event::isFade() const {
    return cDefFade;
  }
}
