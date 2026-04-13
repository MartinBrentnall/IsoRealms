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
#include "PropertyTreeSelector.h"

#include <algorithm>
#include <cmath>
#include <regex>
#include <set>

#include <GL/glew.h>

#include "IsoRealms/Application.h"
#include "IsoRealms/Assets/Type/IFont.h"
#include "IsoRealms/Common/ScreenArea.h"
#include "IsoRealms/IResourceData.h"
#include "IsoRealms/PropertyMaker.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  PropertyTreeSelector::PropertyTreeSelector(PropertyMaker& owner, IResourceAccessManager& resourceAccessManager, IResourceData& resourceData, const PropertyData& data, ITreeSelectorObject& item, std::function<void()> removeFunction) :
            Property(data, resourceAccessManager, removeFunction),
            cPropertyOwner(owner),
            cResourceData(resourceData),
            cSelectedItem(item),
            cValueLabel(cSelectedItem.getTreeItemLabel()) {
  }

  std::string PropertyTreeSelector::getValue() {
    return cSelectedItem.getTreeItemLabel();
  }

  void PropertyTreeSelector::renderValue(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cValueLabel);
    glPushMatrix();
    glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    if (!cSelectedItem.renderAssetIcon()) {
      Utils::renderIconLeaf();
    }
    glPopMatrix();
  }

  float PropertyTreeSelector::getValueWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cValueLabel) + mFontSize * 2.25f;
  }

  void PropertyTreeSelector::confirm(IPropertyManager& manager, float y) {
    IUIStyle& mStyle = manager.getPropertyStyle();
    manager.edit(std::make_unique<Editor>(*this, mStyle, y, getValueWidth(mStyle), mStyle.getFontSize() * 2.0f));
  }

  bool PropertyTreeSelector::hasConfiguration() const {
    return cSelectedItem.hasConfiguration();
  }

  void PropertyTreeSelector::configure(IPropertyManager& manager) {
    manager.openProperties(cResourceData, getPropertyName(), [this](PropertyMaker& owner) {
      cSelectedItem.getAssetProperties(owner);
    });
  }

  PropertyTreeSelector::Editor::Editor(PropertyTreeSelector& parent, IUIStyle& style, float y, float width, float height) :
            cParent(parent),
            cStyle(style),
            cY(y),
            cOpenness(0),
            cClosing(false),
            cPathBarWidth(0.0f) {
    cBackArrowSize.init(0.0f);

    TreeItemInfo mTreeItemInfo = cParent.cSelectedItem.getTreeItemInfo();

    std::string::size_type mSeparator = mTreeItemInfo.cPath.find_first_of('/');
    cMenus.emplace_back(std::make_unique<MenuScroller>(*this, "", mTreeItemInfo, mSeparator == std::string::npos ? MenuScroller::State::ACTIVE : MenuScroller::State::INACTIVE));

    while (mSeparator != std::string::npos) {
      std::string::size_type mNextSeparator = mTreeItemInfo.cPath.find_first_of('/', mSeparator + 1);
      cMenus.emplace_back(std::make_unique<MenuScroller>(*this, mTreeItemInfo.cPath.substr(0, mSeparator), mTreeItemInfo, mNextSeparator == std::string::npos ? MenuScroller::State::ACTIVE : MenuScroller::State::INACTIVE));
      mSeparator = mNextSeparator;
    }
    cCurrentPath = mTreeItemInfo.cPath;

    cWidth.init(cMenus.back()->getMenu().getWidth(cStyle));
    cHeight.init(cMenus.back()->getMenu().getMenuItemCount());
    cSelectionWidth.init(cMenus.back()->getMenu().getSelectedItemWidth(cStyle));
    cYPosition.init(cMenus.back()->getMenu().getSelectedItem());
    cBackArrowSize = cMenus.size() > 1 ? 1.0f : 0.0f;
    updatePathBar(style);
  }

  void PropertyTreeSelector::Editor::openFolder(const std::string& path) {
    cMenus.emplace_back(std::make_unique<MenuScroller>(*this, path, cParent.cSelectedItem.getTreeItemInfo(), MenuScroller::State::NEW));
  }

  void PropertyTreeSelector::Editor::updateAnimationValues() {
    cWidth = cMenus.back()->getMenu().getWidth(cStyle);
    cHeight = cMenus.back()->getMenu().getMenuItemCount();
    cSelectionWidth = cMenus.back()->getMenu().getSelectedItemWidth(cStyle);
    cYPosition = cMenus.back()->getMenu().getSelectedItem();
    cBackArrowSize = cMenus.size() > 1 ? 1.0f : 0.0f;
  }

  void PropertyTreeSelector::Editor::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    glBindTexture(GL_TEXTURE_2D, 0);
    float mFontSize = style.getFontSize();
    float mYPosition = (cOpenness / 250.0f * cYPosition.animation()) * mFontSize * 2.0f + cY;
    float mHeight = (cOpenness / 250.0f * (cHeight.animation() - 1.0f) + 1.0f) * mFontSize * 2.0f;
    float mWidth = (cOpenness / 250.0f * (cWidth.animation() - cSelectionWidth.animation()) + cSelectionWidth.animation());

    glEnable(GL_BLEND);
    glColor4f(0.0f, 0.0f, 0.0f, cOpenness / 250.0f * 0.5f);
    glBegin(GL_QUADS);
    Utils::renderRectangle(-aspectRatio * 2.0f, -1.0f, aspectRatio * 2.0f, 1.0f); // TODO: The "* 2.0f" is a hack because we don't know the actual screen width'
    glEnd();
    glDisable(GL_BLEND);

    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    float mLeft = x - mFontSize;
    float mRight = x + mWidth + mFontSize;
    float mBottom = (mYPosition + mFontSize * 2.0f) - mHeight;
    float mTop = mYPosition + mFontSize * 2.0f;
    Utils::renderRoundedRectangle(mLeft, mBottom, mRight, mTop, mFontSize);

    glColor3f(1.0f, 0.0f, 0.2f);
    Utils::renderBar(x, cY, x + cSelectionWidth.animation(), cY + mFontSize * 2.0f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    Utils::renderRoundedRectangleLines(mLeft, mBottom, mRight, mTop, mFontSize);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - mFontSize * 0.25f * cBackArrowSize.animation(), (cY + mFontSize) + mFontSize * 0.5f * cBackArrowSize.animation());
    glVertex2f(x - mFontSize * 0.75f * cBackArrowSize.animation(),  cY + mFontSize);
    glVertex2f(x - mFontSize * 0.25f * cBackArrowSize.animation(), (cY + mFontSize) - mFontSize * 0.5f * cBackArrowSize.animation());
    glEnd();

    Application& mApplication = cParent.cSelectedItem.getApplication();
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

    if (cPathBarWidth.animation() > 0.0f && cMenus[0]->getMenu().hasChildren()) {
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      Utils::renderRoundedRectangle(-aspectRatio - mFontSize, mPathBarY - mFontSize * 1.5f, -aspectRatio + cPathBarWidth.animation() + mFontSize, mPathBarY + mFontSize * 1.5f, mFontSize);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      Utils::renderRoundedRectangleLines(-aspectRatio - mFontSize, mPathBarY - mFontSize * 1.5f, -aspectRatio + cPathBarWidth.animation() + mFontSize, mPathBarY + mFontSize * 1.5f, mFontSize);

      ScreenArea mPathCrop = mApplication.crop(ScreenArea(-1.0f * aspectRatio, -1.0f * aspectRatio + cPathBarWidth.animation(), mPathBarY - mFontSize, mPathBarY + mFontSize));
      mFont->print(-1.0f * aspectRatio,                    1.0f - mFontSize * 3.0f, mFontSize, IFont::Alignment::LEFT, cActivePath);
      glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
      mFont->print(-1.0f * aspectRatio + cActivePartWidth, 1.0f - mFontSize * 3.0f, mFontSize, IFont::Alignment::LEFT, cInactivePath);
      mApplication.crop(mPathCrop);
    }

    if (cConfirmSelection != nullptr) {
      cConfirmSelection->render(style);
    }
    if (cClosedConfirmSelection != nullptr) {
      cClosedConfirmSelection->render(style);
    }
  }

  void PropertyTreeSelector::Editor::updatePathBar(IUIStyle& style) {
    cActivePath = cMenus.size() > 1 ? cMenus[cMenus.size() - 1]->getMenu().getPath() + "/" : "";
    cInactivePath = (cActivePath == cCurrentPath.substr(0, cActivePath.length()))
                  ? cCurrentPath.substr(cActivePath.length())
                  : "";
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    cActivePath = std::regex_replace(cActivePath, std::regex("/"), " > ");
    cInactivePath = std::regex_replace(cInactivePath, std::regex("/"), " > ");
    cActivePartWidth = mFont->getWidth(mFontSize, cActivePath);
    cPathBarWidth = mFont->getWidth(mFontSize, cInactivePath) + cActivePartWidth;
  }

  bool PropertyTreeSelector::Editor::update(unsigned int milliseconds) {
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

  bool PropertyTreeSelector::Editor::input(UISignalID id, IUIStyle& style) {
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

  bool PropertyTreeSelector::Editor::input(sf::Event& event, IUIStyle& style) {
    return false; // Nothing to do.
  }

  PropertyTreeSelector::Editor::Menu::Menu(Editor& parent, const std::string& path, const TreeItemInfo& currentEntry) :
            cParent(parent),
            cPath(path),
            cSelectedIndex(0) {
    std::string mSelectedPath = currentEntry.cPath;
    std::set<std::string> mFolders;
    std::vector<TreeItemInfo> mProviders;

    cParent.cParent.cSelectedItem.forEachAvailableTreeItem([&](const TreeItemInfo& mTreeItemInfo) {
      const std::string& mPath = mTreeItemInfo.cPath;
      if (mPath.substr(0, path.length() + (path.empty() ? 0 : 1)) == (path.empty() ? path : path + "/")) {
        std::string mEntity = mPath.substr(path.length() + (path == "" ? 0 : 1));
        std::string::size_type mSplit = mEntity.find_first_of('/');
        if (mSplit != std::string::npos) {
          mFolders.emplace(mEntity.substr(0, mSplit));
        } else {
          mProviders.push_back(mTreeItemInfo);
        }
      }
    });

    unsigned int mIndex = 0;
    for (const std::string& mFolder : mFolders) {
      std::string mFullFolderPath = path.empty() ? mFolder : path + "/" + mFolder;
      if (mFullFolderPath == mSelectedPath.substr(0, mFullFolderPath.length()) && (mSelectedPath.length() == mFullFolderPath.length() || mSelectedPath[mFullFolderPath.length()] == '/')) {
        cSelectedIndex = mIndex;
      }
      cMenuItems.emplace_back(std::make_unique<MenuFolder>(*this, mFolder));
      mIndex++;
    }

    for (const TreeItemInfo& mTreeItemInfo : mProviders) {
      if (mTreeItemInfo.cPath == mSelectedPath) {
        cSelectedIndex = mIndex;
      }
      cMenuItems.emplace_back(std::make_unique<MenuItem>(*this, mTreeItemInfo));
      mIndex++;
    }
    cSelectedItem.init(cSelectedIndex);
  }

  unsigned int PropertyTreeSelector::Editor::Menu::getMenuItemCount() const {
    return cMenuItems.size();
  }

  void PropertyTreeSelector::Editor::Menu::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    float mFontSize = style.getFontSize();
    float mYPosition = y + cSelectedItem.animation() * mFontSize * 2.0f;
    for (const std::unique_ptr<IMenuEntry>& mItem : cMenuItems) {
      mItem->render(style, mYPosition, x, aspectRatio);
      mYPosition -= mFontSize * 2.0f;
    }
  }

  float PropertyTreeSelector::Editor::Menu::getWidth(IUIStyle& style) const {
    float mMaxWidth = 0.0f;
    for (const std::unique_ptr<IMenuEntry>& mItem : cMenuItems) {
      mMaxWidth = std::max(mMaxWidth, mItem->getWidth(style));
    }
    return mMaxWidth;
  }

  float PropertyTreeSelector::Editor::Menu::getSelectedItemWidth(IUIStyle& style) const {
    return cMenuItems.empty() ? 0.0f : cMenuItems[cSelectedIndex]->getWidth(style);
  }

  void PropertyTreeSelector::Editor::Menu::update(unsigned int milliseconds) {
    cSelectedItem.update(milliseconds);
  }

  float PropertyTreeSelector::Editor::Menu::getSelectedItemAnimation() const {
    return cSelectedItem.animation();
  }

  bool PropertyTreeSelector::Editor::Menu::input(UISignalID id, IUIStyle& style) {
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

  unsigned int PropertyTreeSelector::Editor::Menu::getSelectedItem() const {
    return cSelectedIndex;
  }

  std::string PropertyTreeSelector::Editor::Menu::getPath() const {
    return cPath;
  }

  bool PropertyTreeSelector::Editor::Menu::hasChildren() const {
    for (const std::unique_ptr<IMenuEntry>& mMenuItem : cMenuItems) {
      if (mMenuItem->hasChildren()) {
        return true;
      }
    }
    return false;
  }

  PropertyTreeSelector::Editor::Menu::MenuFolder::MenuFolder(Menu& parent, const std::string& folder) :
            cParent(parent),
            cFolderName(folder) {
  }

  void PropertyTreeSelector::Editor::Menu::MenuFolder::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cFolderName);
    glPushMatrix();
    glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    Utils::renderIconBranch();
    glPopMatrix();
  }

  float PropertyTreeSelector::Editor::Menu::MenuFolder::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cFolderName) + mFontSize * 2.25f;
  }

  bool PropertyTreeSelector::Editor::Menu::MenuFolder::confirm(IUIStyle& style) {
    cParent.cParent.openFolder(cParent.cPath.empty() ? cFolderName : cParent.cPath + "/" + cFolderName);
    cParent.cParent.updatePathBar(style);
    return false;
  }

  void PropertyTreeSelector::Editor::Menu::MenuFolder::enter(IUIStyle& style) {
    confirm(style);
  }

  bool PropertyTreeSelector::Editor::Menu::MenuFolder::hasChildren() const {
    return true;
  }

  PropertyTreeSelector::Editor::Menu::MenuItem::MenuItem(Menu& parent, const TreeItemInfo& itemInfo) :
            cParent(parent),
            cItemInfo(itemInfo),
            cLabel(getLabelFromPath(itemInfo.cPath)) {
  }

  void PropertyTreeSelector::Editor::Menu::MenuItem::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    mFont->print(x + mFontSize * 2.25f, y + 0.01f, mFontSize, IFont::Alignment::LEFT, cLabel);
    glPushMatrix();
    glTranslatef(x + mFontSize, y + mFontSize, 0.0f);
    glScalef(mFontSize, mFontSize, 0.0f);
    if (!cParent.cParent.cParent.cSelectedItem.renderTreeItemIcon(cItemInfo.cID)) {
      Utils::renderIconLeaf();
    }
    glPopMatrix();
  }

  float PropertyTreeSelector::Editor::Menu::MenuItem::getWidth(IUIStyle& style) const {
    IFont* mFont = style.getFont();
    float mFontSize = style.getFontSize();
    return mFont->getWidth(mFontSize, cLabel) + mFontSize * 2.25f;
  }

  bool PropertyTreeSelector::Editor::Menu::MenuItem::confirm(IUIStyle& style) {
    std::string mCurrentID = cParent.cParent.cParent.cSelectedItem.getTreeItemInfo().cID;
    if (cItemInfo.cID != mCurrentID) {
      if (cParent.cParent.cParent.cSelectedItem.hasConfiguration() && !cParent.cParent.cParent.cSelectedItem.isDefaultConfigured()) {
        cParent.cParent.cConfirmSelection = std::make_unique<Choice>(style, "You will lose the configuration of \"" + cParent.cParent.cParent.cValueLabel + "\" if you select a new one", std::vector<std::string>{"Keep \"" + cParent.cParent.cParent.cValueLabel + "\"", "Choose new item"}, [this](const std::string& choice)->bool {
          if (choice == "Choose new item") {
            cParent.applyChange(cItemInfo.cID);
          }
          cParent.cParent.cClosedConfirmSelection = std::move(cParent.cParent.cConfirmSelection);
          cParent.cParent.cConfirmSelection = nullptr;
          return cParent.cParent.cClosing;
        });
      } else {
        cParent.applyChange(cItemInfo.cID);
      }
    } else {
      cParent.cParent.cClosing = true;
    }
    return cParent.cParent.cClosing;
  }

  void PropertyTreeSelector::Editor::Menu::MenuItem::enter(IUIStyle& style) {
    // Nothing to do.
  }

  bool PropertyTreeSelector::Editor::Menu::MenuItem::hasChildren() const {
    return false;
  }

  std::string PropertyTreeSelector::Editor::Menu::MenuItem::getLabelFromPath(const std::string& path) {
    std::string::size_type mLastSeparator = path.find_last_of('/');
    return mLastSeparator == std::string::npos ? path : path.substr(mLastSeparator + 1);
  }

  void PropertyTreeSelector::Editor::Menu::applyChange(const std::string& id) {
    cParent.cParent.confirmAccess([this, id]() {
      cParent.cParent.cSelectedItem.setID(id);
      cParent.cParent.cValueLabel = cParent.cParent.getValue();
      cParent.cClosing = true;
    }, [this]() {
      // Nothing to do.
    });
  }

  PropertyTreeSelector::Editor::MenuScroller::MenuScroller(Editor& parent, const std::string& path, const TreeItemInfo& currentEntry, State state) :
            cMenu(parent, path, currentEntry),
            cScroll(state == State::ACTIVE   ?  0
                  : state == State::INACTIVE ? -200
                  :                             200) {
  }

  void PropertyTreeSelector::Editor::MenuScroller::render(IUIStyle& style, float y, float x, float aspectRatio) const {
    float mWidth = cMenu.getWidth(style);
    glPushMatrix();
    glTranslatef(cScroll / static_cast<float>(200) * aspectRatio * mWidth, 0.0f, 0.0f);
    cMenu.render(style, y, x, aspectRatio);
    glPopMatrix();
  }

  void PropertyTreeSelector::Editor::MenuScroller::updateActive(unsigned int milliseconds) {
    if (cScroll > 0) {
      cScroll = std::max(cScroll - static_cast<int>(milliseconds), 0);
    } else if (cScroll < 0) {
      cScroll = std::min(cScroll + static_cast<int>(milliseconds), 0);
    }
  }

  void PropertyTreeSelector::Editor::MenuScroller::updateInactive(unsigned int milliseconds) {
    cScroll = std::max(cScroll - static_cast<int>(milliseconds), -200);
  }

  void PropertyTreeSelector::Editor::MenuScroller::updateClosing(unsigned int milliseconds) {
    cScroll = std::min(cScroll + static_cast<int>(milliseconds), 200);
  }

  bool PropertyTreeSelector::Editor::MenuScroller::isClosed() const {
    return cScroll == 200;
  }

  PropertyTreeSelector::Editor::Menu& PropertyTreeSelector::Editor::MenuScroller::getMenu() {
    return cMenu;
  }
}
