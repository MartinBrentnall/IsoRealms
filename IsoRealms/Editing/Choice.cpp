/*
 * Copyright 2025 Martin Brentnall
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
#include "Choice.h"

#include "IsoRealms/IApplication.h"
#include "IsoRealms/IProject.h"
#include "IsoRealms/Utils.h"

#include "IUIStyle.h"
#include "UISignalID.h"

namespace IsoRealms {
  Choice::Choice(IUIStyle& style, const std::string& message, const std::vector<std::string>& options, std::function<bool(const std::string&)> confirmationCallback) :
            cMessage(message),
            cConfirmationCallback(confirmationCallback),
            cSelected(0),
            cOpenness(0),
            cClosing(false) {    
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    
    // Get total width of option buttons.
    float mAllButtonsWidth = (options.size() - 1) * mFontSize * 2.0f;
    for (const std::string& mOption : options) {
      mAllButtonsWidth += mFont->getWidth(mFontSize, mOption);
    }

    // Create option buttons.
    float mXPosition = -mAllButtonsWidth / 2.0f;
    for (const std::string& mOption : options) {
      float mButtonWidth = mFont->getWidth(mFontSize, mOption);
      cOptions.emplace_back(std::make_unique<Option>(mOption, mXPosition, mXPosition + mButtonWidth));
      mXPosition += mButtonWidth + mFontSize * 2.0f;
    }
    
    // Initialise selection highlight.
    cSelectionHighlightLeft.init(cOptions[0]->getLeft());
    cSelectionHighlightRight.init(cOptions[0]->getRight());
    
    // Get box width based on space required by message and buttons.
    cBoxWidth = std::max(mFont->getWidth(mFontSize, cMessage) + mFontSize * 2.0f, mAllButtonsWidth + mFontSize * 4.0f);
  }
  
  void Choice::render(IUIStyle& style) {
    glBindTexture(GL_TEXTURE_2D, 0);
    glLineWidth(1.0f);
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    float mBoxWidth = cOpenness / 250.0f * cBoxWidth;
    float mBoxHeight = cOpenness / 250.0f * mFontSize * 8.0f;
    
    // Render panel.
    glColor3f(0.0f, 0.0f, 0.0f);
    Utils::renderRoundedRectangle(-mBoxWidth / 2.0f, -mBoxHeight / 2.0f, mBoxWidth / 2.0f, mBoxHeight / 2.0f, std::min(mFontSize, mBoxHeight / 2.0f));
    glColor3f(1.0f, 1.0f, 1.0f);
    Utils::renderRoundedRectangleLines(-mBoxWidth / 2.0f, -mBoxHeight / 2.0f, mBoxWidth / 2.0f, mBoxHeight / 2.0f, std::min(mFontSize, mBoxHeight / 2.0f));
    
    // Only allow content rendering within the panel.
    IApplication& mApplication = style.getProject().getApplication();
    ScreenArea mPreviousCrop = mApplication.crop(ScreenArea(-mBoxWidth / 2.0f, mBoxWidth / 2.0f, -mBoxHeight / 2.0f, mBoxHeight / 2.0f));

    // Render message.
    mFont->print(0.0f, mFontSize + 0.01f, mFontSize, IFont::Alignment::CENTER, cMessage);

    // Render selection highlight.
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0f, 0.0f, 0.2f);
    Utils::renderBar(cSelectionHighlightLeft.animation(), -mFontSize * 3.0f, cSelectionHighlightRight.animation(), -mFontSize * 1.0f);

    // Render selection buttons.
    glColor3f(1.0f, 1.0f, 1.0f);
    for (std::unique_ptr<Option>& mOption : cOptions) {
      mOption->render(style);
    }
    
    // Restore crop.
    mApplication.crop(mPreviousCrop);
  }

  bool Choice::update(unsigned int milliseconds) {
    cSelectionHighlightLeft.update(milliseconds);
    cSelectionHighlightRight.update(milliseconds);
    
    if (cClosing) {
      cOpenness = std::max(cOpenness - static_cast<int>(milliseconds), 0);
    } else if (cOpenness < 250) {
      cOpenness = std::min(cOpenness + static_cast<int>(milliseconds), 250);
    }
    return cOpenness == 0;
  }

  bool Choice::input(UISignalID id) {
    switch (id) {
      case UISignalID::CONFIRM: {
        cClosing = true;
        return cConfirmationCallback(cOptions[cSelected]->getText());
      }

      case UISignalID::MOVE_RIGHT: {
        if (cSelected < static_cast<int>(cOptions.size()) - 1) {
          cSelected++;
          updateAnimationValues();
        }
        break;
      }

      case UISignalID::MOVE_LEFT: {
        if (cSelected > 0) {
          cSelected--;
          updateAnimationValues();
        }
        break;
      }

      default: {
        break;
      }
    }
    return false;
  }
  
  Choice::Option::Option(const std::string& text, float left, float right) :
            cText(text),
            cLeft(left),
            cRight(right) {
  }
  
  std::string Choice::Option::getText() const {
    return cText;
  }

  float Choice::Option::getLeft() const {
    return cLeft;
  }

  float Choice::Option::getRight() const {
    return cRight;
  }

  void Choice::Option::render(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(cLeft + (cRight - cLeft) / 2.0f, -mFontSize * 3.0f + 0.01f, mFontSize, IFont::Alignment::CENTER, cText);
  }
  
  void Choice::updateAnimationValues() {
    cSelectionHighlightLeft  = cOptions[cSelected]->getLeft();
    cSelectionHighlightRight = cOptions[cSelected]->getRight();
  }
}
