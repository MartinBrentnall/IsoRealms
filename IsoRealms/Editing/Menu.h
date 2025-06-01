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
#pragma once

#include <memory>
#include <set>
#include <vector>

#include "IsoRealms/Project.h"
#include "IsoRealms/IModule.h"

#include "IUIStyle.h"
#include "UIManager.h"

namespace IsoRealms {
  template<class MENU_ITEM_TYPE> class Menu : public IUIScreen {
    public:
    Menu(UIManager& manager, IUIStyle& style, const std::string& breadCrumb, float red, float green, float blue) :
              cUIManager(manager),
              cStyle(style),
              cBreadCrumb(breadCrumb),
              cBreadCrumbRed(red),
              cBreadCrumbGreen(green),
              cBreadCrumbBlue(blue),
              cSelectedItem(0),
              cScroll(0.0f) {
    }

    /************************\
     * Implements IUIScreen *
    \************************/
    void renderBackground(float aspectRatio, float minimumWidth) override {
      float mFontSize = cStyle.getFontSize();

      float mTop = 1.0f - mFontSize;
      float mLeft = -1.0 * aspectRatio;
      float mBottom = std::max(-1.0f + mFontSize, 1.0f - ((cItems.size() + 2.0f) * mFontSize * 2.0f));
      float mRight = 0.0f;
      for (std::unique_ptr<MENU_ITEM_TYPE>& mItem : cItems) {
        mRight = std::max(mRight, getWidth(*mItem, cStyle));
      }
      mRight = std::max(mRight, minimumWidth) + mFontSize * 2.0f - aspectRatio;
      mRight = std::min(1.0f * aspectRatio, mRight);

      glColor4f(0.0f, 0.0f, 0.0f, 0.75f);
      Utils::renderRoundedRectangle(mLeft, mBottom, mRight, mTop, mFontSize);
      glColor3f(1.0f, 1.0f, 1.0f);
    }

    void render(float aspectRatio) override {
      float mFontSize = cStyle.getFontSize();
      IApplication& mApplication = cUIManager.getProject().getApplication();
      
      ScreenArea mPreviousCrop = mApplication.crop(ScreenArea(-aspectRatio, aspectRatio, -1.0f + mFontSize, 1.0f - mFontSize * 4.0f));
      float mYPosition = (1.0f - mFontSize * 4.0f) + cScroll.animation();
      for (std::unique_ptr<MENU_ITEM_TYPE>& mItem : cItems) {
        mYPosition -= mFontSize * 2.0f;
        glPushMatrix();
        renderMenuItem(*mItem, cStyle, mYPosition, aspectRatio);
        glPopMatrix();
      }
      mApplication.crop(mPreviousCrop);

      if (!cItems.empty()) {
        renderOverlay(*cItems[cSelectedItem], cStyle, (1.0f - (mFontSize * 4.0f + (cSelectedItem + 1) * mFontSize * 2.0f)) - cScroll.animation(), aspectRatio);
      }
    }

    std::string getBreadCrumb() const override {
      return cBreadCrumb;
    }

    void setBreadCrumbColour() const override {
      glColor3f(cBreadCrumbRed, cBreadCrumbGreen, cBreadCrumbBlue);
    }

    float getSelectionHighlightLeft(float aspectRatio) const override {
      if (cItems.empty()) {
        return -1.0f;
      }
      return getSelectionHighlightLeft(*cItems[cSelectedItem], cStyle, aspectRatio);
    }

    float getSelectionHighlightRight(float aspectRatio) const override {
      if (cItems.empty()) {
        return -1.0f;
      }
      return getSelectionHighlightRight(*cItems[cSelectedItem], cStyle, aspectRatio);
    }

    float getSelectionHighlightTop() const override {
      return (1.0f - ((cSelectedItem + 2.0f) * cStyle.getFontSize() * 2.0f)) + cScroll.animation();
    }

    float getSelectionHighlightBottom() const override {
      return getSelectionHighlightTop() - cStyle.getFontSize() * 2.0f;
    }

    void update(unsigned int milliseconds) override {
      cScroll.update(milliseconds);
      updateOverlay(milliseconds);
    }

    bool input(UISignalID id) override {
      float mFontSize = cStyle.getFontSize();
      if (!cItems.empty() && input(*cItems[cSelectedItem], id, (1.0f - (mFontSize * 4.0f + (cSelectedItem + 1) * mFontSize * 2.0f)) + cScroll.value())) {
        return true;
      }

      switch (id) {
        case UISignalID::MOVE_DOWN: {
          if (!cItems.empty() && cSelectedItem < cItems.size() - 1) {
            cSelectedItem++;
            selectedItemChanged();
            updateScrollPosition();
          }
          return true;
        }

        case UISignalID::MOVE_UP: {
          if (cSelectedItem > 0) {
            cSelectedItem--;
            selectedItemChanged();
            updateScrollPosition();
          }
          return true;
        }

        case UISignalID::CANCEL: {
          cUIManager.getProject().updateLater([this]() {
            cUIManager.closeUI();
          });
          return true;
        }

        default: {
        }
      }
      return false;
    }

    bool input(sf::Event& event) override {
      return cItems.empty() ? false : input(*cItems[cSelectedItem], event);
    }

    std::string getTooltip() const override {
      return cItems.empty() ? "" : cItems[cSelectedItem]->getTooltip();
    }

    virtual float getWidth(MENU_ITEM_TYPE& item, IUIStyle& style) const = 0;
    virtual void renderMenuItem(MENU_ITEM_TYPE& item, IUIStyle& style, float y, float aspectRatio) const = 0;
    virtual void renderOverlay(MENU_ITEM_TYPE& item, IUIStyle& style, float y, float aspectRatio) const = 0;
    virtual void updateOverlay(unsigned int milliseconds) = 0;
    virtual float getSelectionHighlightLeft(MENU_ITEM_TYPE& item, IUIStyle& style, float aspectRatio) const = 0;
    virtual float getSelectionHighlightRight(MENU_ITEM_TYPE& item, IUIStyle& style, float aspectRatio) const = 0;
    virtual bool input(MENU_ITEM_TYPE& item, UISignalID id, float y) = 0;
    virtual bool input(MENU_ITEM_TYPE& item, sf::Event& event) = 0;
    virtual void selectedItemChanged() = 0;

    protected:
    void openUI(std::unique_ptr<IUIScreen> screen) {
      cUIManager.openUI(std::move(screen));
    }

    void addItem(std::unique_ptr<MENU_ITEM_TYPE> item) {
      cItems.emplace_back(std::move(item));
    }

    void addItemAfter(MENU_ITEM_TYPE& position, std::unique_ptr<MENU_ITEM_TYPE> item) {
      for (typename std::vector<std::unique_ptr<MENU_ITEM_TYPE>>::iterator i = cItems.begin(); i != cItems.end(); i++) {
        if (&position == i->get()) {
          cItems.insert(i, std::move(item));
          return;
        }
      }
      std::cout << "TODO: Item not added... Throw exception!" << std::endl;
    }

    MENU_ITEM_TYPE& getCurrentItem() {
      return *cItems[cSelectedItem].get();
    }

    void removeItem(MENU_ITEM_TYPE& item) {
      Utils::removeElementUnique(cItems, &item);
    }

    void clear() {
      cItems.clear();
    }

    UIManager& getUIManager() {
      return cUIManager;
    }

    IUIStyle& getStyle() {
      return cStyle;
    }
    
    std::vector<std::unique_ptr<MENU_ITEM_TYPE>>& getItems() {
      return cItems;
    }

    private:
    UIManager& cUIManager;
    IUIStyle& cStyle;
    std::string cBreadCrumb;
    float cBreadCrumbRed;
    float cBreadCrumbGreen;
    float cBreadCrumbBlue;
    std::vector<std::unique_ptr<MENU_ITEM_TYPE>> cItems;
    unsigned int cSelectedItem;
    AnimatedFloat cScroll;
    
    void updateScrollPosition() {
      float mFontSize = cStyle.getFontSize();
      float mMenuHeight = cItems.size() * mFontSize * 2.0f;
      float mMenuSpace = (1.0f - mFontSize * 4.0f) - (-1.0f + mFontSize);
      float mMaxScroll = mMenuHeight - mMenuSpace;
      float mSelectionCenter = cSelectedItem * mFontSize * 2.0f + mFontSize;
      cScroll = std::max(0.0f, std::min(mMaxScroll, mSelectionCenter - mMenuSpace / 2.0f));
    }
  };
}
