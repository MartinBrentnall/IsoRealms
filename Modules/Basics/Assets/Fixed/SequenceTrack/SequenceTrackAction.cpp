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
    reset();
  }
  
  SequenceTrackAction::SequenceTrackAction(IProject& project, Sequence& sequence, JSONObject object) {
    for (JSONObject mEventObject : object.getArray(JSON_EVENTS)) {
      cDefEvents.emplace_back(std::make_unique<Event>(mEventObject, project, cDefEvents.empty() ? 0 : cDefEvents.back()->getTime()));
    }
    reset();
  }

  void SequenceTrackAction::registerAssets(IAssetRegistry& assets) {
    // Nothing to do.
  }

  void SequenceTrackAction::unregisterAssets(IAssetRemover& assets, bool relinquish) {
    // Nothing to do.
  }

  bool SequenceTrackAction::play(unsigned int milliseconds) {
    if (cRuntimeAction < cDefEvents.size()) {
      std::vector<Event*> mEventToExecute;
      while (milliseconds > cRuntimeActionRemaining) {
        mEventToExecute.emplace_back(cDefEvents[cRuntimeAction].get());
        cRuntimeAction++;
        if (cRuntimeAction == cDefEvents.size()) {
          break;
        }
        milliseconds -= cRuntimeActionRemaining;
        cRuntimeActionRemaining = cDefEvents[cRuntimeAction]->getTime() - (cRuntimeAction == 0 ? 0 : cDefEvents[cRuntimeAction - 1]->getTime());
      }
      cRuntimeActionRemaining -= milliseconds;

      // Execute actions at the end in case one triggers a modification to the state of the sequence.
      for (Event* mEvent : mEventToExecute) {
        mEvent->execute();
      }
      return true;
    }
    return false;
  }

  void SequenceTrackAction::reset() {
    cRuntimeAction = 0;
    cRuntimeActionRemaining = cDefEvents[cRuntimeAction]->getTime() - (cRuntimeAction == 0 ? 0 : cDefEvents[cRuntimeAction - 1]->getTime());
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

  std::vector<ISequenceTrackEvent*> SequenceTrackAction::getEvents() {
    std::vector<ISequenceTrackEvent*> mEvents;
    for (const std::unique_ptr<Event>& mEvent : cDefEvents) {
      mEvents.emplace_back(mEvent.get());
    }
    return mEvents;
  }

  void SequenceTrackAction::stopPreview() {
    // TODO: Implement this.
  }

  void SequenceTrackAction::setPreviewPosition(long position) {
    // TODO: Implement this.
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

  SequenceTrackAction::Event::Event(JSONObject object, IProject& project, unsigned int prevTimes) :
            cDefAction(project),
            cDefTime(object.getInteger(JSON_DELAY) + prevTimes) {
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
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>("Action", cDefAction));
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
