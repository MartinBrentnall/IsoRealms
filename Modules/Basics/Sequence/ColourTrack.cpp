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
#include "ColourTrack.h"

namespace IsoRealms::Basics {
  const std::string ColourTrack::JSON_DURATION = "duration";
  const std::string ColourTrack::JSON_EVENTS   = "events";
  const std::string ColourTrack::JSON_OUTPUT   = "outputs";
  const std::string ColourTrack::JSON_START    = "start";
  const std::string ColourTrack::JSON_TYPE     = "type";

  const std::string ColourTrack::TYPE_NAME          = "Colour";

  ColourTrack::ColourTrack(JSONObject object, IProject* project) :
            cDefName(object.getString(JSON_OUTPUT)),
            cDefInitColour(project, 1.0f, 0.0f, 0.0f, 0.0f, [this]() {stateChanged(&cDefInitColour);}),
            cRuntimeEvent(0),
            cRuntimeEventPosition(0),
            cStateNotifier(nullptr) {
    cDefInitColour.init(object, JSON_START);
    for (JSONObject mEventObject : object.getArray(JSON_EVENTS)) {
      std::string mEventType = mEventObject.getString(JSON_TYPE);
      if (mEventType == ColourTrackEventFade::EVENT_TYPE) {
        cDefEvents.push_back(std::make_unique<ColourTrackEventFade>(project, mEventObject.getInteger(JSON_DURATION), mEventObject));
      } else if (mEventType == ColourTrackEventPause::EVENT_TYPE) {
        cDefEvents.push_back(std::make_unique<ColourTrackEventPause>(mEventObject.getInteger(JSON_DURATION)));
      } else {
        throw ParseException("Unknown event type for Basics/Sequence/ColourTrack: " + mEventType);
      }
    }
  }

  void ColourTrack::registerAssets(IAssetRegistry* assets) {
    cStateNotifier = assets->add(this, cDefName, "Sequences");
  }
  
  void ColourTrack::unregisterAssets(IAssetRemover* assets) {
    assets->remove(this);
    cStateNotifier = nullptr;
  }

  void ColourTrack::save(JSONObject object) const {
    object.addString(JSON_TYPE, TYPE_NAME);
    object.addString(JSON_OUTPUT, cDefName);
    cDefInitColour.save(object, JSON_START);
    JSONArray mEventsArray = object.addArray(JSON_EVENTS);
    for (const std::unique_ptr<IColourTrackEvent>& mEvent : cDefEvents) {
      JSONObject mEventObject = mEventsArray.addObject();
      mEvent->save(mEventObject);
    }
  }

  bool ColourTrack::play(unsigned int milliseconds) {
    bool mStillPlaying = false;
    if (cRuntimeEvent < cDefEvents.size()) {
      cRuntimeEventPosition += milliseconds;
      int mRuntimeEventDuration = cDefEvents[cRuntimeEvent]->getDuration();
      while (cRuntimeEvent < cDefEvents.size() && cRuntimeEventPosition >= mRuntimeEventDuration) {
        cRuntimeEventPosition -= mRuntimeEventDuration;
        cRuntimeEvent++;
        if (cRuntimeEvent < cDefEvents.size()) {
          mRuntimeEventDuration = cDefEvents[cRuntimeEvent]->getDuration();
        } else {
          cRuntimeEventPosition = 0;
        }
      }
      mStillPlaying = true;
    }
    updateColour();
    return mStillPlaying;
  }

  void ColourTrack::reset() {
    cRuntimeEvent         = 0;
    cRuntimeEventPosition = 0;
    updateColour();
  }

  unsigned int ColourTrack::getDuration() const {
    unsigned int mDuration = 0;
    for (const std::unique_ptr<IColourTrackEvent>& mEvent : cDefEvents) {
      mDuration += mEvent->getDuration();
    }
    return mDuration;
  }

  void ColourTrack::render(float left, float bottom, float right, float top) const {
    unsigned int mTrackDuration = getDuration();
    unsigned int mEventDuration = 0;
    cDefInitColour.set();
    glBegin(GL_QUADS);
    float mLeft = left;

    // Render track
    for (const std::unique_ptr<IColourTrackEvent>& mEvent : cDefEvents) {
      mEventDuration += mEvent->getDuration();
      float mRight = (right - left) * (mEventDuration / static_cast<float>(mTrackDuration)) + left;
      glVertex2f(mLeft,  top);
      glVertex2f(mLeft,  bottom);
      const IColour* mColour = mEvent->getColour();
      if (mColour != nullptr) {
        mColour->set();
      }
      glVertex2f(mRight, bottom);
      glVertex2f(mRight, top);
      mLeft = mRight;
    }

    float mMiddle = bottom + (top - bottom) * 0.5f;

    // Render interaction points
    mLeft = left;
    mEventDuration = 0;
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(mLeft - 0.005f, mMiddle + 0.005f);
    glVertex2f(mLeft - 0.005f, mMiddle - 0.015f);
    glVertex2f(mLeft + 0.015f, mMiddle - 0.015f);
    glVertex2f(mLeft + 0.015f, mMiddle + 0.005f);

    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(mLeft - 0.01f, mMiddle + 0.01f);
    glVertex2f(mLeft - 0.01f, mMiddle - 0.01f);
    glVertex2f(mLeft + 0.01f, mMiddle - 0.01f);
    glVertex2f(mLeft + 0.01f, mMiddle + 0.01f);
    for (const std::unique_ptr<IColourTrackEvent>& mEvent : cDefEvents) {
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

  void ColourTrack::set() const {
    cRuntimeColour.set();
  }

  float ColourTrack::getRed() const {
    return cRuntimeColour.getRed();
  }

  float ColourTrack::getGreen() const {
    return cRuntimeColour.getGreen();
  }

  float ColourTrack::getBlue() const {
    return cRuntimeColour.getBlue();
  }

  float ColourTrack::getAlpha() const {
    return cRuntimeColour.getAlpha();
  }

  void ColourTrack::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void ColourTrack::updateColour() {
    const IColour* mPreviousEventColour = getPreviousColour();
    LiteralColour mPreviousColour = cRuntimeColour;
    if (cRuntimeEvent < cDefEvents.size()) {
      cRuntimeColour = cDefEvents[cRuntimeEvent]->getColour(std::max(0, cRuntimeEventPosition), mPreviousEventColour);
    } else if (mPreviousEventColour != nullptr) {
      cRuntimeColour = LiteralColour(*mPreviousEventColour);
    } else {
      cRuntimeColour = LiteralColour(1.0f, 0.0f, 0.0f);
    }

    if (cRuntimeColour != mPreviousColour) {
      cStateNotifier->stateChanged(this);
    }
  }

  const IColour* ColourTrack::getPreviousColour() {
    for (int i = cRuntimeEvent; i > 0; i--) {
      const IColour* mPreviousEventColour = cDefEvents[i - 1]->getColour();
      if (mPreviousEventColour != nullptr) {
        return mPreviousEventColour;
      }
    }
    return &cDefInitColour;
  }

  void ColourTrack::stateChanged(IColour* colour) {
    const IColour* mUsedColour = getPreviousColour();
    if (colour == mUsedColour) {
      updateColour();
      return;
    }
    
    if (cRuntimeEvent < cDefEvents.size() && (cRuntimeEvent > 0 || cRuntimeEventPosition > 0)) {
      int mRuntimeEvent = cRuntimeEventPosition == 0 ? cRuntimeEvent - 1 : cRuntimeEvent;
      mUsedColour = cDefEvents[mRuntimeEvent]->getColour();
      if (colour == mUsedColour) {
        updateColour();
        return;
      }
    }
  }
}
