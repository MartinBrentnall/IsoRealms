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
#include "SequenceTrackColour.h"

namespace IsoRealms::Basics {
  const std::string SequenceTrackColour::JSON_DURATION = "duration";
  const std::string SequenceTrackColour::JSON_EVENTS   = "events";
  const std::string SequenceTrackColour::JSON_FADE     = "fade";
  const std::string SequenceTrackColour::JSON_OUTPUT   = "outputs";
  const std::string SequenceTrackColour::JSON_START    = "start";
  const std::string SequenceTrackColour::JSON_TARGET   = "target";
  const std::string SequenceTrackColour::JSON_TYPE     = "type";

  SequenceTrackColour::SequenceTrackColour(IProject& project, Sequence& sequence) :
            cDefName("TODO"),
            cDefInitColour(project, 1.0f, 0.0f, 0.0f, 0.0f, [this]() {stateChanged(*cDefInitColour);}) {
  }
  
  SequenceTrackColour::SequenceTrackColour(IProject& project, Sequence& sequence, JSONObject object) :
            cDefName(object.getString(JSON_OUTPUT)),
            cDefInitColour(project, 1.0f, 0.0f, 0.0f, 0.0f, [this]() {stateChanged(*cDefInitColour);}) {
    cDefInitColour.init(object, JSON_START);
    for (JSONObject mEventObject : object.getArray(JSON_EVENTS)) {
      cDefEvents.push_back(std::make_unique<Event>(project, mEventObject));
    }
  }

  unsigned int SequenceTrackColour::getDuration() const {
    return cDefEvents.empty() ? 0 : cDefEvents[cDefEvents.size() - 1]->getTime();
  }

  void SequenceTrackColour::setName(const std::string& name) {
    cDefName = name;
  }

  std::string SequenceTrackColour::getName() const {
    return cDefName;
  }

  ISequenceTrackEvent* SequenceTrackColour::createEvent(IProject& project, unsigned int time) {
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

  void SequenceTrackColour::deleteEvent(ISequenceTrackEvent* event) {
    Utils::removeElementUnique(cDefEvents, event);
  }

  void SequenceTrackColour::setEventTime(ISequenceTrackEvent* event, unsigned int time) {
    int mEventIndex = Utils::getIndex(cDefEvents, event);
    int mNewIndex = 0;
    for (unsigned int i = 0; i < cDefEvents.size(); i++) {
      if (cDefEvents[i]->getTime() >= time) {
        mNewIndex = i;
        break;
      }
    }

    std::unique_ptr<Event> mEventToMove = nullptr;
    if (mNewIndex < mEventIndex) {
      mEventToMove = std::move(cDefEvents[mEventIndex]);
      cDefEvents.erase(cDefEvents.begin() + mEventIndex);
    } else if (mNewIndex > mEventIndex + 1) {
      mEventToMove = std::move(cDefEvents[mEventIndex]);
      cDefEvents.erase(cDefEvents.begin() + mEventIndex);
      mNewIndex--;
    }

    if (mEventToMove != nullptr) {
      cDefEvents.insert(cDefEvents.begin() + mNewIndex, std::move(mEventToMove));
    }
    event->setTime(time);
  }

  std::vector<ISequenceTrackEvent*> SequenceTrackColour::getEvents() {
    std::vector<ISequenceTrackEvent*> mEvents;
    mEvents.emplace_back(this);
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      mEvents.emplace_back(mEvent.get());
    }
    return mEvents;
  }

  void SequenceTrackColour::renderIcon() const {
    float mOffset = 0.15f;
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = -2; i <= 2; i++) {
      Utils::renderCircle(i * mOffset, i * -mOffset, 0.5f);
    }
    glColor3f(0.0f, 0.0f, 1.0f);
    Utils::renderCircle(-2 * mOffset,-2 * -mOffset, 0.45f);
    glColor3f(0.0f, 1.0f, 0.0f);
    Utils::renderCircle(-1 * mOffset,-1 * -mOffset, 0.45f);
    glColor3f(1.0f, 1.0f, 0.0f);
    Utils::renderCircle( 0 * mOffset, 0 * -mOffset, 0.45f);
    glColor3f(1.0f, 0.5f, 0.0f);
    Utils::renderCircle( 1 * mOffset, 1 * -mOffset, 0.45f);
    glColor3f(1.0f, 0.0f, 0.0f);
    Utils::renderCircle( 2 * mOffset, 2 * -mOffset, 0.45f);
  }

  void SequenceTrackColour::render(float left, float bottom, float right, float top, double startTime, double endTime) const {
    unsigned int mViewDuration = endTime - startTime;
    cDefInitColour->set();
    glBegin(GL_QUADS);
    float mLeft = left;

    // Render track
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      float mRight = (right - left) * (mEvent->getTime() / static_cast<float>(mViewDuration)) + left;
      glVertex2f(mLeft,  top);
      glVertex2f(mLeft,  bottom);
      if (mEvent->isFade()) {
        mEvent->getColour()->set();
      }
      glVertex2f(mRight, bottom);
      glVertex2f(mRight, top);
      mEvent->getColour()->set();
      mLeft = mRight;
    }
    glEnd();
  }

  ISequenceTrackInstance* SequenceTrackColour::createTrackInstance(SequenceInstance& sequenceInstance) {
    return cInstances.emplace_back(std::make_unique<Instance>(*this)).get();
  }

  unsigned int SequenceTrackColour::getTime() const {
    return 0U;
  }

  void SequenceTrackColour::setTime(unsigned int time) {
    // Cannot change.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackColour::getEventProperties(IProject& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Start Colour", "TODO", cDefInitColour));
    return mProperties;
  }

  bool SequenceTrackColour::renderAssetIcon() const {
    return false;
  }

  void SequenceTrackColour::saveAsset(JSONObject object) const {
    object.addString(JSON_OUTPUT, cDefName);
    cDefInitColour.save(object, JSON_START);
    JSONArray mEventsArray = object.addArray(JSON_EVENTS);
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      JSONObject mEventObject = mEventsArray.addObject();
      mEvent->save(mEventObject);
    }
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackColour::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackColour::isDefaultConfiguration() const {
    return true;
  }

  void SequenceTrackColour::stateChanged(IColour* colour) {
    for (std::unique_ptr<Instance>& mInstance : cInstances) {
      mInstance->stateChanged(colour);
    }
  }

  SequenceTrackColour::Instance::Instance(SequenceTrackColour& parent) :
            cParent(parent),
            cRuntimeEvent(0),
            cRuntimeEventPosition(0),
            cStateNotifier(nullptr) {
  }

  void SequenceTrackColour::Instance::registerAssets(IAssetRegistry& assets) {
    cStateNotifier = assets.add(this, cParent.cDefName, "Sequences");
  }

  void SequenceTrackColour::Instance::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    assets.remove(this, relinquish);
    cStateNotifier = nullptr;
  }

  void SequenceTrackColour::Instance::stateChanged(IColour* colour) {
    const IColour* mUsedColour = getPreviousColour();
    if (colour == mUsedColour) {
      updateColour();
      return;
    }

    if (cRuntimeEvent < cParent.cDefEvents.size() && (cRuntimeEvent > 0 || cRuntimeEventPosition > 0)) {
      int mRuntimeEvent = cRuntimeEventPosition == 0 ? cRuntimeEvent - 1 : cRuntimeEvent;
      mUsedColour = cParent.cDefEvents[mRuntimeEvent]->getColour();
      if (colour == mUsedColour) {
        updateColour();
        return;
      }
    }
  }

  bool SequenceTrackColour::Instance::play(unsigned int milliseconds) {
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
    updateColour();
    return mStillPlaying;
  }

  void SequenceTrackColour::Instance::reset() {
    cRuntimeEvent         = 0;
    cRuntimeEventPosition = 0;
    updateColour();
  }

  void SequenceTrackColour::Instance::stopPreview() {
    cRuntimeEvent = 0;
    cRuntimeEventPosition = 0;
  }

  void SequenceTrackColour::Instance::setPreviewPosition(long position) {
    cRuntimeEvent = 0;
    cRuntimeEventPosition = 0;
    play(position);
  }

  void SequenceTrackColour::Instance::updateColour() {
    const IColour* mPreviousEventColour = getPreviousColour();
    LiteralColour mPreviousColour = cRuntimeColour;
    if (cRuntimeEvent < cParent.cDefEvents.size() && cParent.cDefEvents[cRuntimeEvent]->isFade()) {
      const IColour* mCurrentColour = cParent.cDefEvents[cRuntimeEvent]->getColour();
      int mEventDuration = cParent.cDefEvents[cRuntimeEvent]->getTime() - (cRuntimeEvent == 0 ? 0 : cParent.cDefEvents[cRuntimeEvent - 1]->getTime());
      int mEventPosition = cRuntimeEventPosition                        - (cRuntimeEvent == 0 ? 0 : cParent.cDefEvents[cRuntimeEvent - 1]->getTime());
      float mRelativePosition = mEventPosition / static_cast<float>(mEventDuration);
      cRuntimeColour = LiteralColour(*mPreviousEventColour, *mCurrentColour, mRelativePosition);
    } else if (mPreviousEventColour != nullptr) {
      cRuntimeColour = LiteralColour(*mPreviousEventColour);
    } else {
      cRuntimeColour = LiteralColour(1.0f, 0.0f, 0.0f);
    }

    if (cRuntimeColour != mPreviousColour && cStateNotifier != nullptr) { // TODO: State notifier should never be nullptr!!!  Need to make sure "registerAssets" is called after adding a track in editor.
      cStateNotifier->stateChanged(this);
    }
  }

  const IColour* SequenceTrackColour::Instance::getPreviousColour() {
    for (int i = cRuntimeEvent; i > 0; i--) {
      const IColour* mPreviousEventColour = cParent.cDefEvents[i - 1]->getColour();
      if (mPreviousEventColour != nullptr) {
        return mPreviousEventColour;
      }
    }
    return *cParent.cDefInitColour;
  }

  void SequenceTrackColour::Instance::set() const {
    cRuntimeColour.set();
  }

  float SequenceTrackColour::Instance::getRed() const {
    return cRuntimeColour.getRed();
  }

  float SequenceTrackColour::Instance::getGreen() const {
    return cRuntimeColour.getGreen();
  }

  float SequenceTrackColour::Instance::getBlue() const {
    return cRuntimeColour.getBlue();
  }

  float SequenceTrackColour::Instance::getAlpha() const {
    return cRuntimeColour.getAlpha();
  }

  void SequenceTrackColour::Instance::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackColour::Instance::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool SequenceTrackColour::Instance::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackColour::Event::Event(IProject& project, unsigned int time, bool fade) :
            cDefTime(time),
            cDefTarget(project, 1.0f, 0.0f, 0.0f),
            cDefFade(fade) {
  }

  SequenceTrackColour::Event::Event(IProject& project, JSONObject object) :
            Event(project, object.getInteger(JSON_DURATION), object.getBoolean(JSON_FADE, true)) {
    cDefTarget.init(object, JSON_TARGET);
  }

  void SequenceTrackColour::Event::save(JSONObject object) const {
    object.addInteger(JSON_DURATION, cDefTime);
    object.addBoolean(JSON_FADE, cDefFade, true);
    cDefTarget.save(object, JSON_TARGET);
  }

  unsigned int SequenceTrackColour::Event::getTime() const {
    return cDefTime;
  }

  void SequenceTrackColour::Event::setTime(unsigned int time) {
    cDefTime = time;
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackColour::Event::getEventProperties(IProject& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Colour>>("Colour", "TODO", cDefTarget));
    mProperties.emplace_back(std::make_unique<PropertyNativeBoolean>("Fade",   "TODO", [this]() {return cDefFade;}, [this](bool fade) {cDefFade = fade;}, project));
    return mProperties;
  }

  const IColour* SequenceTrackColour::Event::getColour() const {
    return *cDefTarget;
  }

  bool SequenceTrackColour::Event::isFade() const {
    return cDefFade;
  }
}
