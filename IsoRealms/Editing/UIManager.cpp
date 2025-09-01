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

#include "IsoRealms/Assets/Type/IFont.h"
#include "IsoRealms/Utils.h"

#include "Choice.h"

namespace IsoRealms {
  UIManager::UIManager(Project& project, IUIStyle& style, std::function<void()> finishCallback, std::function<void(IEditable*)> editorCallback) :
            cProject(project),
            cStyle(style),
            cFinishCallback(finishCallback),
            cEditorCallback(editorCallback),
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
      float mLineHeight = mFont->getHeight(mFontSize, "A");
      float mTop        = cHighlightTop.animation();// + mLineHeight;
      float mBottom     = mTop - cTooltipHeight.animation();
      glColor3f(0.2f, 0.0f, 0.0f);
      Utils::renderRoundedRectangle(cTooltipLeft.animation() - mFontSize, mBottom - mFontSize, cTooltipRight.animation() + mFontSize, mTop + mFontSize, mFontSize);
      glColor3f(1.0f, 1.0f, 1.0f);
      mFont->print(cTooltipLeft.animation(), cHighlightTop.animation() - mLineHeight, mFontSize, IFont::Alignment::LEFT, cTooltipText);

      glBegin(GL_QUADS);
      glColor3f(1.0f, 0.0f, 0.3f);
      glVertex2f(cHighlightRight.animation(), cHighlightTop.animation()    - mLineHeight / 3.0f);
      glVertex2f(cHighlightRight.animation(), cHighlightBottom.animation() + mLineHeight / 3.0f);
      glColor3f(0.2f, 0.0f, 0.0f);
      glVertex2f(cTooltipLeft.animation() - mFontSize, cHighlightBottom.animation() + mLineHeight / 3.0f);
      glVertex2f(cTooltipLeft.animation() - mFontSize, cHighlightTop.animation()    - mLineHeight / 3.0f);
      glEnd();
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
    cHighlightBottom.update(milliseconds);
    cHighlightLeft.update(milliseconds);
    cHighlightRight.update(milliseconds);
    cHighlightTop.update(milliseconds);
    
    cTooltipHeight.update(milliseconds);
    cTooltipLeft.update(milliseconds);
    cTooltipRight.update(milliseconds);

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

  void UIManager::input(UISignalID id) {
    if (cConfirmationSelection != nullptr) {
      cConfirmationSelection->input(id);
    } else if (!cRuntimeUIs.empty()) {
      cRuntimeUIs.back()->cScreen->input(id);
    }
  }

  bool UIManager::input(sf::Event& event) {
    if (cConfirmationSelection != nullptr) {
      // Nothing to do.
    } else if (!cRuntimeUIs.empty()) {
      return cRuntimeUIs.back()->cScreen->input(event);
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

  Project& UIManager::getProject() const {
    return cProject;
  }

  void UIManager::setTooltip(const std::string& text) {
    cTooltipText.clear();
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
            cTooltipText += '\n';
            mLineCount++;
          }
          cTooltipText += text.substr(mLineBeginning, mPrevSpace - mLineBeginning);
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
        cTooltipText += '\n';
        mLineCount++;
      }
      cTooltipText += text.substr(mLineBeginning, mPrevSpace - mLineBeginning);
      mLineBeginning = mPrevSpace + 1;
    } else if (mLineWidth > mWidestLineWidth) {
      mWidestLineWidth = mLineWidth;
    }
    if (mLineBeginning != 0) {
      cTooltipText += '\n';
      mLineCount++;
    }
    cTooltipText += text.substr(mLineBeginning);

    cTooltipLeft   = cRuntimeUIs.back()->cScreen->getContentRight() + mFontSize * 4.0f;
    cTooltipRight  = cTooltipLeft.value() + mWidestLineWidth;
    cTooltipHeight = mFont->getHeight(mFontSize, cTooltipText);
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
            cSlideAnimation(1000),
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
}
