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
#include "SequenceTrackAction.h"

namespace IsoRealms::Basics {
  const std::string SequenceTrackAction::JSON_DELAY   = "delay";
  const std::string SequenceTrackAction::JSON_EVENTS  = "events";
  const std::string SequenceTrackAction::JSON_EXECUTE = "execute";

  SequenceTrackAction::SequenceTrackAction(IProject& project, Sequence& sequence) {
//    reset();
  }
  
  SequenceTrackAction::SequenceTrackAction(IProject& project, Sequence& sequence, JSONObject object) {
    for (JSONObject mEventObject : object.getArray(JSON_EVENTS)) {
      cDefEvents.emplace_back(std::make_unique<Event>(mEventObject, project));
    }
//    reset();
  }

  unsigned int SequenceTrackAction::getDuration() const {
    return cDefEvents.empty() ? 0 : cDefEvents[cDefEvents.size() - 1]->getTime();
  }

  void SequenceTrackAction::setName(const std::string& name) {
    // Track has no name, not supported.
  }

  std::string SequenceTrackAction::getName() const {
    return "";
  }

  ISequenceTrackEvent* SequenceTrackAction::createEvent(IProject& project, unsigned int time) {
    return nullptr; // TODO: Implement this.
  }

  void SequenceTrackAction::deleteEvent(ISequenceTrackEvent* event) {
    Utils::removeElementUnique(cDefEvents, event);
  }

  void SequenceTrackAction::setEventTime(ISequenceTrackEvent* event, unsigned int time) {
    // TODO: Implement this.
  }

  std::vector<ISequenceTrackEvent*> SequenceTrackAction::getEvents() {
    std::vector<ISequenceTrackEvent*> mEvents;
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      mEvents.emplace_back(mEvent.get());
    }
    return mEvents;
  }

  void SequenceTrackAction::renderIcon() const {
    Utils::renderIconTerminal();
  }

  void SequenceTrackAction::render(float left, float bottom, float right, float top, double startTime, double endTime) const {
    unsigned int mViewDuration = endTime - startTime;
    float mRight = (right - left) * (getDuration() / static_cast<float>(mViewDuration)) + left;

    // Render a nice background
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.0f, 0.0f);
    glVertex2f(mRight, top);
    glVertex2f(left,  top);
    glColor3f(0.2f, 0.0f, 0.0f);
    glVertex2f(left,  bottom);
    glVertex2f(mRight, bottom);
    glEnd();
  }

  ISequenceTrackInstance* SequenceTrackAction::createTrackInstance() {
    return cInstances.emplace_back(std::make_unique<Instance>(*this)).get();
  }

  bool SequenceTrackAction::renderAssetIcon() const {
    return false;
  }
  
  void SequenceTrackAction::saveAsset(JSONObject object) const {
    JSONArray mEventsArray = object.addArray(JSON_EVENTS);
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      JSONObject mEventObject = mEventsArray.addObject();
      mEvent->save(mEventObject);
    }
  }
  
  std::vector<std::unique_ptr<IProperty>> SequenceTrackAction::getAssetProperties() {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    return mProperties;
  }

  bool SequenceTrackAction::isDefaultConfiguration() const {
    return true;
  }

  SequenceTrackAction::Instance::Instance(SequenceTrackAction& parent) :
            cParent(parent),
            cRuntimeEvent(0),
            cRuntimeEventPosition(0) {
  }

  void SequenceTrackAction::Instance::registerAssets(IAssetRegistry& assets) {
    // Nothing to do.
  }

  void SequenceTrackAction::Instance::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    // Nothing to do.
  }

  bool SequenceTrackAction::Instance::play(unsigned int milliseconds) {
    bool mStillPlaying = false;
    if (cRuntimeEvent < cParent.cDefEvents.size()) {
      std::vector<Event*> mEventToExecute;
      cRuntimeEventPosition += milliseconds;
      int mNextEventTime = cParent.cDefEvents[cRuntimeEvent]->getTime();
      while (cRuntimeEvent < cParent.cDefEvents.size() && cRuntimeEventPosition >= mNextEventTime) {
        mEventToExecute.emplace_back(cParent.cDefEvents[cRuntimeEvent].get());
        cRuntimeEvent++;
        if (cRuntimeEvent < cParent.cDefEvents.size()) {
          mNextEventTime = cParent.cDefEvents[cRuntimeEvent]->getTime();;
        }
      }
      mStillPlaying = true;

      // Execute actions at the end in case one triggers a modification to the state of the sequence.
      for (Event* mEvent : mEventToExecute) {
        mEvent->execute();
      }
    }
    return mStillPlaying;
  }

  void SequenceTrackAction::Instance::reset() {
    cRuntimeEvent = 0;
    cRuntimeEventPosition = 0;
  }

  void SequenceTrackAction::Instance::stopPreview() {
    // TODO: Implement this.
  }

  void SequenceTrackAction::Instance::setPreviewPosition(long position) {
    // TODO: Implement this.
  }

  SequenceTrackAction::Event::Event(JSONObject object, IProject& project) :
            cDefAction(project),
            cDefTime(object.getInteger(JSON_DELAY)) {
    cDefAction.init(object, JSON_EXECUTE);
  }

  unsigned int SequenceTrackAction::Event::getTime() const {
    return cDefTime;
  }

  void SequenceTrackAction::Event::setTime(unsigned int time) {
    cDefTime = time;
  }

  std::vector<std::unique_ptr<IProperty>> SequenceTrackAction::Event::getEventProperties(IProject& project) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Action", "TODO", cDefAction));
    return mProperties;
  }

  void SequenceTrackAction::Event::save(JSONObject object) const {
    object.addInteger(JSON_DELAY, cDefTime);
    cDefAction.save(object, JSON_EXECUTE);
  }

  void SequenceTrackAction::Event::execute() {
    cDefAction.execute();
  }
}
