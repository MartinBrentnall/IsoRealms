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
#pragma once

#include <GL/glew.h>

#include "IsoRealms/AnimatedInteger.h"
#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Application.h"
#include "IsoRealms/Editing/EditorToolbar.h"
#include "IsoRealms/Editing/PropertiesMenu.h"
#include "IsoRealms/Editing/IUIStyle.h"
#include "IsoRealms/Editing/UIManager.h"
#include "IsoRealms/Input/EditorAnalogueInput.h"
#include "IsoRealms/Input/EditorDigitalInput.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  class Sequence;
  class ISequenceTrackEvent;

  class SequenceEditor : public IEditableScreen,
                         public IUIStyle {
    public:
    SequenceEditor(Sequence& sequence);
    void updateScreen(unsigned int milliseconds);

    /******************************\
     * Implements IEditableScreen *
    \******************************/
    bool input(sf::Event& event) override;
    void resetInput() override;
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;
    void notifyVisible() override;
    void notifyHidden() override;
    void notifyLostFocus() override;
    void notifyGainedFocus() override;
    std::vector<std::string> getDigitalInputs() const override;
    std::vector<std::string> getAnalogueInputs() const override;
    void setDigitalInput(const std::string& name, IBoolean* input) override;
    void setAnalogueInput(const std::string& name, IFloat* input) override;
    void setExitAction(ActionExecutor* action) override;
    void setAppearance(IFont* font, float scale) override;
    void unregisterAssets(IAssetRemover& assets, bool relinquish) override;
    IScreen* screen() override;
    
    /*******************************\
     * Implements IUIConfiguration *
    \*******************************/
    IFont* getFont() const override;
    float getFontSize() const override;
    IFont* getCodeFont() const override;
    float getCodeFontSize() const override;
    IProject& getProject() const override;
      
    private:
    static const float ZOOM_LIMIT_MAXIMUM;
    static const float ZOOM_LIMIT_MINIMUM;

    static const float ANALOGUE_INPUT_DEAD_ZONE;

    static const float DEFAULT_VISIBLE_DURATION; /// Visible duration at 1.0f scale.
    static const int MAX_VISIBLE_MAJOR_TICKS;    /// Maximum number of visible major ticks (with labels)

    static const long DURATION_SECOND;
    static const long DURATION_MINUTE;
    static const long DURATION_HOUR;
    static const long DURATION_DAY;

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

    ActionExecutor* cExitAction;

    Sequence& cSequence;

    // View.
    float cTimelinePan;
    float cTimelineZoomSpeed;
    double cTimelineZoom;
    float cTimelineZoomStep;

    AnimatedInteger cCursorTrack;
    AnimatedFloat cCursorTimeline;
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
