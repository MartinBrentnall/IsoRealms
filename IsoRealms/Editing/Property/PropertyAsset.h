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

#include <cmath>
#include <regex>
#include <set>

#include <GL/glew.h>

#include "IsoRealms/AnimatedFloat.h"
#include "IsoRealms/Common/ScreenArea.h"
#include "IsoRealms/Editing/Choice.h"
#include "IsoRealms/Editing/IUIStyle.h"
#include "IsoRealms/IApplication.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

#include "IPropertyManager.h"
#include "Property.h"

namespace IsoRealms {
  template<class TYPE> class PropertyAsset : public Property {
    public:
    PropertyAsset(const std::string& name, TYPE& asset, std::function<void()> removeFunction = nullptr) :
              Property(name, removeFunction),
              cAsset(asset),
              cValueLabel(getValue()) {
    }
    
    std::string getValue() {
      std::string mID = cAsset.getID();
      return mID.substr(mID.find_last_of('/') + 1);
    }

    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override {
      IFont* mFont = style.getFont();
      float mFontSize = style.getFontSize();
      mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cValueLabel);
      glPushMatrix();
      glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
      glScalef(mFontSize, mFontSize, 0.0f);
      if (!cAsset.renderAssetIcon()) {
        Utils::renderIconLeaf();
      }
      glPopMatrix();
    }
    
    float getValueWidth(IUIStyle& style) const override {
      IFont* mFont = style.getFont();
      float mFontSize = style.getFontSize();
      return mFont->getWidth(mFontSize, cValueLabel) + mFontSize * 2.25f;
    }

    void confirm(IPropertyManager& manager, float y) override {
      IUIStyle& mStyle = manager.getPropertyStyle();
      manager.edit(std::make_unique<Editor>(*this, mStyle, y, getValueWidth(mStyle), mStyle.getFontSize() * 2.0f));
    }
    
    bool hasConfiguration() const override {
      return cAsset.hasConfiguration();
    }
    
    void configure(IPropertyManager& manager) override {
      manager.openProperties(getPropertyName(), [this]() {
        return cAsset.getAssetProperties();
      });
    }
  
    private:
    class Editor : public IPropertyEditor {
      public:
      Editor(PropertyAsset<TYPE>& parent, IUIStyle& style, float y, float width, float height) :
                cParent(parent),
                cStyle(style),
                cY(y),
                cOpenness(0),
                cClosing(false),
                cPathBarWidth(0.0f) {
        cBackArrowSize.init(0.0f);

        std::string mAssetID = cParent.cAsset.getID();

        // Construct menus based on current asset ID.
        std::string::size_type mSeparator = mAssetID.find_first_of('/');
        cMenus.emplace_back(std::make_unique<MenuScroller>(*this, "", mAssetID, mSeparator == std::string::npos ? MenuScroller::State::ACTIVE : MenuScroller::State::INACTIVE));

        while (mSeparator != std::string::npos) {
          std::string::size_type mNextSeparator = mAssetID.find_first_of('/', mSeparator + 1);
          cMenus.emplace_back(std::make_unique<MenuScroller>(*this, mAssetID.substr(0, mSeparator), mAssetID, mNextSeparator == std::string::npos ? MenuScroller::State::ACTIVE : MenuScroller::State::INACTIVE));
          mSeparator = mNextSeparator;
        }

        cWidth.init(cMenus.back()->getMenu().getWidth(cStyle));
        cHeight.init(cMenus.back()->getMenu().getMenuItemCount());
        cSelectionWidth.init(cMenus.back()->getMenu().getSelectedItemWidth(cStyle));
        cYPosition.init(cMenus.back()->getMenu().getSelectedItem());
        cBackArrowSize = cMenus.size() > 1 ? 1.0f : 0.0f;
        updatePathBar(style);
      }

      void openFolder(const std::string& path) {
        std::string mAssetID = cParent.cAsset.getID();
        cMenus.emplace_back(std::make_unique<MenuScroller>(*this, path, mAssetID, MenuScroller::State::NEW));
      }

      void updateAnimationValues() {
        cWidth = cMenus.back()->getMenu().getWidth(cStyle);
        cHeight = cMenus.back()->getMenu().getMenuItemCount();
        cSelectionWidth = cMenus.back()->getMenu().getSelectedItemWidth(cStyle);
        cYPosition = cMenus.back()->getMenu().getSelectedItem();
        cBackArrowSize = cMenus.size() > 1 ? 1.0f : 0.0f;
      }

      /******************************\
       * Implements IPropertyEditor *
      \******************************/
      void render(IUIStyle& style, float y, float x, float aspectRatio) const override {
        glBindTexture(GL_TEXTURE_2D, 0);
        float mFontSize = style.getFontSize();
        float mYPosition = (cOpenness / 250.0f * cYPosition.animation()) * mFontSize * 2.0f + cY;
        float mHeight = (cOpenness / 250.0f * (cHeight.animation() - 1.0f) + 1.0f) * mFontSize * 2.0f;
        float mWidth = (cOpenness / 250.0f * (cWidth.animation() - cSelectionWidth.animation()) + cSelectionWidth.animation());

        // Dim everything behind the editor
        glEnable(GL_BLEND);
        glColor4f(0.0f, 0.0f, 0.0f, cOpenness / 250.0f * 0.5f);
        glBegin(GL_QUADS);
        Utils::renderRectangle(-aspectRatio * 2.0f, -1.0f, aspectRatio * 2.0f, 1.0f); // TODO: The "* 2.0f" is a hack because we don't know the actual screen width'
        glEnd();
        glDisable(GL_BLEND);

        // Render black background.
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        Utils::renderRoundedRectangle(x - mFontSize, (mYPosition + mFontSize * 2.0f) - mHeight, x + mWidth + mFontSize, mYPosition + mFontSize * 2.0f, mFontSize);

        // Render cursor.
        glColor3f(1.0f, 0.0f, 0.2f);
        Utils::renderBar(x, cY, x + cSelectionWidth.animation(), cY + mFontSize * 2.0f);

        // Render background line border.
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        Utils::renderRoundedRectangleLines(x - mFontSize, (mYPosition + mFontSize * 2.0f) - mHeight, x + mWidth + mFontSize, mYPosition + mFontSize * 2.0f, mFontSize);

        // Render arrow indicating left possibility.
        glBegin(GL_TRIANGLES);
        glVertex2f(x - mFontSize * 0.25f * cBackArrowSize.animation(), (cY + mFontSize) + mFontSize * 0.5f * cBackArrowSize.animation());
        glVertex2f(x - mFontSize * 0.75f * cBackArrowSize.animation(),  cY + mFontSize);
        glVertex2f(x - mFontSize * 0.25f * cBackArrowSize.animation(), (cY + mFontSize) - mFontSize * 0.5f * cBackArrowSize.animation());
        glEnd();

        // Render menu contents.
        IApplication& mApplication = cParent.cAsset.getApplication();
        ScreenArea mPreviousCrop = mApplication.crop(ScreenArea(x, x + mWidth, (mYPosition + mFontSize * 2.0f) - mHeight, mYPosition + mFontSize * 2.0f));
        for (const std::unique_ptr<MenuScroller>& mMenu : cMenus) {
          mMenu->render(cStyle, cY, x, aspectRatio);
        }
        for (const std::unique_ptr<MenuScroller>& mMenu : cClosedMenus) {
          mMenu->render(cStyle, cY, x, aspectRatio);
        }
        mApplication.crop(mPreviousCrop);
        
        float mPathBarY = 1.0f - mFontSize * 2;
        IFont* mFont = style.getFont();

        // Render path bar.
        if (cPathBarWidth.animation() > 0.0f) {
          glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
          Utils::renderRoundedRectangle(-aspectRatio - mFontSize, mPathBarY - mFontSize * 1.5f, -aspectRatio + cPathBarWidth.animation() + mFontSize, mPathBarY + mFontSize * 1.5f, mFontSize);
          glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
          Utils::renderRoundedRectangleLines(-aspectRatio - mFontSize, mPathBarY - mFontSize * 1.5f, -aspectRatio + cPathBarWidth.animation() + mFontSize, mPathBarY + mFontSize * 1.5f, mFontSize);
          
          ScreenArea mPreviousCrop = mApplication.crop(ScreenArea(-1.0f * aspectRatio, -1.0f * aspectRatio + cPathBarWidth.animation(), mPathBarY - mFontSize, mPathBarY + mFontSize));
          mFont->print(-1.0f * aspectRatio,                    1.0f - mFontSize * 3.0f, mFontSize, IFont::Alignment::LEFT, cActivePath);
          glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
          mFont->print(-1.0f * aspectRatio + cActivePartWidth, 1.0f - mFontSize * 3.0f, mFontSize, IFont::Alignment::LEFT, cInactivePath);
          mApplication.crop(mPreviousCrop);
        }
        
        // Render confirmation choice.
        if (cConfirmSelection != nullptr) {
          cConfirmSelection->render(style);
        }
        if (cClosedConfirmSelection != nullptr) {
          cClosedConfirmSelection->render(style);
        }
      }

      void updatePathBar(IUIStyle& style) {
        cActivePath = cMenus.size() > 1 ? cMenus[cMenus.size() - 1]->getMenu().getPath() + "/" : "";
        std::string mAssetID = cParent.cAsset.getID();
        cInactivePath = (cActivePath == mAssetID.substr(0, cActivePath.length()))
                      ? mAssetID.substr(cActivePath.length())
                      : "";
        IFont* mFont = style.getFont();
        float mFontSize = style.getFontSize();
        cActivePath = std::regex_replace(cActivePath, std::regex("/"), " > ");
        cInactivePath = std::regex_replace(cInactivePath, std::regex("/"), " > ");
        cActivePartWidth = mFont->getWidth(mFontSize, cActivePath);
        cPathBarWidth = mFont->getWidth(mFontSize, cInactivePath) + cActivePartWidth;
      }

      bool update(unsigned int milliseconds) override {
        cWidth.update(milliseconds);
        cHeight.update(milliseconds);
        cSelectionWidth.update(milliseconds);
        cYPosition.update(milliseconds);
        cBackArrowSize.update(milliseconds);
        cPathBarWidth.update(milliseconds);
        
        if (cClosing) {
          cOpenness = std::max(cOpenness - static_cast<int>(milliseconds), 0);
        } else if (cOpenness < 250) {
          cOpenness = std::min(cOpenness + static_cast<int>(milliseconds), 250);
        }

        for (unsigned int i = 0; i < cMenus.size(); i++) {
          if (i == cMenus.size() - 1) {
            cMenus[i]->updateActive(milliseconds);
          } else {
            cMenus[i]->updateInactive(milliseconds);
          }
          cMenus[i]->getMenu().update(milliseconds);
        }

        for (const std::unique_ptr<MenuScroller>& mMenu : cClosedMenus) {
          mMenu->updateClosing(milliseconds);
        }

        while (!cClosedMenus.empty() && cClosedMenus.back()->isClosed()) {
          cClosedMenus.pop_back();
        }
        
        if (cConfirmSelection != nullptr) {
          cConfirmSelection->update(milliseconds);
        }
        if (cClosedConfirmSelection != nullptr) {
          if (cClosedConfirmSelection->update(milliseconds)) {
            cClosedConfirmSelection = nullptr;
          }
        }

        return cOpenness == 0;
      }

      bool input(UISignalID id, IUIStyle& style) override {
        if (cConfirmSelection != nullptr) {
          return cConfirmSelection->input(id);
        } if (cMenus.back()->getMenu().input(id, style)) {
          return true;
        } else switch (id) {
          case UISignalID::CANCEL: {
            cClosing = true;
            return true;
            break;
          }

          case UISignalID::MOVE_LEFT: {
            if (cMenus.size() > 1) {
              cClosedMenus.emplace(cClosedMenus.begin(), std::move(cMenus.back()));
              cMenus.pop_back();
              updatePathBar(style);
            }
            break;
          }

          default: {
            break;
          }
        }
        updateAnimationValues();
        return false;
      }

      bool input(sf::Event& event, IUIStyle& style) override {
        return false; // Nothing to do.
      }

      private:
      class Menu {
        public:
        Menu(Editor& parent, const std::string& path, const std::string& id) :
                  cParent(parent),
                  cPath(path),
                  cSelectedIndex(0) {
          std::vector<std::string> mAllIDs = cParent.cParent.cAsset.getAvailableProviders();

          std::set<std::string> mFolders;
          std::vector<std::string> mProviders;

          for (std::string& mID : mAllIDs) {
            if (mID.substr(0, path.length() + (path.empty() ? 0 : 1)) == (path.empty() ? path : path + "/")) {
              std::string mEntity = mID.substr(path.length() + (path == "" ? 0 : 1));
              std::string::size_type mSplit = mEntity.find_first_of('/');
              if (mSplit != std::string::npos) {
                mFolders.emplace(mEntity.substr(0, mSplit));
              } else {
                mProviders.emplace_back(mID);
              }
            }
          }

          unsigned int mIndex = 0;
          for (const std::string& mFolder : mFolders) {
            std::string mFullFolderPath = path.empty() ? mFolder : path + "/" + mFolder;
            if (mFullFolderPath == id.substr(0, mFullFolderPath.length()) && (id.length() == mFullFolderPath.length() || id[mFullFolderPath.length()] == '/')) {
              cSelectedIndex = mIndex;
            }
            cMenuItems.emplace_back(std::make_unique<AssetMenuItemFolder>(*this, mFolder));
            mIndex++;
          }

          for (std::string& mProvider : mProviders) {
            if (mProvider == id) {
              cSelectedIndex = mIndex;
            }
            cMenuItems.emplace_back(std::make_unique<AssetMenuItemAsset>(*this, mProvider));
            mIndex++;
          }
          cSelectedItem.init(cSelectedIndex);
        }

        unsigned int getMenuItemCount() const {
          return cMenuItems.size();
        }

        void render(IUIStyle& style, float y, float x, float aspectRatio) const {
          float mFontSize = style.getFontSize();
          float mYPosition = y + cSelectedItem.animation() * mFontSize * 2.0f;
          for (const std::unique_ptr<IAssetMenuItem>& mItem : cMenuItems) {
            mItem->render(style, mYPosition, x, aspectRatio);
            mYPosition -= mFontSize * 2.0f;
          }
        }

        float getWidth(IUIStyle& style) const {
          float mMaxWidth = 0.0f;
          for (const std::unique_ptr<IAssetMenuItem>& mItem : cMenuItems) {
            mMaxWidth = std::max(mMaxWidth, mItem->getWidth(style));
          }
          return mMaxWidth;
        }

        float getSelectedItemWidth(IUIStyle& style) const {
          return cMenuItems.empty() ? 0.0f : cMenuItems[cSelectedIndex]->getWidth(style);
        }

        void update(unsigned int milliseconds) {
          cSelectedItem.update(milliseconds);
        }

        float getSelectedItemAnimation() const {
          return cSelectedItem.animation();
        }

        bool input(UISignalID id, IUIStyle& style) {
          switch (id) {
            case UISignalID::CONFIRM: {
              return cMenuItems[cSelectedIndex]->confirm(style);
            }

            case UISignalID::MOVE_RIGHT: {
              cMenuItems[cSelectedIndex]->enter(style);
              break;
            }

            case UISignalID::MOVE_DOWN: {
              if (cSelectedIndex < cMenuItems.size() - 1) {
                cSelectedIndex += 1;
                cSelectedItem = cSelectedIndex;
              }
              break;
            }

            case UISignalID::MOVE_UP: {
              if (cSelectedIndex > 0) {
                cSelectedIndex -= 1;
                cSelectedItem = cSelectedIndex;
              }
              break;
            }

            default: {
              break;
            }
          }
          return false;
        }

        unsigned int getSelectedItem() const {
          return cSelectedIndex;
        }
        
        std::string getPath() const {
          return cPath;
        }

        private:
        class IAssetMenuItem {
          public:
          virtual void render(IUIStyle& style, float y, float x, float aspectRatio) const = 0;
          virtual float getWidth(IUIStyle& style) const = 0;
          virtual bool confirm(IUIStyle& style) = 0;
          virtual void enter(IUIStyle& style) = 0;
        };

        class AssetMenuItemFolder : public IAssetMenuItem {
          public:
          AssetMenuItemFolder(Menu& parent, const std::string& folder) :
                    cParent(parent),
                    cFolderName(folder) {
          }
          
          /*****************************\
           * Implements IAssetMenuItem *
          \*****************************/
          void render(IUIStyle& style, float y, float x, float aspectRatio) const override {
            IFont* mFont = style.getFont();
            float mFontSize = style.getFontSize();
            mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cFolderName);
            glPushMatrix();
            glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
            glScalef(mFontSize, mFontSize, 0.0f);
            Utils::renderIconBranch();
            glPopMatrix();
          }
          
          float getWidth(IUIStyle& style) const override {
            IFont* mFont = style.getFont();
            float mFontSize = style.getFontSize();
            return mFont->getWidth(mFontSize, cFolderName) + mFontSize * 2.25f;
          }

          bool confirm(IUIStyle& style) override {
            cParent.cParent.openFolder(cParent.cPath.empty() ? cFolderName : cParent.cPath + "/" + cFolderName);
            cParent.cParent.updatePathBar(style);
            return false;
          }

          void enter(IUIStyle& style) override {
            confirm(style);
          }

          private:
          Menu& cParent;
          std::string cFolderName;
        };

        class AssetMenuItemAsset : public IAssetMenuItem {
          public:
          AssetMenuItemAsset(Menu& parent, const std::string& id) :
                    cParent(parent),
                    cAssetID(id),
                    cAssetLabel(getLabel(id)) {
          }
          
          /*****************************\
           * Implements IAssetMenuItem *
          \*****************************/
          void render(IUIStyle& style, float y, float x, float aspectRatio) const override {
            IFont* mFont = style.getFont();
            float mFontSize = style.getFontSize();
            mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cAssetLabel);
            glPushMatrix();
            glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
            glScalef(mFontSize, mFontSize, 0.0f);
            if (!cParent.cParent.cParent.cAsset.renderProviderIcon(cAssetID)) {
              Utils::renderIconLeaf();
            }
            glPopMatrix();
          }
          
          float getWidth(IUIStyle& style) const override {
            IFont* mFont = style.getFont();
            float mFontSize = style.getFontSize();
            return mFont->getWidth(mFontSize, cAssetLabel) + mFontSize * 2.25f;
          }

          bool confirm(IUIStyle& style) override {
            std::string mCurrentAssetID = cParent.cParent.cParent.cAsset.getID();
            if (cAssetID != mCurrentAssetID) {
              if (cParent.cParent.cParent.cAsset.hasConfiguration() && !cParent.cParent.cParent.cAsset.isDefaultConfigured()) {
                cParent.cParent.cConfirmSelection = std::make_unique<Choice>(style, "You will lose the configuration of \"" + cParent.cParent.cParent.cValueLabel + "\" if you select a new one", std::vector<std::string>{"Keep \"" + cParent.cParent.cParent.cValueLabel + "\"", "Choose new asset"}, [this](const std::string& choice)->bool {
                  if (choice == "Choose new asset") {
                    cParent.cParent.cParent.cAsset.setID(cAssetID);
                    cParent.cParent.cParent.cValueLabel = cParent.cParent.cParent.getValue();
                    cParent.cParent.cClosing = true;
                  }
                  cParent.cParent.cClosedConfirmSelection = std::move(cParent.cParent.cConfirmSelection);
                  cParent.cParent.cConfirmSelection = nullptr;
                  return cParent.cParent.cClosing;
                });
              } else {
                cParent.cParent.cParent.cAsset.setID(cAssetID);
                cParent.cParent.cParent.cValueLabel = cParent.cParent.cParent.getValue();
                cParent.cParent.cClosing = true;
              }
            } else {
              cParent.cParent.cClosing = true;
            }
            return cParent.cParent.cClosing;
          }

          void enter(IUIStyle& style) override {
            // Nothing to do.
          }

          private:
          Menu& cParent;
          std::string cAssetID;
          std::string cAssetLabel;
          
          static std::string getLabel(const std::string& id) {
            std::string::size_type mLastSeparator = id.find_last_of('/');
            return mLastSeparator == std::string::npos ? id : id.substr(mLastSeparator + 1);
          }
        };

        Editor& cParent;
        std::string cPath;
        std::vector<std::unique_ptr<IAssetMenuItem>> cMenuItems;
        unsigned int cSelectedIndex;
        AnimatedFloat cSelectedItem;
      };

      class MenuScroller {
        public:
        enum class State {
          ACTIVE,
          INACTIVE,
          NEW
        };

        MenuScroller(Editor& parent, const std::string& path, const std::string& id, State state) :
                  cMenu(parent, path, id),
                  cScroll(state == State::ACTIVE   ?  0
                        : state == State::INACTIVE ? -200
                        :                             200) {
        }

        void render(IUIStyle& style, float y, float x, float aspectRatio) const {
          float mWidth = cMenu.getWidth(style);
          glPushMatrix();
          glTranslatef(cScroll / static_cast<float>(200) * aspectRatio * mWidth, 0.0f, 0.0f);
          cMenu.render(style, y, x, aspectRatio);
          glPopMatrix();
        }

        void updateActive(unsigned int milliseconds) {
          if (cScroll > 0) {
            cScroll = std::max(cScroll - static_cast<int>(milliseconds), 0);
          } else if (cScroll < 0) {
            cScroll = std::min(cScroll + static_cast<int>(milliseconds), 0);
          }
        }

        void updateInactive(unsigned int milliseconds) {
          cScroll = std::max(cScroll - static_cast<int>(milliseconds), -200);
        }

        void updateClosing(unsigned int milliseconds) {
          cScroll = std::min(cScroll + static_cast<int>(milliseconds), 200);
        }

        bool isClosed() const {
          return cScroll == 200;
        }

        Menu& getMenu() {
          return cMenu;
        }

        private:
        Menu cMenu;
        int cScroll;
      };

      PropertyAsset<TYPE>& cParent;
      IUIStyle& cStyle;

      float cY;

      AnimatedFloat cYPosition;
      AnimatedFloat cWidth;
      AnimatedFloat cHeight;
      AnimatedFloat cSelectionWidth;
      AnimatedFloat cBackArrowSize;
      int cOpenness;
      bool cClosing;
      std::vector<std::unique_ptr<MenuScroller>> cMenus;
      std::vector<std::unique_ptr<MenuScroller>> cClosedMenus;
      std::unique_ptr<Choice> cConfirmSelection;
      std::unique_ptr<Choice> cClosedConfirmSelection;

      // Path bar stuff
      std::string cActivePath;
      std::string cInactivePath;
      float cActivePartWidth;
      AnimatedFloat cPathBarWidth;
    };

    TYPE& cAsset;
    std::string cValueLabel;
  };
}
