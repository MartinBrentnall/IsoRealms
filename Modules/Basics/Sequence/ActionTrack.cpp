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
#include "ActionTrack.h"

namespace IsoRealms::Basics {
  const std::string ActionTrack::TAG_ACTION      = "Action";

  const std::string ActionTrack::ATTRIBUTE_DELAY = "delay";

  const std::string ActionTrack::TYPE_NAME       = "Action";

  ActionTrack::ActionTrack(DOMNode& node, IProject* project) {
    for (DOMNode& mChild : node) {
      std::string mChildName = mChild.getName();
      if (mChildName == TAG_ACTION) {
        cDefEvents.emplace_back(std::make_unique<ActionEvent>(mChild, project));
      }
    }
    reset();
  }

  void ActionTrack::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }

  void ActionTrack::unregisterAssets(IAssetRemover* assets) {
    // Nothing to do.
  }

  void ActionTrack::save(DOMNodeWriter* node) const {
    DOMNodeWriter mTrackNode = node->addBranch(TYPE_NAME);
    for (const std::unique_ptr<ActionEvent>& mEvent : cDefEvents) {
      DOMNodeWriter mEventNode = mTrackNode.addBranch(TAG_ACTION);
      mEvent->save(&mEventNode);
    }
  }

  bool ActionTrack::play(unsigned int milliseconds) {
    if (cRuntimeAction < cDefEvents.size()) {
      std::vector<ActionEvent*> mEventToExecute;
      while (milliseconds > cRuntimeActionRemaining) {
        mEventToExecute.emplace_back(cDefEvents[cRuntimeAction].get());
        cRuntimeAction++;
        if (cRuntimeAction == cDefEvents.size()) {
          break;
        }
        milliseconds -= cRuntimeActionRemaining;
        cRuntimeActionRemaining = cDefEvents[cRuntimeAction]->getDuration();
      }
      cRuntimeActionRemaining -= milliseconds;

      // Execute actions at the end in case one triggers a modification to the state of the sequence.
      for (ActionEvent* mEvent : mEventToExecute) {
        mEvent->execute();
      }
      return true;
    }
    return false;
  }

  void ActionTrack::reset() {
    cRuntimeAction = 0;
    cRuntimeActionRemaining = cDefEvents.empty() ? 0 : cDefEvents[cRuntimeAction]->getDuration();
  }

  unsigned int ActionTrack::getDuration() const {
    unsigned int mDuration = 0;
    for (const std::unique_ptr<ActionEvent>& mEvent : cDefEvents) {
      mDuration += mEvent->getDuration();
    }
    return mDuration;
  }

  void ActionTrack::render(float left, float bottom, float right, float top) const {

    // Render a nice background
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.0f, 0.0f);
    glVertex2f(right, top);
    glVertex2f(left,  top);
    glColor3f(0.2f, 0.0f, 0.0f);
    glVertex2f(left,  bottom);
    glVertex2f(right, bottom);

    // Render interaction points
    float mMiddle = bottom + (top - bottom) * 0.5f;
    unsigned int mEventDuration = 0;
    unsigned int mTrackDuration = getDuration();

    for (const std::unique_ptr<ActionEvent>& mEvent : cDefEvents) {
      mEventDuration += mEvent->getDuration();
      float mRight = (right - left) * (mEventDuration / static_cast<float>(mTrackDuration)) + left;
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex2f(mRight - 0.005f, mMiddle + 0.005f);
      glVertex2f(mRight - 0.005f, mMiddle - 0.015f);
      glVertex2f(mRight + 0.015f, mMiddle - 0.015f);
      glVertex2f(mRight + 0.015f, mMiddle + 0.005f);

      glColor3f(1.0f, 1.0f, 1.0f);
      glVertex2f(mRight - 0.01f, mMiddle + 0.01f);
      glVertex2f(mRight - 0.01f, mMiddle - 0.01f);
      glVertex2f(mRight + 0.01f, mMiddle - 0.01f);
      glVertex2f(mRight + 0.01f, mMiddle + 0.01f);
    }
    glEnd();
  }

  ActionTrack::ActionEvent::ActionEvent(DOMNode& node, IProject* project) :
            cDefAction(project),
            cDefDuration(node.getIntegerAttribute(ATTRIBUTE_DELAY)) {
    cDefAction.init(node, TAG_ACTION);
  }
  
  void ActionTrack::ActionEvent::save(DOMNodeWriter* node) const {
    node->addAttribute(ATTRIBUTE_DELAY, cDefDuration);
    cDefAction.save(node, TAG_ACTION);
  }

  unsigned int ActionTrack::ActionEvent::getDuration() const {
    return cDefDuration;
  }

  void ActionTrack::ActionEvent::execute() {
    cDefAction.execute();
  }
}
