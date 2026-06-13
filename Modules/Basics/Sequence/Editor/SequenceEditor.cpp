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
#include "SequenceEditor.h"

#include "Modules/Basics/Assets/Registry/SequenceTrackRegistry.h"
#include "Modules/Basics/Basics.h"
#include "Modules/Basics/Sequence/Sequence.h"

namespace IsoRealms::Basics {
  SequenceEditor::SequenceEditor(Sequence& sequence, IDialogManager& dialogManager) :
            cTimeFormatMillisecondExact([](long duration) {
              long mDays = duration / DURATION_DAY;
              duration %= DURATION_DAY;
              long mHours = duration / DURATION_HOUR;
              duration %= DURATION_HOUR;
              long mMinutes = duration / DURATION_MINUTE;
              duration %= DURATION_MINUTE;
              long mSeconds = duration / DURATION_SECOND;
              long mMilliseconds = duration % DURATION_SECOND;
              std::stringstream mStringStream;
              if (mDays > 0) {
                mStringStream << mDays << "-" << std::setfill('0') << std::setw(2) << mHours << ":" << mMinutes << ":" << mSeconds << "." << std::setfill('0') << std::setw(3) << mMilliseconds;
              } else if (mHours > 0) {
                mStringStream << mHours << ":" << std::setfill('0') << std::setw(2) << ":" << mMinutes << ":" << mSeconds << "." << std::setfill('0') << std::setw(3) << mMilliseconds;
              } else if (mMinutes > 0) {
                mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setfill('0') << std::setw(3) << mMilliseconds;
              } else if (mSeconds > 0) {
                mStringStream << mSeconds << "." << std::setw(3) << mMilliseconds;
              } else {
                mStringStream << mMilliseconds << "ms";
              }
              return mStringStream.str();
            }),
            cTimeFormatMillisecondPrecise([](long duration) {
              long mDays = duration / DURATION_DAY;
              duration %= DURATION_DAY;
              long mHours = duration / DURATION_HOUR;
              duration %= DURATION_HOUR;
              long mMinutes = duration / DURATION_MINUTE;
              duration %= DURATION_MINUTE;
              long mSeconds = duration / DURATION_SECOND;
              long mMilliseconds = duration % DURATION_SECOND;
              std::stringstream mStringStream;
              if (mDays > 0) {
                mStringStream << mDays << "-" << std::setfill('0') << std::setw(2) << mHours << ":" << mMinutes << ":" << mSeconds << "." << std::setfill('0') << std::setw(2) << (mMilliseconds / 10);
              } else if (mHours > 0) {
                mStringStream << mHours << ":" << std::setfill('0') << std::setw(2) << ":" << mMinutes << ":" << mSeconds << "." << std::setfill('0') << std::setw(2) << (mMilliseconds / 10);
              } else if (mMinutes > 0) {
                mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setfill('0') << std::setw(2) << (mMilliseconds / 10);
              } else if (mSeconds > 0) {
                mStringStream << mSeconds << "." << std::setw(2) << (mMilliseconds / 10);
              } else {
                mStringStream << mMilliseconds << "ms";
              }
              return mStringStream.str();
            }),
            cTimeFormatMillisecondRough([](long duration) {
              long mDays = duration / DURATION_DAY;
              duration %= DURATION_DAY;
              long mHours = duration / DURATION_HOUR;
              duration %= DURATION_HOUR;
              long mMinutes = duration / DURATION_MINUTE;
              duration %= DURATION_MINUTE;
              long mSeconds = duration / DURATION_SECOND;
              long mMilliseconds = duration % DURATION_SECOND;
              std::stringstream mStringStream;
              if (mDays > 0) {
                mStringStream << mDays << "-" << std::setfill('0') << std::setw(2) << mHours << ":" << mMinutes << ":" << mSeconds << "." << std::setfill('0') << std::setw(1) << (mMilliseconds / 100);
              } else if (mHours > 0) {
                mStringStream << mHours << ":" << std::setfill('0') << std::setw(2) << ":" << mMinutes << ":" << mSeconds << "." << std::setfill('0') << std::setw(1) << (mMilliseconds / 100);
              } else if (mMinutes > 0) {
                mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds << "." << std::setfill('0') << std::setw(1) << (mMilliseconds / 100);
              } else if (mSeconds > 0) {
                mStringStream << mSeconds << "." << std::setw(1) << (mMilliseconds / 100);
              } else {
                mStringStream << mMilliseconds << "ms";
              }
              return mStringStream.str();
            }),
            cTimeFormatSecond([](long duration) {
              long mDays = duration / DURATION_DAY;
              duration %= DURATION_DAY;
              long mHours = duration / DURATION_HOUR;
              duration %= DURATION_HOUR;
              long mMinutes = duration / DURATION_MINUTE;
              duration %= DURATION_MINUTE;
              long mSeconds = duration / DURATION_SECOND;
              std::stringstream mStringStream;
              if (mDays > 0) {
                mStringStream << mDays << "-" << std::setfill('0') << std::setw(2) << mHours << ":" << std::setfill('0') << std::setw(2) << mMinutes << ":" << std::setfill('0') << std::setw(2) <<mSeconds;
              } else if (mHours > 0) {
                mStringStream << mHours << ":" << std::setfill('0') << std::setw(2) << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds;
              } else if (mMinutes > 0) {
                mStringStream << mMinutes << ":" << std::setfill('0') << std::setw(2) << mSeconds;
              } else {
                mStringStream << mSeconds << "s";
              }
              return mStringStream.str();
            }),
            cTimeFormatMinute([](long duration) {
              long mDays = duration / DURATION_DAY;
              duration %= DURATION_DAY;
              long mHours = duration / DURATION_HOUR;
              duration %= DURATION_HOUR;
              long mMinutes = duration / DURATION_MINUTE;
              std::stringstream mStringStream;
              if (mDays > 0) {
                mStringStream << mDays << "-" << std::setfill('0') << std::setw(2) << mHours << ":" << mMinutes;
              } else if (mHours > 0) {
                mStringStream << mHours << ":" << std::setfill('0') << std::setw(2) << mMinutes;
              } else if (mMinutes > 0) {
                mStringStream << mMinutes << "m";
              }
              return mStringStream.str();
            }),
            cTimeFormatHour([](long duration) {
              long mDays = duration / DURATION_DAY;
              duration %= DURATION_DAY;
              long mHours = duration / DURATION_HOUR;
              std::stringstream mStringStream;
              if (mDays > 0) {
                mStringStream << mDays << "-" << std::setfill('0') << std::setw(2) << mHours;
              } else if (mHours > 0) {
                mStringStream << mHours << "h";
              }
              return mStringStream.str();
            }),
            cTimeFormatDay([](long duration) {
              long mDays = duration / DURATION_DAY;
              std::stringstream mStringStream;
              mStringStream << mDays << "d";
              return mStringStream.str();
            }),
            cHatHandler(sequence.getProject().getApplication().getHatHandler()),
            cSequence(sequence),
            cTimelinePan(0.0f),
            cTimelineZoomSpeed(0.0f),
            cTimelineZoom((cSequence.getDuration() == 0 ? DEFAULT_VISIBLE_DURATION : cSequence.getDuration()) / DEFAULT_VISIBLE_DURATION),
            cTimelineZoomStep(5.0f * std::log2(cTimelineZoom)),
            cCursorTrack(0),
            cCursorTimeline(0.0f),
            cTrackScrollOffset(0.0f),
            cCursorTrackProperties(false),
            cCursorTrackPropertiesAnimation(0),
            cMoveIndicatorTop(0.0f),
            cMoveIndicatorBottom(0.0f),
            cCursorTimelineSpeed(0.0f),
            cCursorEvent(nullptr),
            cMoveMode(false),
            cMoveAllMode(false),
            cMovingEvent(nullptr),
            cTrackLocks(cSequence.getTrackCount(), 0),
            cHasFocus(true),
            cEditingProperties(false),
            cPropertiesUI(sequence.getProject(), *this, [this]() {
              cEditingProperties = false;
            }, [](IEditable* editor) {
              std::cout << "WARNING: SequenceEditor::SequenceEditor: This UI does not support editables." << std::endl;
            }),
            cFont(nullptr),
            cFontSize(0.03f),
            cPreviewing(false),
            cPreviewPosition(0L),
            cMajorTicks{
              {10,                   10, cTimeFormatMillisecondPrecise}, // Ten milliseconds
              {20,                   2,  cTimeFormatMillisecondPrecise}, // Twenty milliseconds
              {50,                   5,  cTimeFormatMillisecondPrecise}, // Fifty milliseconds
              {100,                  10, cTimeFormatMillisecondRough},   // One hundred milliseconds
              {200,                  2,  cTimeFormatMillisecondRough},   // Two hundred milliseconds
              {500,                  5,  cTimeFormatMillisecondRough},   // Five hundred milliseconds
              {DURATION_SECOND,      10, cTimeFormatSecond},             // One second
              {DURATION_SECOND * 2,  2,  cTimeFormatSecond},             // Two seconds
              {DURATION_SECOND * 5,  5,  cTimeFormatSecond},             // Five seconds
              {DURATION_SECOND * 10, 10, cTimeFormatSecond},             // Ten seconds
              {DURATION_SECOND * 30, 3,  cTimeFormatSecond},             // Thirty seconds
              {DURATION_MINUTE,      6,  cTimeFormatMinute},             // One minute
              {DURATION_MINUTE * 2,  2,  cTimeFormatMinute},             // Two minutes
              {DURATION_MINUTE * 5,  5,  cTimeFormatMinute},             // Five minutes
              {DURATION_MINUTE * 10, 10, cTimeFormatMinute},             // Ten minutes
              {DURATION_MINUTE * 30, 3,  cTimeFormatMinute},             // Thirty minutes
              {DURATION_HOUR,        6,  cTimeFormatHour},               // One hour
              {DURATION_HOUR * 2,    2,  cTimeFormatHour},               // Two hours
              {DURATION_HOUR * 3,    3,  cTimeFormatHour},               // Three hours
              {DURATION_HOUR * 6,    6,  cTimeFormatHour},               // Six hours
              {DURATION_HOUR * 12,   12, cTimeFormatHour},               // Twelve hours
              {DURATION_HOUR * 24,   12, cTimeFormatDay},                // One day
            } {
  }

  void SequenceEditor::renderScreen(float scale, float aspectRatio) const {
    float mHeight = 0.08f;
    float mGap = 0.01f;
    float mX = -aspectRatio + mHeight + mGap;
    float mY = 1.0f;
    float mWidth = (aspectRatio * 2.0f - (mHeight + mGap));
    double mVisibleDuration = DEFAULT_VISIBLE_DURATION * cTimelineZoom;
    double mStartDuration = std::max(0.0, cCursorTimeline.animation() - mVisibleDuration / 2.0);
    Application& mApplication = cSequence.getProject().getApplication();

    // Calculate cursor Y position (with scroll offset applied).
    float mCursorY = (1.0f - mHeight / 2.0f) - cCursorTrack.animation() * (mHeight + 0.01f) - cTrackScrollOffset.animation();

    // Render tracks.
    mY = 1.0f - cTrackScrollOffset.animation();
    ScreenArea mPreviousCrop = mApplication.crop(ScreenArea(mX, aspectRatio - 0.001f, -1.0f, 1.0f));
    for (unsigned int i = 0; i < cSequence.getTrackCount(); i++) {
      SequenceTrack& mTrack = cSequence.getTrack(i);
      glPushMatrix();
      glTranslatef(-mStartDuration * mWidth / mVisibleDuration, 0.0f, 0.0f);
      mTrack->render(mX, mY - mHeight, mX + mWidth, mY, mStartDuration, mStartDuration + mVisibleDuration);
      glPopMatrix();

      // Bottom shadow.
      glEnable(GL_BLEND);
      glBegin(GL_QUADS);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
      glVertex2f(mX + mWidth, mY);
      glVertex2f(mX,          mY);
      glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
      glVertex2f(mX,          mY - mHeight * 0.2f);
      glVertex2f(mX + mWidth, mY - mHeight * 0.2f);

      // Top shine.
      glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
      glVertex2f(mX + mWidth, mY - mHeight * 0.8f);
      glVertex2f(mX,          mY - mHeight * 0.8f);
      glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
      glVertex2f(mX,          mY - mHeight);
      glVertex2f(mX + mWidth, mY - mHeight);
      glEnd();
      glDisable(GL_BLEND);

      mY -= mHeight + mGap;
    }
    mApplication.crop(mPreviousCrop);

    // Render controller cursor.
    double mCursorX = std::min(mX + cCursorTimeline.animation() * mWidth / mVisibleDuration, static_cast<double>(mX + mWidth / 2.0f));

    // Render move indicator.
    if (cMoveIndicatorBottom.animation() != cMoveIndicatorTop.animation()) {
      float mMoveIndicatorBottom = (1.0f - mHeight / 2.0f) - cMoveIndicatorBottom.animation() * (mHeight + 0.01f) - cTrackScrollOffset.animation();
      float mMoveIndicatorTop    = (1.0f - mHeight / 2.0f) - cMoveIndicatorTop.animation()    * (mHeight + 0.01f) - cTrackScrollOffset.animation();
      glColor3f(0.0f, 1.0f, 0.0f);
      Utils::renderCircle(mCursorX, mMoveIndicatorBottom, 0.01f);
      Utils::renderCircle(mCursorX, mMoveIndicatorTop,    0.01f);
      glEnable(GL_BLEND);
      glBegin(GL_QUADS);
      glVertex2f(mCursorX,         mMoveIndicatorBottom);
      glVertex2f(mCursorX,         mMoveIndicatorTop);
      glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
      glVertex2f(mCursorX - 0.01f, mMoveIndicatorTop);
      glVertex2f(mCursorX - 0.01f, mMoveIndicatorBottom);
      glVertex2f(mCursorX + 0.01f, mMoveIndicatorBottom);
      glVertex2f(mCursorX + 0.01f, mMoveIndicatorTop);
      glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
      glVertex2f(mCursorX,         mMoveIndicatorTop);
      glVertex2f(mCursorX,         mMoveIndicatorBottom);
      glEnd();
      glDisable(GL_BLEND);
    }

    float mHighlightSize = cCursorTrackPropertiesAnimation / 200.0f;
    float mCursorSize    = -mHighlightSize + 1.0f;

    if (mCursorSize > 0.0f) {
      glColor3f(1.0f, 0.0f, 0.2f);
      Utils::renderCircle(mCursorX, mCursorY, 0.02f  * mCursorSize);
      glColor3f(1.0f, 1.0f, 1.0f);
      Utils::renderCircle(mCursorX, mCursorY, 0.015f * mCursorSize);
    }

    if (mHighlightSize > 0.0f) {
      glColor3f(1.0f, 0.0f, 0.2f);
      mY = 1.0f - cCursorTrack.animation() * (mHeight + mGap) - (mHeight / 2.0f) - cTrackScrollOffset.animation();
      Utils::renderRoundedRectangle((-aspectRatio + mHeight) - (mHeight * mHighlightSize), mY - (mHeight / 2.0f) * mHighlightSize, -aspectRatio + mHeight, mY + (mHeight / 2.0f) * mHighlightSize, mHeight * 0.2f * mHighlightSize);
    }

    // Render icons.
    for (unsigned int i = 0; i < cSequence.getTrackCount(); i++) {
      SequenceTrack& mTrack = cSequence.getTrack(i);
      glPushMatrix();
      glTranslatef(-aspectRatio + mHeight * 0.5f, (1.0f - mHeight * 0.5f) - i * (mHeight + mGap) - cTrackScrollOffset.animation(), 0.0f);
      glScalef(mHeight * 0.5f, mHeight * 0.5f, 0.0f);
      mTrack->renderIcon();
      glPopMatrix();

      if (cTrackLocks[i] == 1) {
        glPushMatrix();
        glTranslatef(-aspectRatio + mHeight * 0.75f, (1.0f - mHeight * 0.75f) - i * (mHeight + mGap), 0.0f);
        glScalef(mHeight * 0.25f, mHeight * 0.25f, 0.0f);
        Utils::renderIconLock();
        glPopMatrix();
      }
      mY -= mHeight + mGap;
    }

    glPushMatrix();
    glTranslatef(-aspectRatio + mHeight * 0.5f, (1.0f - mHeight * 0.5f) - cSequence.getTrackCount() * (mHeight + mGap) - cTrackScrollOffset.animation(), 0.0f);
    glScalef(mHeight * 0.5f, mHeight * 0.5f, 0.0f);
    Utils::renderIconNone();
    glPopMatrix();

    // Render event handles.
    mY = 1.0f - cTrackScrollOffset.animation();
    glPushMatrix();
    glTranslatef(-mStartDuration * mWidth / mVisibleDuration, 0.0f, 0.0f);
    for (unsigned int i = 0; i < cSequence.getTrackCount(); i++) {
      SequenceTrack& mTrack = cSequence.getTrack(i);
      std::vector<ISequenceTrackEvent*> mEvents = mTrack->getEvents();
      float mMiddle = mY - mHeight / 2.0f;
      for (ISequenceTrackEvent* mEvent : mEvents) {
        float mRight = ((mX + mWidth) - mX) * (mEvent->getTime() / static_cast<float>(mVisibleDuration)) + mX;
        glColor3f(0.0f, 0.0f, 0.0f);
        Utils::renderCircle(mRight + 0.005f, mMiddle - 0.005f, 0.015f);
        if (mEvent == cCursorEvent && !cCursorTrackProperties) {
          glColor3f(0.5f, 0.0f, 1.0f);
        } else {
          glColor3f(1.0f, 1.0f, 1.0f);
        }
        Utils::renderCircle(mRight,          mMiddle,          0.015f);
      }
      mY -= mHeight + mGap;
    }

    // Render preview position.
    if (cPreviewing) {
      float mPreviewX = ((mX + mWidth) - mX) * (cPreviewPosition / static_cast<float>(mVisibleDuration)) + mX;
      float mMoveIndicatorBottom = (1.0f - mHeight / 2.0f) - (cSequence.getTrackCount() - 0.5f) * (mHeight + 0.01f) - cTrackScrollOffset.animation();
      float mMoveIndicatorTop    = (1.0f - mHeight / 2.0f) +                              0.5f  * (mHeight + 0.01f) - cTrackScrollOffset.animation();
      glEnable(GL_BLEND);
      glBegin(GL_QUADS);
      glColor3f(0.8f, 1.0f, 1.0f);
      glVertex2f(mPreviewX,         mMoveIndicatorBottom);
      glVertex2f(mPreviewX,         mMoveIndicatorTop);
      glColor4f(0.0f, 1.0f, 1.0f, 0.0f);
      glVertex2f(mPreviewX - 0.01f, mMoveIndicatorTop);
      glVertex2f(mPreviewX - 0.01f, mMoveIndicatorBottom);
      glVertex2f(mPreviewX + 0.01f, mMoveIndicatorBottom);
      glVertex2f(mPreviewX + 0.01f, mMoveIndicatorTop);
      glColor4f(0.8f, 1.0f, 1.0f, 1.0f);
      glVertex2f(mPreviewX,         mMoveIndicatorTop);
      glVertex2f(mPreviewX,         mMoveIndicatorBottom);
      glEnd();
      glDisable(GL_BLEND);
    }

    // Render timeline labels and ticks.
    const TickConfiguration& mTickConfiguration = getTickConfiguration(mVisibleDuration);

    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);

    long mStartTick = Utils::round(static_cast<long>(cCursorTimeline.value() - mVisibleDuration), mTickConfiguration.cMajorTickSpacing, -1);

    for (long i = mStartTick; i < cCursorTimeline.value() + mVisibleDuration; i += mTickConfiguration.cMajorTickSpacing) {
      double mRight = ((mX + mWidth) - mX) * (i / static_cast<double>(mVisibleDuration)) + mX;
      glVertex2f(mRight, mY);
      glVertex2f(mRight, mY - mHeight / 2.0f);
      for (int j = 0; j < mTickConfiguration.cMinorTicks; j++) {
        double mMinorOffset = j / static_cast<double>(mTickConfiguration.cMinorTicks) * mTickConfiguration.cMajorTickSpacing;
        mRight = ((mX + mWidth) - mX) * ((i + mMinorOffset) / static_cast<double>(mVisibleDuration)) + mX;
        float mShortener = (mTickConfiguration.cMinorTicks % 2 == 0 && j == mTickConfiguration.cMinorTicks / 2) ? 4.0f : 8.0f;
        glVertex2f(mRight, mY);
        glVertex2f(mRight, mY - mHeight / mShortener);
      }
    }
    glEnd();

    for (long i = mStartTick; i < cCursorTimeline.value() + mVisibleDuration; i += mTickConfiguration.cMajorTickSpacing) {
      double mRight = ((mX + mWidth) - mX) * (i / static_cast<double>(mVisibleDuration)) + mX;
      cFont->print(mRight, mY - mHeight, 0.02f, IFont::Alignment::CENTER, mTickConfiguration.cTimeFormat(i));
    }

    glPopMatrix();

    // Render current time.
    cFont->print( aspectRatio - cFontSize, -1.0f + cFontSize, cFontSize, IFont::Alignment::RIGHT, cTimeFormatMillisecondExact(cCursorTimeline.value()));
    cFont->print(-aspectRatio + cFontSize, -1.0f + cFontSize, cFontSize, IFont::Alignment::LEFT,  cCursorTrack.value() < static_cast<int>(cSequence.getTrackCount()) ? cSequence.getTrack(cCursorTrack.value())->getName() : "");

    glBindTexture(GL_TEXTURE_2D, 0);
    cPropertiesUI.render(aspectRatio);
  }

  bool SequenceEditor::renderAssetIcon() const {
    return false; // TODO: What's this used for?
  }

  void SequenceEditor::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void SequenceEditor::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool SequenceEditor::isDefaultConfiguration() const {
    return true;
  }

  void SequenceEditor::updateScreen(unsigned int milliseconds) {
    cCursorTimeline = std::max(cCursorTimeline.value() + cCursorTimelineSpeed * cTimelineZoom, 0.0);
    unsigned int mOldTimeline = std::round(cCursorTimeline.animation());
    cCursorTrack.update(milliseconds);
    cCursorTimeline.update(milliseconds);
    cMoveIndicatorBottom.update(milliseconds);
    cMoveIndicatorTop.update(milliseconds);
    
    // Update scroll offset based on cursor Y position.
    float mHeight = 0.08f;
    float mGap = 0.01f;
    float mTotalHeight = (cSequence.getTrackCount() + 1) * (mHeight + mGap);
    float mVisibleHeight = 2.0f; // From -1.0 to 1.0
    
    if (mTotalHeight > mVisibleHeight) {
      // Calculate cursor Y position without scroll offset.
      float mCursorYNoScroll = (1.0f - mHeight / 2.0f) - cCursorTrack.animation() * (mHeight + mGap);
      
      // Calculate desired scroll offset to keep cursor within margins.
      float mDesiredScrollOffset = cTrackScrollOffset.value();
      float mCurrentCursorY = mCursorYNoScroll - mDesiredScrollOffset;
      
      // If cursor is near top, scroll down (increase offset) to bring it down.
      if (mCurrentCursorY > (1.0f - SCROLL_MARGIN)) {
        float mTargetY = 1.0f - SCROLL_MARGIN;
        mDesiredScrollOffset = mCursorYNoScroll - mTargetY;
      }
      
      // If cursor is near bottom, scroll up (decrease offset) to bring it up.
      if (mCurrentCursorY < (-1.0f + SCROLL_MARGIN)) {
        float mTargetY = -1.0f + SCROLL_MARGIN;
        mDesiredScrollOffset = mCursorYNoScroll - mTargetY;
      }
      
      // Clamp scroll offset so tracks don't scroll past boundaries (offset >= 0 means we can't scroll above first track).
      float mMaxScroll = mTotalHeight - mVisibleHeight;
      mDesiredScrollOffset = std::clamp(mDesiredScrollOffset, -mMaxScroll, 0.0f);
      
      cTrackScrollOffset = mDesiredScrollOffset;
    } else {
      // No scrolling needed if all tracks fit on screen.
      cTrackScrollOffset = 0.0f;
    }
    
    cTrackScrollOffset.update(milliseconds);
    if (cTimelineZoomSpeed != 0.0f) {
      cTimelineZoomStep = std::clamp(cTimelineZoomStep + cTimelineZoomSpeed, ZOOM_LIMIT_MINIMUM, ZOOM_LIMIT_MAXIMUM);
      cTimelineZoom = std::pow(2.0f, cTimelineZoomStep / 5.0f);
    }

    // Check which event the cursor is closest to.
    updateSelectedEvent();

    unsigned int mNewTimeline = std::round(cCursorTimeline.animation());
    int mMovement = mNewTimeline - mOldTimeline;
    if (cMoveMode && !cCursorTrackProperties && cCursorTrack.value() < static_cast<int>(cSequence.getTrackCount())) {
      cMoveIndicatorBottom = cCursorTrack.value() + 0.5f;
      cMoveIndicatorTop    = cCursorTrack.value() - 0.5f;
      std::vector<ISequenceTrackEvent*> mEvents = cSequence.getTrack(cCursorTrack.value())->getEvents();
      for (ISequenceTrackEvent* mEvent : mEvents) {
        if (mEvent->getTime() > mOldTimeline) {
          mEvent->setTime(mEvent->getTime() + mMovement);
        }
      }
    } else if (cMovingEvent != nullptr) {
      cSequence.getTrack(cCursorTrack.value())->setEventTime(cMovingEvent, cMovingEvent->getTime() + mMovement);
    } else if (cMoveAllMode && !cCursorTrackProperties && cCursorTrack.value() < static_cast<int>(cSequence.getTrackCount())) {
      cMoveIndicatorBottom = cSequence.getTrackCount() - 0.5f;
      cMoveIndicatorTop    =                           - 0.5f;
      for (unsigned int i = 0; i < cSequence.getTrackCount(); i++) {
        if (cTrackLocks[i] == 0) {
          for (ISequenceTrackEvent* mEvent : cSequence.getTrack(i)->getEvents()) {
            if (mEvent->getTime() > mOldTimeline) {
              mEvent->setTime(mEvent->getTime() + mMovement);
            }
          }
        }
      }
    } else {
      cMoveIndicatorBottom = cCursorTrack.value();
      cMoveIndicatorTop    = cCursorTrack.value();
    }
    cPropertiesUI.update(milliseconds);

    if (cCursorTrackProperties || cCursorTrack.value() == static_cast<int>(cSequence.getTrackCount())) {
      cCursorTrackPropertiesAnimation = std::min(200, cCursorTrackPropertiesAnimation + static_cast<int>(milliseconds));
    } else {
      cCursorTrackPropertiesAnimation = std::max(0, cCursorTrackPropertiesAnimation - static_cast<int>(milliseconds));
    }

    if (cPreviewing) {
      cSequence.preview(milliseconds);
      cPreviewPosition += milliseconds;
    }
  }

  bool SequenceEditor::input(sf::Event& event) {
    if (cHasFocus) {
      if (cEditingProperties) {
        if (cPropertiesUI.input(event)) {
          return true;
        }
      } else switch (event.type) {
        case sf::Event::MouseButtonPressed: {
          switch (event.mouseButton.button) {
            case sf::Mouse::Button::Left: {
              // TODO: Implement this.
              return true;
            }

            default: {
              break;
            }
          }
          break;
        }

        case sf::Event::MouseButtonReleased: {
          // TODO: Implement this.
          return true;
        }

        case sf::Event::MouseWheelScrolled: {
          switch (event.mouseWheelScroll.wheel) {
            case sf::Mouse::VerticalWheel: {
              // TODO: Implement this.
            }

            default : {
              break;
            }
          }
          break;
        }

        case sf::Event::MouseMoved: {
          // TODO: Implement this.
        }

        case sf::Event::KeyPressed: {
          switch (event.key.code) {
            case sf::Keyboard::F1: {
              // TODO: Implement this.
              break;
            }

            case sf::Keyboard::Escape: {
              cExitAction->execute();
              break;
            }

            default: {
              break;
            }
          }
          return true;
        }

        case sf::Event::KeyReleased: {
          switch (event.key.code) {
            case sf::Keyboard::LControl:
            case sf::Keyboard::RControl: {
              // TODO: Implement this.
              break;
            }

            default: {
              break;
            }
          }
          return true;
        }

        case sf::Event::JoystickMoved: {
          if (cHatHandler.upPressed()) {
            if (cCursorTrack.value() > 0) {
              cCursorTrack -= 1;
            }
            return true;
          }
          if (cHatHandler.downPressed()) {
            if (cCursorTrack.value() < static_cast<int>(cSequence.getTrackCount())) {
              cCursorTrack += 1;
            }
            return true;
          }
          if (cHatHandler.leftPressed()) {
            if (cCursorTrack.value() < static_cast<int>(cSequence.getTrackCount())) {
              if (cMovingEvent != nullptr) {
                cCursorTimeline = getPreviousAlignedEventTime();
              } else {
                if (cCursorTimeline.value() == 0) {
                  cCursorTrackProperties = true;
                } else {
                  cCursorTimeline = getPreviousEventTime();
                }
              }
            }
            return true;
          }
          if (cHatHandler.rightPressed()) {
            if (cCursorTrack.value() < static_cast<int>(cSequence.getTrackCount())) {
              if (cMovingEvent != nullptr) {
                cCursorTimeline = getNextAlignedEventTime();
              } else {
                if (cCursorTrackProperties) {
                  cCursorTrackProperties = false;
                } else {
                  cCursorTimeline = getNextEventTime();
                }
              }
            }
            return true;
          }

          float mAmount = std::abs(event.joystickMove.position) < ANALOGUE_INPUT_DEAD_ZONE ? 0 : (event.joystickMove.position - (event.joystickMove.position < 0 ? -ANALOGUE_INPUT_DEAD_ZONE : ANALOGUE_INPUT_DEAD_ZONE)) * (100.0f / (100.0f - ANALOGUE_INPUT_DEAD_ZONE));
          switch (event.joystickMove.axis) {
            case 0: {
              if (!cCursorTrackProperties) {
                cCursorTimelineSpeed = mAmount * 50.0f;
              }
              break;
            }

            case 1: {
              // TODO: Implement this.
              break;
            }

            case 5: {
              cTimelineZoomSpeed = mAmount / 500.0f;
              break;
            }

            default: {
              break;
            }
          }
          return true;
        }

        case sf::Event::JoystickButtonPressed: {
          switch (event.joystickButton.button) {
            case 0: {
              if (cCursorTrack.value() == static_cast<int>(cSequence.getTrackCount())) {
                cSequence.addTrack();
                cTrackLocks.emplace_back(0);
                cCursorTrackProperties = true;
                cCursorTimeline = 0.0f;
              }

              if (cCursorTrackProperties) {
                cPropertiesUI.openUI(std::make_unique<PropertiesMenu>(cPropertiesUI, *this, cSequence.getResourceData(), [this](IPropertyMaker& owner) {
                  owner.createPropertyTreeSelector(SequenceTrackRegistry::JSON_KEY, cSequence.getTrack(cCursorTrack.value()));
                  owner.createPropertyNativeString("name",             [this]() {
                    return cSequence.getTrack(cCursorTrack.value())->getName();
                  }, [this](const std::string& name) {
                    cSequence.getTrack(cCursorTrack.value())->setName(name);
                  }, "", [this](const std::string& name) {
                    for (int i = 0; i < static_cast<int>(cSequence.getTrackCount()); i++) {
                      if (i != cCursorTrack.value() && name == cSequence.getTrack(i)->getName()) {
                        return false;
                      }
                    }
                    return true;
                  });
                }), "Track Configuration");
                cEditingProperties = true;
              } else {
                if (cCursorEvent == nullptr) {
                  cCursorEvent = cSequence.getTrack(cCursorTrack.value())->createEvent(cSequence.getResourceData(), std::round(cCursorTimeline.value()));
                }

                if (cCursorEvent != nullptr) {
                  cPropertiesUI.openUI(std::make_unique<PropertiesMenu>(cPropertiesUI, *this, cSequence.getResourceData(), [this](IPropertyMaker& owner) {
                    return cCursorEvent->getEventProperties(owner);
                  }), "Event Configuration");
                  cEditingProperties = true;
                }
              }
              break;
            }

            case 1: {
              if (cCursorTrackProperties) {
                cTrackLocks[cCursorTrack.value()] = cTrackLocks[cCursorTrack.value()] == 0 ? 1 : 0;
              } else {
                if (cCursorEvent != nullptr) {
                  cCursorTimeline.init(cCursorEvent->getTime());
                  cMovingEvent = cCursorEvent;
                }
              }
              break;
            }

            case 2: {
              cSequence.stopPreview();
              cSequence.setPreviewPosition(cCursorTimeline.value());
              cPreviewing = true;
              cPreviewPosition = cCursorTimeline.value();
              break;
            }

            case 3: {
              cSequence.stopPreview();
              cPreviewing = false;
              break;
            }

            case 4: {
              cMoveMode = true;
              break;
            }

            case 5: {
              cMoveAllMode = true;
              break;
            }

            case 6: {
              if (cCursorTrack.value() < static_cast<int>(cSequence.getTrackCount())) {
                if (cCursorTrackProperties) {
                  cEditingProperties = true;
                  cPropertiesUI.confirm("Are you sure you want to delete this track?", [this]() {
                    cSequence.deleteTrack(cCursorTrack.value());
                    cTrackLocks.erase(cTrackLocks.begin() + cCursorTrack.value());
                    cEditingProperties = false;
                  }, [this]() {
                    cEditingProperties = false;
                  });
                } else if (cCursorEvent != nullptr) {
                  cSequence.getTrack(cCursorTrack.value())->deleteEvent(cCursorEvent);
                  }
                }
              }
              break;
            case 9: {
              cExitAction->execute();
              break;
            }

            default: {
              break;
            }
          }
          return true;
        }

        case sf::Event::JoystickButtonReleased: {
          switch (event.joystickButton.button) {
            case 1: {
              cMovingEvent = nullptr;
              break;
            }

            case 4: {
              cMoveMode = false;
              break;
            }

            case 5: {
              cMoveAllMode = false;
              break;
            }
          }
        }

        default: {
          break;
        }
      }
    }
    return false;
  }

  void SequenceEditor::resetInput() {
    // Nothing to do.
  }

  void SequenceEditor::notifyVisible() {
    // Nothing to do.
  }
  
  void SequenceEditor::notifyHidden() {
    // Nothing to do.
  }
  
  void SequenceEditor::notifyLostFocus() {
    cHasFocus = false;
  }

  void SequenceEditor::notifyGainedFocus() {
    cHasFocus = true;
  }

  std::vector<std::string> SequenceEditor::getDigitalInputs() const {
    std::vector<std::string> mDigitalInputNames;
    return mDigitalInputNames;
  }

  std::vector<std::string> SequenceEditor::getAnalogueInputs() const {
    std::vector<std::string> mAnalogueInputNames;
    return mAnalogueInputNames;
  }

  void SequenceEditor::setDigitalInput(const std::string& name, IBoolean* input) {
    // Nothing to do.
  }

  void SequenceEditor::setAnalogueInput(const std::string& name, IFloat* input) {
    // Nothing to do.
  }

  void SequenceEditor::setExitAction(IAction* action) {
    cExitAction = action;
  }

  void SequenceEditor::setAppearance(IFont* font, float scale) {
    cFont = font;
    cFontSize = scale;
  }

  IScreen* SequenceEditor::screen() {
    return this;
  }
  
  IFont* SequenceEditor::getFont() const {
    return cFont;
  }
  
  float SequenceEditor::getFontSize() const {
    return cFontSize;;
  }
  
  IFont* SequenceEditor::getCodeFont() const {
    return cFont;
  }
  
  float SequenceEditor::getCodeFontSize() const {
    return cFontSize;
  }
  
  IsoRealms::Project& SequenceEditor::getProject() const {
    return cSequence.getProject();
  }

  bool SequenceEditor::isEvent(unsigned int time) const {
    std::vector<ISequenceTrackEvent*> mEvents = cSequence.getTrack(cCursorTrack.value())->getEvents();
    for (ISequenceTrackEvent* mEvent : mEvents) {
      if (time == mEvent->getTime()) {
        return true;
      }
    }
    return false;
  }

  unsigned int SequenceEditor::getPreviousAlignedEventTime() const {
    unsigned int mTime = 0U;
    for (unsigned int i = 0; i < cSequence.getTrackCount(); i++) {
      if (cCursorTrack.value() != static_cast<int>(i)) {
        std::vector<ISequenceTrackEvent*> mEvents = cSequence.getTrack(i)->getEvents();
        for (ISequenceTrackEvent* mEvent : mEvents) {
          if (mEvent->getTime() < cCursorTimeline.value() && mEvent->getTime() > mTime && !isEvent(mEvent->getTime())) {
            mTime = mEvent->getTime();
          }
        }
      }
    }
    return mTime == 0U ? cCursorTimeline.value() : mTime;
  }

  unsigned int SequenceEditor::getNextAlignedEventTime() const {
    unsigned int mTime = cSequence.getDuration();
    for (unsigned int i = 0; i < cSequence.getTrackCount(); i++) {
      if (cCursorTrack.value() != static_cast<int>(i)) {
        std::vector<ISequenceTrackEvent*> mEvents = cSequence.getTrack(i)->getEvents();
        for (ISequenceTrackEvent* mEvent : mEvents) {
          if (mEvent->getTime() > cCursorTimeline.value() && mEvent->getTime() < mTime && !isEvent(mEvent->getTime())) {
            mTime = mEvent->getTime();
          }
        }
      }
    }
    return mTime;
  }

  unsigned int SequenceEditor::getPreviousEventTime() const {
    unsigned int mTime = 0U;
    std::vector<ISequenceTrackEvent*> mEvents = cSequence.getTrack(cCursorTrack.value())->getEvents();
    for (ISequenceTrackEvent* mEvent : mEvents) {
      unsigned int mOldTime = mTime;
      mTime = mEvent->getTime();
      if (mTime >= cCursorTimeline.value()) {
        return mOldTime;
      }
    }
    return mTime;
  }

  unsigned int SequenceEditor::getNextEventTime() const {
    unsigned int mTime = 0U;
    std::vector<ISequenceTrackEvent*> mEvents = cSequence.getTrack(cCursorTrack.value())->getEvents();
    for (ISequenceTrackEvent* mEvent : mEvents) {
      mTime = mEvent->getTime();
      if (mTime > cCursorTimeline.value()) {
        return mTime;
      }
    }
    return cCursorTimeline.value();
  }

  void SequenceEditor::updateSelectedEvent() {
    cCursorEvent = nullptr;
    if (cCursorTrack.value() < static_cast<int>(cSequence.getTrackCount())) {
      std::vector<ISequenceTrackEvent*> mEvents = cSequence.getTrack(cCursorTrack.value())->getEvents();
      float mClosestDistance = 10000.0f;
      for (ISequenceTrackEvent* mEvent : mEvents) {
        float mEventDistance = std::abs(mEvent->getTime() - cCursorTimeline.animation()) / cTimelineZoom;
        if (mEventDistance < mClosestDistance) {
          cCursorEvent = mEvent;
          mClosestDistance = mEventDistance;
        }
      }
    }
  }

  const SequenceEditor::TickConfiguration& SequenceEditor::getTickConfiguration(float duration) const {
    float mThreshold = duration / MAX_VISIBLE_MAJOR_TICKS;
//    std::cout << "Threshold: " << mThreshold << std::endl;
    for (const TickConfiguration& mTickSpacingOption : cMajorTicks) {
//      std::cout << "CHECKING: (" << mTickSpacingOption.cMajorTickSpacing << " < " << mThreshold << ")" << std::endl;
      if (mTickSpacingOption.cMajorTickSpacing > mThreshold) {
//        std::cout << "RETURNING: " << mTickSpacingOption.cMajorTickSpacing << std::endl;
        return mTickSpacingOption;
      }
    }
    return cMajorTicks[0];
  }
}
