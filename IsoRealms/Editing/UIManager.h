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

#include <algorithm>
#include <stack>
#include <vector>

#include "IsoRealms/Assets/Literal/LiteralColour.h"
#include "IsoRealms/Assets/Type/IEditable.h"
#include "IsoRealms/Common/AnimatedFloat.h"
#include "IsoRealms/Input/ButtonMapping.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/ResourceDefinition.h"

#include "IDialogManager.h"
#include "IUIScreen.h"
#include "IUIStyle.h"
#include "UISignalID.h"

namespace IsoRealms {
  class Choice;
  class Project;

  class UIManager final : public IDialogManager {
    public:
    UIManager(Project& project, IUIStyle& style, std::function<void()> finishCallback, std::function<void(IEditable*)> editorCallback);

    /*****************************\
     * Implements IDialogManager *
    \*****************************/
    bool confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) override;
    Project& getProject() const override;

    void render(float aspectRatio) const;
    void update(unsigned int milliseconds);
    bool input(UISignalID id);
    bool input(sf::Event& event);  

    void openUI(std::unique_ptr<IUIScreen> screen, const std::string& breadCrumb, const IColour& breadCrumbColour = LiteralColour(1.0f, 1.0f, 1.0f));
    void closeUI();
    void edit(IEditable* editable);
    void hide();
    void show();
    bool isHidden() const;
    void setTooltip(const std::string& text);
    float getBreadCrumbWidth() const;

    private:
    class UIScreen {
      public:
      UIScreen(std::unique_ptr<IUIScreen> screen, const std::string& breadCrum, const IColour& breadCrumbColour);
      void updateSlideActive(unsigned int milliseconds);
      bool updateSlideClosing(unsigned int milliseconds);
      void updateSlideInactive(unsigned int milliseconds);

      std::unique_ptr<IUIScreen> cScreen;
      int cSlideAnimation;
      std::string cBreadCrumb;
      LiteralColour cBreadCrumbColour;
    };

    class Tooltip {
      public:
      Tooltip() :
                cText(""),
                cSlideAnimation(0.0f) {
      }

      Tooltip(const Tooltip& tooltip) :
                cText(tooltip.cText),
                cSlideAnimation(tooltip.cSlideAnimation) {
      }

      std::string cText;
      int cSlideAnimation;
    };
    
    class ButtonState {
      public:
      ButtonState(std::function<bool()> pressAction);
      bool setPressed(bool pressed);
      void update(unsigned int milliseconds);

      private:
      bool cPressed;
      int cTimeUntilTrigger;
      int cRepeatInterval;
      std::function<bool()> cPressAction;
    };

    static const int BUTTON_STATE_PRESS_REPEAT_DELAY    = 350;
    static const int BUTTON_STATE_PRESS_REPEAT_INTERVAL = 100;

    // External interfaces.
    Project& cProject;
    IUIStyle& cStyle;

    HatHandler& cHatHandler;

    ButtonState cButtonStateUp;
    ButtonState cButtonStateDown;
    ButtonState cButtonStateLeft;
    ButtonState cButtonStateRight;

    std::function<void()> cFinishCallback;
    std::function<void(IEditable*)> cEditorCallback;

    std::vector<std::unique_ptr<UIScreen>> cRuntimeUIs;       /// Stack of project UIs.
    std::vector<std::unique_ptr<UIScreen>> cRuntimeClosedUIs; /// Closed project UIs that are still being animated.

    mutable AnimatedFloat cHighlightLeft;
    mutable AnimatedFloat cHighlightRight;
    mutable AnimatedFloat cHighlightTop;
    mutable AnimatedFloat cHighlightBottom;

    // Tooltip data.
    Tooltip cRuntimeTooltip;
    std::vector<std::unique_ptr<Tooltip>> cRuntimeClosedTooltips;
    mutable AnimatedFloat cTooltipLeft;
    mutable AnimatedFloat cTooltipRight;
    mutable AnimatedFloat cTooltipHeight;
    bool cTooltipVisible;
    int cTooltipAnimation;

    bool cHidden;
    int cHideAnimation;

    std::unique_ptr<Choice> cConfirmationSelection;
    std::unique_ptr<Choice> cClosedConfirmationSelection;
  };
}
