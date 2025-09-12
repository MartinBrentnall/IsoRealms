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

#include <memory>
#include <set>
#include <vector>

#include <GL/glew.h>

#include "IsoRealms/Application.h"
#include "IsoRealms/Common/ScreenArea.h"
#include "IsoRealms/Project.h"
#include "IsoRealms/Utils.h"

#include "IUIStyle.h"
#include "UIManager.h"

namespace IsoRealms {
  template <typename MENU_ITEM_TYPE> class Menu : public IUIScreen {
    public:
    Menu(UIManager& manager, IUIStyle& style) :
              cUIManager(manager),
              cStyle(style),
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

      glColor3f(0.0f, 0.0f, 0.0f);
      Utils::renderRoundedRectangle(mLeft, mBottom, cPanelRight, mTop, mFontSize);
      glColor3f(1.0f, 1.0f, 1.0f);
    }

    void render(float aspectRatio) override {
      float mFontSize = cStyle.getFontSize();
      Application& mApplication = cUIManager.getProject().getApplication();
      
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
            updateRight();
            cUIManager.setTooltip(getTooltip());
          }
          return true;
        }

        case UISignalID::MOVE_UP: {
          if (cSelectedItem > 0) {
            cSelectedItem--;
            selectedItemChanged();
            updateScrollPosition();
            updateRight();
            cUIManager.setTooltip(getTooltip());
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

    float getTooltipXPosition() const override {
      return cPanelRight;
    }

    float getContentRight() const override {
      float mAspectRatio = 1.0f / cUIManager.getProject().getApplication().getScreenAspectRatio();
      float mFontSize = cStyle.getFontSize();
      float mPanelRight = 0.0f;
      for (const std::unique_ptr<MENU_ITEM_TYPE>& mItem : cItems) {
        mPanelRight = std::max(mPanelRight, getWidth(*mItem, cStyle));
      }
      return std::min(1.0f * mAspectRatio, mPanelRight + mFontSize * 2.0f - mAspectRatio);
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
    void openUI(std::unique_ptr<IUIScreen> screen, const std::string& breadCrumb, const IColour& breadCrumbColour = LocalColour(1.0f, 1.0f, 1.0f)) {
      cUIManager.openUI(std::move(screen), breadCrumb, breadCrumbColour);
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

    bool isEmpty() {
      return cItems.empty();
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

    void updateRight() override {
      float mAspectRatio = 1.0f / cUIManager.getProject().getApplication().getScreenAspectRatio();
      float mFontSize = cStyle.getFontSize();
      cPanelRight = std::max(getContentRight(), cUIManager.getBreadCrumbWidth() + mFontSize * 2.0f - mAspectRatio);
    }

    private:
    UIManager& cUIManager;
    IUIStyle& cStyle;
    std::vector<std::unique_ptr<MENU_ITEM_TYPE>> cItems;
    unsigned int cSelectedItem;
    AnimatedFloat cScroll;

    // Runtime data.
    float cPanelRight;
    
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
