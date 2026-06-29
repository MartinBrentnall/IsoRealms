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
#include "PropertiesMenu.h"

#include "IsoRealms/Application.h"
#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/ComponentType.h"
#include "IsoRealms/Editing/ComponentEditor.h"
#include "IsoRealms/Editing/PropertyData.h"

#include "Property/IPropertyEditor.h"
#include "Property/PropertyRemover.h"

namespace IsoRealms {
  PropertiesMenu::PropertiesMenu(UIManager& manager, IUIStyle& style, ComponentEditor& componentEditor, IComponentData& owner, std::function<void(IComponentDefiner& definer)> propertyFetcher) : 
            Menu(manager, style),
            cComponentEditor(componentEditor),
            cOwner(owner),
            cPropertyFetcher(propertyFetcher),
            cEditingProperty(nullptr),
            cClosingProperty(nullptr),
            cColumnWidthLabel(0.0f),
            cColumnWidthValue(0.0f),
            cHasConfigureColumn(false),
            cHasRemoveColumn(false),
            cAction(Action::SELECT),
            cFetching(false),
            cIndentLevel(0) {
    cComponentEditor.openMenu(*this);
    cMetadata = &cComponentEditor.getParentMenuMetadata();
    refreshProperties();
  }

  void PropertiesMenu::recalculateColumnWidths() {
    cColumnWidthLabel = 0.0f;
    cColumnWidthValue = 0.0f;
    cHasConfigureColumn = false;
    cHasRemoveColumn = false;
    IUIStyle& mStyle = getStyle();
    for (std::unique_ptr<IMenuItem>& mItem : getItems()) {
      IProperty* mProperty = mItem->getProperty();
      if (mProperty != nullptr) {
        cColumnWidthLabel = std::max(cColumnWidthLabel.value(), mItem->getWidth(mStyle));
        cColumnWidthValue = std::max(cColumnWidthValue.value(), mProperty->getValueWidth(mStyle));
        if (mProperty->hasConfiguration()) {
          cHasConfigureColumn = true;
        }

        if (mProperty->isRemovable()) {
          cHasRemoveColumn = true;
        }
      }
    }
  }

  float PropertiesMenu::getNameValueSeparationWidth(IUIStyle& style) const {
    return cColumnWidthLabel.value() == 0.0f ? 0.0f : style.getFontSize() * 2.25f;
  }

  float PropertiesMenu::getValueColumnX(IMenuItem& item, IUIStyle& style, float aspectRatio) const {
    return -1.0f * aspectRatio + item.getIndentation(style) + cColumnWidthLabel.animation() + getNameValueSeparationWidth(style);
  }

  bool PropertiesMenu::isMouseOverValueColumn(IMenuItem& item, IUIStyle& style, float x, float aspectRatio) const {
    float mLeft = getValueColumnX(item, style, aspectRatio);
    return x >= mLeft && x <= mLeft + item.getValueWidth(style);
  }

  int PropertiesMenu::findItemIndexAtValueColumn(float x, float y, float aspectRatio) {
    int mItemIndex = findItemIndexAtY(y);
    if (mItemIndex >= 0) {
      IUIStyle& mStyle = getStyle();
      if (isMouseOverValueColumn(*getItems()[static_cast<unsigned int>(mItemIndex)], mStyle, x, aspectRatio)) {
        return mItemIndex;
      }
    }
    return -1;
  }

  float PropertiesMenu::getWidth(IMenuItem& item, IUIStyle& style) const {
    //     Property name:              Spacing                              Property value              Remove icon                                              Configure icon          
    return cColumnWidthLabel.value() + getNameValueSeparationWidth(style) + cColumnWidthValue.value() + style.getFontSize() * (cHasRemoveColumn ? 3.0f : 0.0f) + style.getFontSize() * (cHasConfigureColumn ? 3.0f : 0.0f);
  }

  float PropertiesMenu::getHeight(IMenuItem& item, IUIStyle& style) const {
    return item.getHeight(style);
  }

  void PropertiesMenu::renderMenuItem(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const {
    IProperty* mProperty = item.getProperty();
    if (mProperty != nullptr) {
      float mFontSize = style.getFontSize();
      float mValueColumnX = getValueColumnX(item, style, aspectRatio);
      item.render(style, y, mValueColumnX, aspectRatio);
      if (mProperty->hasConfiguration()) {
        glPushMatrix();
        glTranslatef(mValueColumnX + cColumnWidthValue.animation() + mFontSize * 2.25f, y + mFontSize, 0.0f);
        glScalef(mFontSize * 0.8f, mFontSize * 0.8f, 0.0f);
        Utils::renderIconCustom();
        glPopMatrix();
      }

      if (mProperty->isRemovable()) {
        glPushMatrix();
        float mPosition = (cHasConfigureColumn ? 4.50f : 2.25f) + getNameValueSeparationWidth(style);
        glTranslatef(mValueColumnX + cColumnWidthValue.animation() + mFontSize * mPosition, y + mFontSize, 0.0f);
        glScalef(mFontSize * 0.8f, mFontSize * 0.8f, 0.0f);
        Utils::renderIconNone();
        glPopMatrix();
      }
    }
  }
  
  void PropertiesMenu::renderOverlay(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const {
    float mValueColumnX = getValueColumnX(item, style, aspectRatio);
    if (cEditingProperty != nullptr) {
      cEditingProperty->render(style, y, mValueColumnX, aspectRatio);
    }
    if (cClosingProperty != nullptr) {
      cClosingProperty->render(style, y, mValueColumnX, aspectRatio);
    }
  }
  
  void PropertiesMenu::updateOverlay(unsigned int milliseconds) {
    cColumnWidthLabel.update(milliseconds);
    cColumnWidthValue.update(milliseconds);
    if (cEditingProperty != nullptr) {
      cEditingProperty->update(milliseconds);
    }
    if (cClosingProperty != nullptr) {
      if (cClosingProperty->update(milliseconds)) {
        cClosingProperty = nullptr;
      }
    }
  }

  float PropertiesMenu::getSelectionHighlightLeft(IMenuItem& item, IUIStyle& style, float aspectRatio) const {
    float mFontSize = style.getFontSize();
    float mValueColumnX = getValueColumnX(item, style, aspectRatio);
    return cAction == Action::SELECT    ? mValueColumnX
         : cAction == Action::CONFIGURE ? mValueColumnX + cColumnWidthValue.animation() + mFontSize * 2.25f
         :                                mValueColumnX + cColumnWidthValue.animation() + mFontSize * (cHasConfigureColumn ? 4.50f : 2.25f);
  }
  
  float PropertiesMenu::getSelectionHighlightRight(IMenuItem& item, IUIStyle& style, float aspectRatio) const {
    return cAction == Action::SELECT ? getSelectionHighlightLeft(item, style, aspectRatio) + item.getValueWidth(style)
                                     : getSelectionHighlightLeft(item, style, aspectRatio);
  }
  
  bool PropertiesMenu::input(IMenuItem& item, UISignalID id, float y) {
    if (cEditingProperty != nullptr) {
      if (cEditingProperty->input(id, getStyle())) {
        getUIManager().getProject().updateLater([this]() {
          cClosingProperty = std::move(cEditingProperty);
          cEditingProperty = nullptr;
          recalculateColumnWidths();
        });
      }
      return true;
    }

    switch (id) {
      case UISignalID::MOVE_RIGHT: {
        switch (cAction) {
          case Action::SELECT: {
            if (item.getProperty()->hasConfiguration()) {
              cAction = Action::CONFIGURE;
            } else if (item.getProperty()->isRemovable()) {
              cAction = Action::REMOVE;
            }
            break;
          }
          
          case Action::CONFIGURE: {
            if (item.getProperty()->isRemovable()) {
              cAction = Action::REMOVE;
            }
            break;
          }
          
          case Action::REMOVE: {
            // Nothing to do.
            break;
          }
        }
        return true;
      }
      
      case UISignalID::MOVE_LEFT: {
        switch (cAction) {
          case Action::SELECT: {
            // Nothing to do.
            break;
          }
          
          case Action::CONFIGURE: {
            cAction = Action::SELECT;
            break;
          }
          
          case Action::REMOVE: {
            cAction = item.getProperty()->hasConfiguration() ? Action::CONFIGURE : Action::SELECT;
            break;
          }
        }
        return true;
      }
      
      case UISignalID::CONFIRM: {
        switch (cAction) {
          case Action::SELECT: {
            getUIManager().getProject().updateLater([this, &item, y]() {
              item.getProperty()->confirm(*this, y);
            });
            break;
          }
          
          case Action::CONFIGURE: {
            getUIManager().getProject().updateLater([this, &item]() {
              item.getProperty()->configure(*this);
            });
            break;
          }
          
          case Action::REMOVE: {
            getUIManager().getProject().updateLater([this, &item]() {
              item.getProperty()->remove();
              removeItem(item);
            });
            cClosingProperty = nullptr;
            break;
          }
        }
        return true;
      }

      default: {
        // Nothing to do.
      }
    }
    
    return false;
  }
  
  bool PropertiesMenu::input(sf::Event& event) {
    if (getItems().empty()) {
      return false;
    }

    IMenuItem& mSelectedItem = getCurrentItem();
    if (cEditingProperty != nullptr || cClosingProperty != nullptr) {
      return input(mSelectedItem, event);
    }

    if (event.type == sf::Event::MouseMoved) {
      Application& mApplication = getUIManager().getProject().getApplication();
      Point2D mLocation = mApplication.normalise(event.mouseMove.x, event.mouseMove.y);
      float mAspectRatio = 1.0f / mApplication.getScreenAspectRatio();
      float mFontSize = getStyle().getFontSize();
      if (mLocation.getX() >= -mAspectRatio && mLocation.getX() <= getContentRight()
          && mLocation.getY() >= -1.0f + mFontSize && mLocation.getY() <= 1.0f - mFontSize * 4.0f) {
        int mItemIndex = findItemIndexAtValueColumn(mLocation.getX(), mLocation.getY(), mAspectRatio);
        if (mItemIndex >= 0 && static_cast<unsigned int>(mItemIndex) != getSelectedItemIndex()) {
          selectItem(static_cast<unsigned int>(mItemIndex), false);
        }
        return true;
      }
      return false;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
      Application& mApplication = getUIManager().getProject().getApplication();
      Point2D mLocation = mApplication.normalise(event.mouseButton.x, event.mouseButton.y);
      float mAspectRatio = 1.0f / mApplication.getScreenAspectRatio();
      if (mLocation.getX() >= -mAspectRatio && mLocation.getX() <= getContentRight()) {
        int mItemIndex = findItemIndexAtValueColumn(mLocation.getX(), mLocation.getY(), mAspectRatio);
        if (mItemIndex >= 0) {
          selectItem(static_cast<unsigned int>(mItemIndex), false);
          IMenuItem& mItem = *getItems()[static_cast<unsigned int>(mItemIndex)];
          return input(mItem, UISignalID::CONFIRM, getItemYPosition(static_cast<unsigned int>(mItemIndex)));
        }
      }
    }

    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
      Application& mApplication = getUIManager().getProject().getApplication();
      Point2D mLocation = mApplication.normalise(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
      float mAspectRatio = 1.0f / mApplication.getScreenAspectRatio();
      float mFontSize = getStyle().getFontSize();
      if (mLocation.getX() >= -mAspectRatio && mLocation.getX() <= getContentRight()
          && mLocation.getY() >= -1.0f + mFontSize && mLocation.getY() <= 1.0f - mFontSize * 4.0f) {
        scrollBy(-event.mouseWheelScroll.delta * mFontSize * 2.0f);
        return true;
      }
    }

    return input(mSelectedItem, event);
  }

  bool PropertiesMenu::input(IMenuItem& item, sf::Event& event) {
    if (cEditingProperty != nullptr) {
      if (cEditingProperty->input(event, getStyle())) {
        getUIManager().getProject().updateLater([this]() {
          cClosingProperty = std::move(cEditingProperty);
          cEditingProperty = nullptr;
          recalculateColumnWidths();
          // refreshProperties(); TODO: Doing this nulls out the closing property... need a less brute-force way of refreshing properties (names and values)
        });
        return true;
      }
    }
    return false;
  }
  
  void PropertiesMenu::selectedItemChanged(IMenuItem& item) {
    cAction = Action::SELECT;
  }

  bool PropertiesMenu::isSelectable(IMenuItem& item) const {
    return item.isSelectable();
  }

  void PropertiesMenu::refresh() {
    refreshProperties();
  }

  void PropertiesMenu::addProperty(std::unique_ptr<IProperty> property) {
    std::string mPropertyName = property->getPropertyName();
    std::unique_ptr<MenuItemProperty> mMenuItem = std::make_unique<MenuItemProperty>(mPropertyName, std::move(property), cIndentLevel);
    if (cFetching) {
      addItem(std::move(mMenuItem));
    } else {
      IMenuItem& mCurrentItem = getCurrentItem();
      addItemAfter(mCurrentItem, std::move(mMenuItem));
    }
  }

  void PropertiesMenu::addRemover(const std::string& name, std::function<void()> removeFunction) {
    addItem(std::make_unique<MenuItemProperty>("", std::make_unique<PropertyRemover>(PropertyData("", "", ""), cComponentEditor, name, removeFunction), cIndentLevel));
  }

  void PropertiesMenu::addSpacer(float height) {
    addItem(std::make_unique<MenuItemSpacer>(height));
  }

  void PropertiesMenu::pushIndent() {
    cIndentLevel++;
  }

  void PropertiesMenu::popIndent() {
    if (cIndentLevel > 0) {
      cIndentLevel--;
    }
  }

  void PropertiesMenu::openProperties(IComponentData& owner, const std::string& name, std::function<void(IComponentDefiner&)> propertyFetcher) {
    UIManager& mUIManager = getUIManager();
    IUIStyle& mStyle = getStyle();
    mUIManager.openUI(std::make_unique<PropertiesMenu>(mUIManager, mStyle, cComponentEditor, owner, propertyFetcher), name, LiteralColour(1.0f, 1.0f, 1.0f));
  }
  
  void PropertiesMenu::edit(std::unique_ptr<IPropertyEditor> editor) {
    cEditingProperty = std::move(editor);
  }
  
  void PropertiesMenu::edit(IEditable* editor) {
    getUIManager().edit(editor);
  }
  
  void PropertiesMenu::refreshProperties() {
    clear();
    cEditingProperty = nullptr;
    cClosingProperty = nullptr;
    cIndentLevel = 0;
    cFetching = true;
    cPropertyFetcher(cComponentEditor);
    cFetching = false;
    recalculateColumnWidths();
  }
  
  IUIStyle& PropertiesMenu::getPropertyStyle() {
    return getStyle();
  }

  IComponentData& PropertiesMenu::getOwner() {
    return cOwner;
  }

  void PropertiesMenu::setMetadata(const Metadata& metadata) {
    cMetadata = &metadata;
  }

  const Metadata& PropertiesMenu::getMetadata() const {
    return *cMetadata;
  }

  void PropertiesMenu::onClose() {
    cComponentEditor.closeMenu();
  }
}
