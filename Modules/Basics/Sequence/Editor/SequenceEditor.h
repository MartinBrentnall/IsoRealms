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
#pragma once

#include <GL/glew.h>

#include "IsoRealms.h"

namespace IsoRealms::Basics {
  class Sequence;
  class ISequenceTrackEvent;

  class SequenceEditor : public IEditableScreen,
                         public IUIStyle {
    public:
    SequenceEditor(Sequence& sequence, IDialogManager& dialogManager);
    void updateScreen(unsigned int milliseconds);

    /******************************\
     * Implements IEditableScreen *
    \******************************/
    bool input(sf::Event& event) override;
    void resetInput() override;
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;
    void notifyVisible() override;
    void notifyHidden() override;
    void notifyLostFocus() override;
    void notifyGainedFocus() override;
    std::vector<std::string> getDigitalInputs() const override;
    std::vector<std::string> getAnalogueInputs() const override;
    void setDigitalInput(const std::string& name, IBoolean* input) override;
    void setAnalogueInput(const std::string& name, IFloat* input) override;
    void setExitAction(IAction* action) override;
    void setAppearance(IFont* font, float scale) override;
    IScreen* screen() override;
    
    /*******************************\
     * Implements IUIConfiguration *
    \*******************************/
    IFont* getFont() const override;
    float getFontSize() const override;
    IFont* getCodeFont() const override;
    float getCodeFontSize() const override;
    IsoRealms::Project& getProject() const override;
      
    private:
    inline static const float ZOOM_LIMIT_MAXIMUM = 28.0f;
    inline static const float ZOOM_LIMIT_MINIMUM = -68.0f;

    inline static const float ANALOGUE_INPUT_DEAD_ZONE = 16.0f;

    inline static const float DEFAULT_VISIBLE_DURATION = 1000.0f * 60.0f * 15.0f; /// Visible duration at 1.0f scale.
    inline static const int   MAX_VISIBLE_MAJOR_TICKS  = 20; /// Maximum number of visible major ticks (with labels)

    inline static const long DURATION_SECOND = 1000;
    inline static const long DURATION_MINUTE = 60 * DURATION_SECOND;
    inline static const long DURATION_HOUR   = 60 * DURATION_MINUTE;
    inline static const long DURATION_DAY    = 24 * DURATION_HOUR;

    inline static constexpr float SCROLL_MARGIN = 0.4f;

    std::function<std::string(long)> cTimeFormatMillisecondExact;
    std::function<std::string(long)> cTimeFormatMillisecondPrecise;
    std::function<std::string(long)> cTimeFormatMillisecondRough;
    std::function<std::string(long)> cTimeFormatSecond;
    std::function<std::string(long)> cTimeFormatMinute;
    std::function<std::string(long)> cTimeFormatHour;
    std::function<std::string(long)> cTimeFormatDay;

    class TickConfiguration {
      public:
      TickConfiguration(long ticks, int minorTicks, std::function<std::string(long)> timeFormat) :
                cMajorTickSpacing(ticks),
                cMinorTicks(minorTicks),
                cTimeFormat(timeFormat) {
      }

      long cMajorTickSpacing;
      int cMinorTicks;
      std::function<std::string(long)> cTimeFormat;
    };

    // External interfaces.
    HatHandler& cHatHandler;

    IAction* cExitAction;

    Sequence& cSequence;

    // View.
    float cTimelinePan;
    float cTimelineZoomSpeed;
    double cTimelineZoom;
    float cTimelineZoomStep;

    AnimatedInteger cCursorTrack;
    AnimatedFloat cCursorTimeline;
    AnimatedFloat cTrackScrollOffset;
    bool cCursorTrackProperties;
    int cCursorTrackPropertiesAnimation;

    AnimatedFloat cMoveIndicatorTop;
    AnimatedFloat cMoveIndicatorBottom;
    double cCursorTimelineSpeed;
    ISequenceTrackEvent* cCursorEvent;
    bool cMoveMode;
    bool cMoveAllMode;
    ISequenceTrackEvent* cMovingEvent;
    std::vector<int> cTrackLocks;
    
    bool cHasFocus;
    bool cEditingProperties;
    UIManager cPropertiesUI;
    IFont* cFont;
    float cFontSize;

    bool cPreviewing;
    long cPreviewPosition;

    std::vector<TickConfiguration> cMajorTicks;

    void updateSelectedEvent();
    bool isEvent(unsigned int time) const;
    unsigned int getPreviousAlignedEventTime() const;
    unsigned int getNextAlignedEventTime() const;
    unsigned int getPreviousEventTime() const;
    unsigned int getNextEventTime() const;
    const TickConfiguration& getTickConfiguration(float duration) const;
  };
}
