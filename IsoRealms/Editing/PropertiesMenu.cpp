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

#include "IsoRealms/Project/Project.h"
#include "IsoRealms/Project/ResourceType.h"

#include "Property/IPropertyEditor.h"

namespace IsoRealms {
  PropertiesMenu::PropertiesMenu(UIManager& manager, IUIStyle& style, IResourceData& owner, std::function<void(IPropertyMaker& owner)> propertyFetcher) : Menu(manager, style),
            cPropertyMaker(owner.getProject().getApplication(), owner, *this, manager),
            cPropertyFetcher(propertyFetcher),
            cEditingProperty(nullptr),
            cClosingProperty(nullptr),
            cColumnWidthLabel(0.0f),
            cColumnWidthValue(0.0f),
            cHasConfigureColumn(false),
            cHasRemoveColumn(false),
            cAction(Action::SELECT),
            cFetching(false) {
    refreshProperties();
  }

  void PropertiesMenu::recalculateColumnWidths() {
    cColumnWidthLabel = 0.0f;
    cColumnWidthValue = 0.0f;
    cHasConfigureColumn = false;
    cHasRemoveColumn = false;
    IUIStyle& mStyle = getStyle();
    for (std::unique_ptr<MenuItemProperty>& mItem : getItems()) {
      cColumnWidthLabel = std::max(cColumnWidthLabel.value(), mItem->getWidth(mStyle));
      cColumnWidthValue = std::max(cColumnWidthValue.value(), mItem->getProperty()->getValueWidth(mStyle));
      if (mItem->getProperty()->hasConfiguration()) {
        cHasConfigureColumn = true;
      }

      if (mItem->getProperty()->isRemovable()) {
        cHasRemoveColumn = true;
      }
    }
  }

  float PropertiesMenu::getWidth(MenuItemProperty& item, IUIStyle& style) const {
    return cColumnWidthLabel.value() + cColumnWidthValue.value() + style.getFontSize() * 2.25f * (1.5f + (cHasRemoveColumn ? 1 : 0) + (cHasConfigureColumn ? 1 : 0));
  }

  float PropertiesMenu::getHeight(MenuItemProperty& item, IUIStyle& style) const {
    return style.getFontSize() * 2.0f;
  }

  void PropertiesMenu::renderMenuItem(MenuItemProperty& item, IUIStyle& style, float y, float aspectRatio) const {
    float mFontSize = style.getFontSize();
    item.render(style, y, -1.0f * aspectRatio + cColumnWidthLabel.animation() + mFontSize * 2.25f, aspectRatio);
    if (item.getProperty()->hasConfiguration()) {
      glPushMatrix();
      glTranslatef(-1.0f * aspectRatio + cColumnWidthLabel.animation() + cColumnWidthValue.animation() + mFontSize * 4.50f, y + mFontSize, 0.0f);
      glScalef(mFontSize * 0.8f, mFontSize * 0.8f, 0.0f);
      Utils::renderIconCustom();
      glPopMatrix();
    }

    if (item.getProperty()->isRemovable()) {
      glPushMatrix();
      glTranslatef(-1.0f * aspectRatio + cColumnWidthLabel.animation() + cColumnWidthValue.animation() + mFontSize * 6.75f, y + mFontSize, 0.0f);
      glScalef(mFontSize * 0.8f, mFontSize * 0.8f, 0.0f);
      Utils::renderIconNone();
      glPopMatrix();
    }
  }
  
  void PropertiesMenu::renderOverlay(MenuItemProperty& item, IUIStyle& style, float y, float aspectRatio) const {
    if (cEditingProperty != nullptr) {
      cEditingProperty->render(style, y, -1.0f * aspectRatio + cColumnWidthLabel.animation() + style.getFontSize() * 2.25f, aspectRatio);
    }
    if (cClosingProperty != nullptr) {
      cClosingProperty->render(style, y, -1.0f * aspectRatio + cColumnWidthLabel.animation() + style.getFontSize() * 2.25f, aspectRatio);
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

  float PropertiesMenu::getSelectionHighlightLeft(MenuItemProperty& item, IUIStyle& style, float aspectRatio) const {
    float mFontSize = style.getFontSize();
    return cAction == Action::SELECT    ? -1.0 * aspectRatio + cColumnWidthLabel.animation() + mFontSize * 2.25f
         : cAction == Action::CONFIGURE ? -1.0 * aspectRatio + cColumnWidthLabel.animation() + mFontSize * 2.25f + cColumnWidthValue.animation() + mFontSize * 2.25
         :                                -1.0 * aspectRatio + cColumnWidthLabel.animation() + mFontSize * 2.25f + cColumnWidthValue.animation() + mFontSize * 4.50;
  }
  
  float PropertiesMenu::getSelectionHighlightRight(MenuItemProperty& item, IUIStyle& style, float aspectRatio) const {
    return cAction == Action::SELECT ? getSelectionHighlightLeft(item, style, aspectRatio) + item.getValueWidth(style)
                                     : getSelectionHighlightLeft(item, style, aspectRatio);
  }
  
  bool PropertiesMenu::input(MenuItemProperty& item, UISignalID id, float y) {
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
  
  bool PropertiesMenu::input(MenuItemProperty& item, sf::Event& event) {
    if (cEditingProperty != nullptr) {
      if (cEditingProperty->input(event, getStyle())) {
        getUIManager().getProject().updateLater([this]() {
          cClosingProperty = std::move(cEditingProperty);
          cEditingProperty = nullptr;
          recalculateColumnWidths();
          // refreshProperties(); TODO: Doing this nulls out the closing property... need a less brute-force way of refreshing properties (names and values)
        });
      }
    }
    return false;
  }
  
  void PropertiesMenu::selectedItemChanged() {
    cAction = Action::SELECT;
  }

  void PropertiesMenu::refresh() {
    refreshProperties();
  }

  void PropertiesMenu::addProperty(std::unique_ptr<IProperty> property) {
    std::string mPropertyName = property->getPropertyName();
    std::unique_ptr<MenuItemProperty> mMenuItem = std::make_unique<MenuItemProperty>(mPropertyName, std::move(property));
    if (cFetching) {
      addItem(std::move(mMenuItem));
    } else {
      MenuItemProperty& mCurrentItem = getCurrentItem();
      addItemAfter(mCurrentItem, std::move(mMenuItem));
    }
  }

  void PropertiesMenu::openProperties(IResourceData& owner, const std::string& name, std::function<void(IPropertyMaker&)> propertyFetcher) {
    UIManager& mUIManager = getUIManager();
    IUIStyle& mStyle = getStyle();
    mUIManager.openUI(std::make_unique<PropertiesMenu>(mUIManager, mStyle, owner, [this, propertyFetcher](IPropertyMaker& owner) {
      propertyFetcher(owner);
    }), name, LiteralColour(0.75f, 0.5f, 1.0f));
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
    cFetching = true;
    cPropertyFetcher(cPropertyMaker);
    cFetching = false;
    recalculateColumnWidths();
  }
  
  IUIStyle& PropertiesMenu::getPropertyStyle() {
    return getStyle();
  }
}
