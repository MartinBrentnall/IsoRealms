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
#include "UIManager.h"

namespace IsoRealms {
  UIManager::UIManager(IProject& project, IUIStyle& style, std::function<void()> finishCallback, std::function<void(IEditable*)> editorCallback) :
            cProject(project),
            cStyle(style),
            cFinishCallback(finishCallback),
            cEditorCallback(editorCallback),
            cHidden(false),
            cHideAnimation(0) {
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
      std::string mBreadCrumb = mProjectUI->cScreen->getBreadCrumb();
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
      std::string mHelpText = cRuntimeUIs.back()->cScreen->getTooltip();
      std::string mWrappedText;
      int mLineBeginning = 0;
      int mPrevSpace = 0;
      int mLineCount = 1;
      float mMaxLineWidth = 0.5f;
      for (unsigned int i = 0; i < mHelpText.length(); i++) {
        if (mHelpText[i] == ' ') {
          float mLineWidth = mFont->getWidth(mFontSize, mHelpText.substr(mLineBeginning, i - mLineBeginning));
          if (mLineWidth > mMaxLineWidth) {
            if (mLineBeginning != 0) {
              mWrappedText += '\n';
              mLineCount++;
            }
            mWrappedText += mHelpText.substr(mLineBeginning, mPrevSpace - mLineBeginning);
            mLineBeginning = mPrevSpace + 1;
          }
          mPrevSpace = i;
        }
      }
      float mLineWidth = mFont->getWidth(mFontSize, mHelpText.substr(mLineBeginning));
      if (mLineWidth > mMaxLineWidth) {
        if (mLineBeginning != 0) {
          mWrappedText += '\n';
          mLineCount++;
        }
        mWrappedText += mHelpText.substr(mLineBeginning, mPrevSpace - mLineBeginning);
        mLineBeginning = mPrevSpace + 1;
      }
      if (mLineBeginning != 0) {
        mWrappedText += '\n';
        mLineCount++;
      }
      mWrappedText += mHelpText.substr(mLineBeginning);


      float mLineHeight = mFont->getHeight(mFontSize, "A");
      float mLeft = cHighlightRight.animation() + mFontSize * 4.0f;
      float mRight = mLeft + mMaxLineWidth;
      float mTop = cHighlightTop.animation();// + mLineHeight;
      float mBottom = mTop - mFont->getHeight(mFontSize, mWrappedText);
      glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
      Utils::renderRoundedRectangle(mLeft - mFontSize, mBottom - mFontSize, mRight + mFontSize, mTop + mFontSize, mFontSize);
      glColor3f(1.0f, 1.0f, 1.0f);
      mFont->print(mLeft, cHighlightTop.animation() - mLineHeight, mFontSize, IFont::Alignment::LEFT, mWrappedText);
    }

    // Render UI's (menus, etc.).
    unsigned int mUICount = 1;
    mBreadCrumbWidth = 0.0f;

    for (const std::unique_ptr<UIScreen>& mProjectUI : cRuntimeUIs) {
      std::string mBreadCrumb = mProjectUI->cScreen->getBreadCrumb();

      glPushMatrix();
      int mBreadCrumbSlide = std::max(0, mProjectUI->cSlideAnimation);
      int mBreadCrumbSlideRemaining = mProjectUI->cSlideAnimation - mBreadCrumbSlide;
      glTranslatef(aspectRatio * (mBreadCrumbSlide / 500.0f), 0.0f, 0.0f);
      if (mBreadCrumbWidth != 0.0f) {
        mFont->print(-1.0f * aspectRatio + mBreadCrumbWidth + mFontSize, 1.0f - mFontSize * 3.0f, mFontSize, IFont::Alignment::LEFT, " > ");
        mBreadCrumbWidth += mFont->getWidth(mFontSize, " > ");
      }
      mProjectUI->cScreen->setBreadCrumbColour();
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
      std::string mBreadCrumb = mProjectUI->cScreen->getBreadCrumb();
      glPushMatrix();
      glTranslatef(aspectRatio * (mProjectUI->cSlideAnimation / 500.0f), 0.0f, 0.0f);
      if (mBreadCrumbWidth != 0.0f) {
        mFont->print(-1.0f * aspectRatio + mBreadCrumbWidth + mFontSize, (1.0f - mFontSize * 3.0f) + 0.01f, mFontSize, IFont::Alignment::LEFT, " > ");
        mBreadCrumbWidth += mFont->getWidth(mFontSize, " > ");
      }
      mProjectUI->cScreen->setBreadCrumbColour();
      mFont->print(-1.0f * aspectRatio + mBreadCrumbWidth + mFontSize, (1.0f - mFontSize * 3.0f) + 0.01f, mFontSize, IFont::Alignment::LEFT, mBreadCrumb);
      glColor3f(1.0f, 1.0f, 1.0f);
      mProjectUI->cScreen->render(aspectRatio - mFontSize);
      glPopMatrix();
    }
    
    if (cHideAnimation > 0) {
      glPopMatrix();
    }
    
    glDisable(GL_BLEND);
  }

  void UIManager::update(unsigned int milliseconds) {
    cHighlightBottom.update(milliseconds);
    cHighlightLeft.update(milliseconds);
    cHighlightRight.update(milliseconds);
    cHighlightTop.update(milliseconds);
    
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
  }

  void UIManager::input(UISignalID id) {
    if (!cRuntimeUIs.empty()) {
      cRuntimeUIs.back()->cScreen->input(id);
    }
  }

  bool UIManager::input(sf::Event& event) {
    if (!cRuntimeUIs.empty()) {
      return cRuntimeUIs.back()->cScreen->input(event);
    }
    return false;
  }
  
  void UIManager::openUI(std::unique_ptr<IUIScreen> screen) {
    cRuntimeUIs.push_back(std::make_unique<UIScreen>(std::move(screen)));
  }

  void UIManager::closeUI() {
    cRuntimeClosedUIs.emplace_back(std::move(cRuntimeUIs[cRuntimeUIs.size() - 1]));
    cRuntimeUIs.pop_back();
    if (!cRuntimeUIs.empty()) {
      cRuntimeUIs.back()->cScreen->refresh();
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

  IProject& UIManager::getProject() const {
    return cProject;
  }

  UIManager::UIScreen::UIScreen(std::unique_ptr<IUIScreen> screen) :
            cScreen(std::move(screen)),
            cSlideAnimation(1000) {
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
