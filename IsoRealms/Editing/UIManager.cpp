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
#include "UIManager.h"

#include <GL/glew.h>

#include "IsoRealms/Common/ScreenArea.h"
#include "IsoRealms/Assets/Type/IFont.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Utils.h"

#include "Choice.h"

namespace IsoRealms {
  UIManager::UIManager(Project& project, IUIStyle& style, std::function<void()> finishCallback, std::function<void(IEditable*)> editorCallback) :
            cProject(project),
            cStyle(style),
            cHatHandler(project.getApplication().getHatHandler()),
            cButtonStateUp(   [this]() {return input(UISignalID::MOVE_UP);   }),
            cButtonStateDown( [this]() {return input(UISignalID::MOVE_DOWN); }),
            cButtonStateLeft( [this]() {return input(UISignalID::MOVE_LEFT); }),
            cButtonStateRight([this]() {return input(UISignalID::MOVE_RIGHT);}),
            cFinishCallback(finishCallback),
            cEditorCallback(editorCallback),
            cTooltipVisible(false),
            cTooltipAnimation(0),
            cHidden(false),
            cHideAnimation(0) {
  }

  bool UIManager::confirm(const std::string& message, std::function<void()> confirm, std::function<void()> cancel) {
    cConfirmationSelection = std::make_unique<Choice>(cStyle, message, std::vector<std::string>{"Yes", "No"}, [this, confirm, cancel](const std::string& choice)->bool {
      cClosedConfirmationSelection = std::move(cConfirmationSelection);
      cConfirmationSelection = nullptr;
      if (choice == "Yes") {
        confirm();
        return true;
      } else {
        cancel();
        return false;
      }
    });
    return true; // TODO: I think this function should not return.
  }

  Project& UIManager::getProject() const {
    return cProject;
  }

  void UIManager::render(float aspectRatio) const {
    glEnable(GL_BLEND);
    
    if (cHideAnimation > 0) {
      glPushMatrix();
      glTranslatef(aspectRatio * (-cHideAnimation / 125.0f), 0.0f, 0.0f);
    }
    

    IFont* mFont = cStyle.getFont();
    float mFontSize = cStyle.getFontSize();
    aspectRatio -= mFontSize;
//    glTranslatef(mFontSize * 1.5f, -mFontSize, 0.0f);

    float mBreadCrumbWidth = 0.0f;
    for (const std::unique_ptr<UIScreen>& mProjectUI : cRuntimeUIs) {
      std::string mBreadCrumb = mProjectUI->cBreadCrumb;
      if (mBreadCrumbWidth != 0.0f) {
        mBreadCrumbWidth += mFont->getWidth(mFontSize, " > ");
      }
      mBreadCrumbWidth += mFont->getWidth(mFontSize, mBreadCrumb);

      glPushMatrix();
      glTranslatef(aspectRatio * (mProjectUI->cSlideAnimation / 500.0f), 0.0f, 0.0f);
      glDepthMask(GL_FALSE);
      mProjectUI->cScreen->renderBackground(aspectRatio, mBreadCrumbWidth);
      glDepthMask(GL_TRUE);
      glPopMatrix();
    }
//    std::cout << "There are " << cRuntimeClosedUIs.size() << " closing UI's in " << this << "..." << std::endl;
    for (const std::unique_ptr<UIScreen>& mProjectUI : cRuntimeClosedUIs) {
      glPushMatrix();
      glTranslatef(aspectRatio * (mProjectUI->cSlideAnimation / 500.0f), 0.0f, 0.0f);
      mProjectUI->cScreen->renderBackground(aspectRatio, mBreadCrumbWidth);
      glPopMatrix();
    }

    // Render selection bar.
    if (!cRuntimeUIs.empty()) {
      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_DEPTH_TEST);
      cHighlightLeft = cRuntimeUIs.back()->cScreen->getSelectionHighlightLeft(aspectRatio - mFontSize);
      cHighlightRight = cRuntimeUIs.back()->cScreen->getSelectionHighlightRight(aspectRatio - mFontSize);
      cHighlightTop = cRuntimeUIs.back()->cScreen->getSelectionHighlightTop();
      cHighlightBottom = cRuntimeUIs.back()->cScreen->getSelectionHighlightBottom();
//      std::cout << "Top: " << cHighlightTop.animation() << "    Bottom: " << cHighlightBottom.animation() << "     Left: " << cHighlightLeft.animation() << "     Right: " << cHighlightRight.animation() << std::endl;
      glColor3f(1.0f, 0.0f, 0.3f);
      Utils::renderBar(cHighlightLeft.animation(), cHighlightBottom.animation(), cHighlightRight.animation(), cHighlightTop.animation());
      glColor3f(1.0f, 1.0f, 1.0f);

      // Tooltip information.
      if (cTooltipAnimation > 0) {
        float mLineHeight = mFont->getHeight(mFontSize, "A");
        float mTop        = cHighlightTop.animation();// + mLineHeight;
        float mBottom     = mTop - cTooltipHeight.animation();

        float mTooltipLeft   = cHighlightRight.animation()  + ((cTooltipLeft.animation()  - mFontSize) - cHighlightRight.animation())  * (cTooltipAnimation / 250.0f);
        float mTooltipRight  = cHighlightRight.animation()  + ((cTooltipRight.animation() + mFontSize) - cHighlightRight.animation())  * (cTooltipAnimation / 250.0f);
        float mTooltipBottom = cHighlightBottom.animation() + ((mBottom                   - mFontSize) - cHighlightBottom.animation()) * (cTooltipAnimation / 250.0f);
        float mTooltipTop    = cHighlightTop.animation()    + ((mTop                      + mFontSize) - cHighlightTop.animation())    * (cTooltipAnimation / 250.0f);

        glColor3f(0.2f, 0.0f, 0.0f);
        Utils::renderRoundedRectangle(mTooltipLeft, mTooltipBottom, mTooltipRight, mTooltipTop, mFontSize);
        glColor3f(1.0f, 1.0f, 1.0f);

        ScreenArea mPreviousCrop = cProject.getApplication().crop(ScreenArea(mTooltipLeft, mTooltipRight, mTooltipBottom, mTooltipTop));;
        mFont->print(cTooltipLeft.animation() + 1.0f * (cRuntimeTooltip.cSlideAnimation / 1000.0f), cHighlightTop.animation() - mLineHeight, mFontSize, IFont::Alignment::LEFT, cRuntimeTooltip.cText);
        for (const std::unique_ptr<Tooltip>& mClosedTooltip : cRuntimeClosedTooltips) {
          mFont->print(cTooltipLeft.animation() + 1.0f * (mClosedTooltip->cSlideAnimation / 1000.0f), cHighlightTop.animation() - mLineHeight, mFontSize, IFont::Alignment::LEFT, mClosedTooltip->cText);
        }
        cProject.getApplication().crop(mPreviousCrop);

        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.3f);
        glVertex2f(cHighlightRight.animation(), cHighlightTop.animation()    - mLineHeight / 3.0f);
        glVertex2f(cHighlightRight.animation(), cHighlightBottom.animation() + mLineHeight / 3.0f);
        glColor3f(0.2f, 0.0f, 0.0f);
        glVertex2f(mTooltipLeft, cHighlightBottom.animation() + mLineHeight / 3.0f);
        glVertex2f(mTooltipLeft, cHighlightTop.animation()    - mLineHeight / 3.0f);
        glEnd();
      }
    }

    // Render UI's (menus, etc.).
    unsigned int mUICount = 1;
    mBreadCrumbWidth = 0.0f;

    for (const std::unique_ptr<UIScreen>& mProjectUI : cRuntimeUIs) {
      std::string mBreadCrumb = mProjectUI->cBreadCrumb;

      glPushMatrix();
      int mBreadCrumbSlide = std::max(0, mProjectUI->cSlideAnimation);
      int mBreadCrumbSlideRemaining = mProjectUI->cSlideAnimation - mBreadCrumbSlide;
      glTranslatef(aspectRatio * (mBreadCrumbSlide / 500.0f), 0.0f, 0.0f);
      if (mBreadCrumbWidth != 0.0f) {
        mFont->print(-1.0f * aspectRatio + mBreadCrumbWidth + mFontSize, 1.0f - mFontSize * 3.0f, mFontSize, IFont::Alignment::LEFT, " > ");
        mBreadCrumbWidth += mFont->getWidth(mFontSize, " > ");
      }
      mProjectUI->cBreadCrumbColour.set();
      mFont->print(-1.0f * aspectRatio + mBreadCrumbWidth + mFontSize, 1.0f - mFontSize * 3.0f, mFontSize, IFont::Alignment::LEFT, mBreadCrumb);
      mBreadCrumbWidth += mFont->getWidth(mFontSize, mBreadCrumb);
      glColor3f(1.0f, 1.0f, 1.0f);

      // if (mUICount != cRuntimeUIs.size()) {
      //   glTranslatef(aspectRatio * (mProjectUI->cSlideAnimation / 500.0f), 0.0f, 0.0f);
      // }
      mUICount++;
      glTranslatef(aspectRatio * (mBreadCrumbSlideRemaining / 500.0f), 0.0f, 0.0f);
      mProjectUI->cScreen->render(aspectRatio - mFontSize);
      glPopMatrix();
    }
    for (const std::unique_ptr<UIScreen>& mProjectUI : cRuntimeClosedUIs) {
      std::string mBreadCrumb = mProjectUI->cBreadCrumb;
      glPushMatrix();
      glTranslatef(aspectRatio * (mProjectUI->cSlideAnimation / 500.0f), 0.0f, 0.0f);
      if (mBreadCrumbWidth != 0.0f) {
        mFont->print(-1.0f * aspectRatio + mBreadCrumbWidth + mFontSize, (1.0f - mFontSize * 3.0f) + 0.01f, mFontSize, IFont::Alignment::LEFT, " > ");
        mBreadCrumbWidth += mFont->getWidth(mFontSize, " > ");
      }
      mProjectUI->cBreadCrumbColour.set();
      mFont->print(-1.0f * aspectRatio + mBreadCrumbWidth + mFontSize, (1.0f - mFontSize * 3.0f) + 0.01f, mFontSize, IFont::Alignment::LEFT, mBreadCrumb);
      glColor3f(1.0f, 1.0f, 1.0f);
      mProjectUI->cScreen->render(aspectRatio - mFontSize);
      glPopMatrix();
    }
    
    if (cHideAnimation > 0) {
      glPopMatrix();
    }
    
    glDisable(GL_BLEND);

    // Render confirmation choice.
    if (cConfirmationSelection != nullptr) {
      cConfirmationSelection->render(cStyle);
    }
    if (cClosedConfirmationSelection != nullptr) {
      cClosedConfirmationSelection->render(cStyle);
    }
  }

  void UIManager::update(unsigned int milliseconds) {
    cButtonStateUp.update(milliseconds);
    cButtonStateDown.update(milliseconds);
    cButtonStateLeft.update(milliseconds);
    cButtonStateRight.update(milliseconds);

    cHighlightBottom.update(milliseconds);
    cHighlightLeft.update(milliseconds);
    cHighlightRight.update(milliseconds);
    cHighlightTop.update(milliseconds);
    
    cTooltipHeight.update(milliseconds);
    cTooltipLeft.update(milliseconds);
    cTooltipRight.update(milliseconds);

    cRuntimeTooltip.cSlideAnimation = std::max(0, cRuntimeTooltip.cSlideAnimation - static_cast<int>(milliseconds * 4));
    for (const std::unique_ptr<Tooltip>& mClosedTooltip : cRuntimeClosedTooltips) {
      mClosedTooltip->cSlideAnimation = std::max(-1000, mClosedTooltip->cSlideAnimation - static_cast<int>(milliseconds * 4));
    }
    while (!cRuntimeClosedTooltips.empty() && (*cRuntimeClosedTooltips.begin())->cSlideAnimation == -1000) {
      cRuntimeClosedTooltips.erase(cRuntimeClosedTooltips.begin());
    }

    if (cTooltipVisible) {
      cTooltipAnimation = std::min(250, cTooltipAnimation + static_cast<int>(milliseconds));
    } else {
      cTooltipAnimation = std::max(0,   cTooltipAnimation - static_cast<int>(milliseconds));
    }

    cHideAnimation = cHidden ? std::min(250, static_cast<int>(cHideAnimation + milliseconds))
                             : std::max(0,   static_cast<int>(cHideAnimation - milliseconds));

    for (unsigned int i = 0; i < cRuntimeUIs.size(); ++i) {
      if (i == cRuntimeUIs.size() - 1) {
        cRuntimeUIs[i]->updateSlideActive(milliseconds);
      } else {
        cRuntimeUIs[i]->updateSlideInactive(milliseconds);
      }
      cRuntimeUIs[i]->cScreen->update(milliseconds);
    }

    std::stack<unsigned int> mClosedIndices;
    for (unsigned int i = 0; i < cRuntimeClosedUIs.size(); ++i) {
      if (cRuntimeClosedUIs[i]->updateSlideClosing(milliseconds)) {
        mClosedIndices.emplace(i);
      }
      cRuntimeClosedUIs[i]->cScreen->update(milliseconds);
    }

    while (!mClosedIndices.empty()) {
      unsigned int mClosedIndex = mClosedIndices.top();
      cRuntimeClosedUIs.erase(cRuntimeClosedUIs.begin() + mClosedIndex);
      mClosedIndices.pop();
    }

    if (cConfirmationSelection != nullptr) {
      cConfirmationSelection->update(milliseconds);
    }
    if (cClosedConfirmationSelection != nullptr) {
      if (cClosedConfirmationSelection->update(milliseconds)) {
        cClosedConfirmationSelection = nullptr;
      }
    }
  }

  bool UIManager::input(UISignalID id) {
    if (cConfirmationSelection != nullptr) {
      cConfirmationSelection->input(id);
    } else if (!cRuntimeUIs.empty()) {
      switch (id) {
        case UISignalID::TOGGLE_HELP: {
          cTooltipVisible = !cTooltipVisible;
          return true;
        }

        default: {
          return cRuntimeUIs.back()->cScreen->input(id);
          break;
        }
      }
    }
    return false;
  }

  bool UIManager::input(sf::Event& event) {
    if (cConfirmationSelection == nullptr && !cRuntimeUIs.empty() && cRuntimeUIs.back()->cScreen->input(event)) {
      return true;
    } else switch (event.type) {
      case sf::Event::KeyPressed: {
        switch (event.key.code) {
          case sf::Keyboard::BackSpace: // Fall through.
          case sf::Keyboard::Escape:    return input(UISignalID::CANCEL);
          case sf::Keyboard::Space:     // Fall through.
          case sf::Keyboard::Return:    return input(UISignalID::CONFIRM);
          case sf::Keyboard::Up:        return input(UISignalID::MOVE_UP);
          case sf::Keyboard::Down:      return input(UISignalID::MOVE_DOWN);
          case sf::Keyboard::Left:      return input(UISignalID::MOVE_LEFT);
          case sf::Keyboard::Right:     return input(UISignalID::MOVE_RIGHT);
          case sf::Keyboard::Tab:       return input(UISignalID::TOGGLE_HELP);
          default: break;
        }
      }

      case sf::Event::JoystickMoved: {
        if (cHatHandler.upPressed())     {return cButtonStateUp.setPressed(   true);  }
        if (cHatHandler.downPressed())   {return cButtonStateDown.setPressed( true);  }
        if (cHatHandler.leftPressed())   {return cButtonStateLeft.setPressed( true);  }
        if (cHatHandler.rightPressed())  {return cButtonStateRight.setPressed(true);  }
        if (cHatHandler.upReleased())    {return cButtonStateUp.setPressed(   false); }
        if (cHatHandler.downReleased())  {return cButtonStateDown.setPressed( false); }
        if (cHatHandler.leftReleased())  {return cButtonStateLeft.setPressed( false); }
        if (cHatHandler.rightReleased()) {return cButtonStateRight.setPressed(false); }
        break;
      }

      case sf::Event::JoystickButtonPressed: {
        if (event.joystickButton.button == GameControllerButton::CROSS)  {return input(UISignalID::CONFIRM);     }
        if (event.joystickButton.button == GameControllerButton::CIRCLE) {return input(UISignalID::CANCEL);      }
        if (event.joystickButton.button == GameControllerButton::SQUARE) {return input(UISignalID::TOGGLE_HELP); }
        break;
      }

      default: break;
    }    
    return false;
  }
  
  void UIManager::openUI(std::unique_ptr<IUIScreen> screen, const std::string& breadCrumb, const IColour& breadCrumbColour) {
    cRuntimeUIs.push_back(std::make_unique<UIScreen>(std::move(screen), breadCrumb, breadCrumbColour));
    setTooltip(cRuntimeUIs.back()->cScreen->getTooltip());
    cRuntimeUIs.back()->cScreen->updateRight();
  }

  void UIManager::closeUI() {
    cRuntimeClosedUIs.emplace_back(std::move(cRuntimeUIs[cRuntimeUIs.size() - 1]));
    cRuntimeUIs.pop_back();
    if (!cRuntimeUIs.empty()) {
      cRuntimeUIs.back()->cScreen->refresh();
      setTooltip(cRuntimeUIs.back()->cScreen->getTooltip());
      cRuntimeClosedUIs.back()->cScreen->updateRight();
    } else {
      cFinishCallback();
    }
  }

  void UIManager::edit(IEditable* editable) {
    cEditorCallback(editable);
  }

  void UIManager::hide() {
    cHidden = true;
  }

  void UIManager::show() {
    cHidden = false;
  }
  
  bool UIManager::isHidden() const {
    return cHidden;
  }

  void UIManager::setTooltip(const std::string& text) {
    if (text == cRuntimeTooltip.cText) {
      return;
    }

    // Animate out the previous tooltip.
    cRuntimeClosedTooltips.emplace_back(std::make_unique<Tooltip>(cRuntimeTooltip));

    // Construct the tooltip text.
    cRuntimeTooltip.cText.clear();
    int mLineBeginning = 0;
    int mPrevSpace = 0;
    int mLineCount = 1;
    float mMaxLineWidth = 0.8f;
    float mWidestLineWidth = 0.0f;
    IFont* mFont = cStyle.getFont();
    float mFontSize = cStyle.getFontSize();
    for (unsigned int i = 0; i < text.length(); i++) {
      if (text[i] == ' ') {
        float mLineWidth = mFont->getWidth(mFontSize, text.substr(mLineBeginning, i - mLineBeginning));
        if (mLineWidth > mMaxLineWidth) {
          if (mLineBeginning != 0) {
            cRuntimeTooltip.cText += '\n';
            mLineCount++;
          }
          cRuntimeTooltip.cText += text.substr(mLineBeginning, mPrevSpace - mLineBeginning);
          mLineBeginning = mPrevSpace + 1;
        } else if (mLineWidth > mWidestLineWidth) {
          mWidestLineWidth = mLineWidth;
        }
        mPrevSpace = i;
      }
    }
    float mLineWidth = mFont->getWidth(mFontSize, text.substr(mLineBeginning));
    if (mLineWidth > mMaxLineWidth) {
      if (mLineBeginning != 0) {
        cRuntimeTooltip.cText += '\n';
        mLineCount++;
      }
      cRuntimeTooltip.cText += text.substr(mLineBeginning, mPrevSpace - mLineBeginning);
      mLineBeginning = mPrevSpace + 1;
    } else if (mLineWidth > mWidestLineWidth) {
      mWidestLineWidth = mLineWidth;
    }
    if (mLineBeginning != 0) {
      cRuntimeTooltip.cText += '\n';
      mLineCount++;
    }
    cRuntimeTooltip.cText += text.substr(mLineBeginning);
    cRuntimeTooltip.cSlideAnimation = 1000;

    cTooltipLeft   = cRuntimeUIs.back()->cScreen->getContentRight() + mFontSize * 4.0f;
    cTooltipRight  = cTooltipLeft.value() + mWidestLineWidth;
    cTooltipHeight = mFont->getHeight(mFontSize, cRuntimeTooltip.cText);
  }

  float UIManager::getBreadCrumbWidth() const {
    IFont* mFont = cStyle.getFont();
    float mFontSize = cStyle.getFontSize();
    float mBreadCrumbWidth = 0.0f;
    for (const std::unique_ptr<UIScreen>& mProjectUI : cRuntimeUIs) {
      std::string mBreadCrumb = mProjectUI->cBreadCrumb;
      if (mBreadCrumbWidth != 0.0f) {
        mBreadCrumbWidth += mFont->getWidth(mFontSize, " > ");
      }
      mBreadCrumbWidth += mFont->getWidth(mFontSize, mBreadCrumb);
    }
    return mBreadCrumbWidth;
  }

  UIManager::UIScreen::UIScreen(std::unique_ptr<IUIScreen> screen, const std::string& breadCrum, const IColour& breadCrumbColour) :
            cScreen(std::move(screen)),
            cBreadCrumb(breadCrum),
            cBreadCrumbColour(breadCrumbColour) {
  }

  void UIManager::UIScreen::updateSlideActive(unsigned int milliseconds) {
    if (cSlideAnimation > 0) {
      cSlideAnimation = std::max(0, cSlideAnimation - static_cast<int>(milliseconds * 4));
    } else {
      cSlideAnimation = std::min(0, cSlideAnimation + static_cast<int>(milliseconds * 4));
    }
  }

  bool UIManager::UIScreen::updateSlideClosing(unsigned int milliseconds) {
    if (cSlideAnimation < 1000) {
      cSlideAnimation = std::min(1000, cSlideAnimation + static_cast<int>(milliseconds * 4));
    }
    return cSlideAnimation == 1000;
  }

  void UIManager::UIScreen::updateSlideInactive(unsigned int milliseconds) {
    if (cSlideAnimation > -1000) {
      cSlideAnimation = std::max(-1000, cSlideAnimation - static_cast<int>(milliseconds * 4));
    }
  }

  UIManager::ButtonState::ButtonState(std::function<bool()> pressAction) :
            cPressed(false),
            cTimeUntilTrigger(BUTTON_STATE_PRESS_REPEAT_DELAY),
            cRepeatInterval(BUTTON_STATE_PRESS_REPEAT_INTERVAL),
            cPressAction(pressAction) {
  }

  bool UIManager::ButtonState::setPressed(bool pressed) {
    bool mResult = false;
    if (pressed) {
      mResult = cPressAction();
    }
    cPressed = pressed;
    return mResult;
  }

  void UIManager::ButtonState::update(unsigned int milliseconds) {
    if (cPressed) {
      if (cTimeUntilTrigger < 0) {
        cPressAction();
        if (cRepeatInterval > 10) {
          cRepeatInterval -= 4;
        }
        cTimeUntilTrigger = cRepeatInterval + cTimeUntilTrigger;
      }
      cTimeUntilTrigger -= milliseconds;
    } else {
      cRepeatInterval = BUTTON_STATE_PRESS_REPEAT_INTERVAL;
      cTimeUntilTrigger = BUTTON_STATE_PRESS_REPEAT_DELAY;
    }
  }
}
