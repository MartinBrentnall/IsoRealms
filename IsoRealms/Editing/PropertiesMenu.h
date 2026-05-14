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

#include "IsoRealms/IResource.h"
#include "IsoRealms/PropertyMaker.h"

#include "Menu.h"
#include "MenuItemProperty.h"
#include "Property/IPropertyManager.h"

namespace IsoRealms {
  class PropertiesMenu : public Menu<MenuItemProperty>,
                         public IPropertyManager {
    public:
    PropertiesMenu(UIManager& manager, IUIStyle& style, IResourceData& owner, std::function<void(IPropertyMaker& owner)> propertyFetcher);

    /*************************************\
     * Implements Menu<MenuItemProperty> *
    \*************************************/
    float getWidth(MenuItemProperty& item, IUIStyle& style) const override;
    float getHeight(MenuItemProperty& item, IUIStyle& style) const override;
    void renderMenuItem(MenuItemProperty& item, IUIStyle& style, float y, float aspectRatio) const override;
    void renderOverlay(MenuItemProperty& item, IUIStyle& style, float y, float aspectRatio) const override;
    void updateOverlay(unsigned int milliseconds) override;
    float getSelectionHighlightLeft(MenuItemProperty& item, IUIStyle& style, float aspectRatio) const override;
    float getSelectionHighlightRight(MenuItemProperty& item, IUIStyle& style, float aspectRatio) const override;
    bool input(MenuItemProperty& item, UISignalID id, float y) override;
    bool input(MenuItemProperty& item, sf::Event& event) override;
    void selectedItemChanged() override;
    void refresh() override;

    /*******************************\
     * Implements IPropertyManager *
    \*******************************/
    void addProperty(std::unique_ptr<IProperty> property) override;
    void openProperties(IResourceData& owner, const std::string& name, std::function<void(IPropertyMaker&)> propertyFetcher) override;
    void edit(std::unique_ptr<IPropertyEditor> editor) override;
    void edit(IEditable* editor) override;
    void refreshProperties() override;
    IUIStyle& getPropertyStyle() override;

    private:
    enum class Action {
      SELECT,
      CONFIGURE,
      REMOVE
    };

    PropertyMaker cPropertyMaker;
    std::function<void(IPropertyMaker&)> cPropertyFetcher;
    
    std::unique_ptr<IPropertyEditor> cEditingProperty;
    std::unique_ptr<IPropertyEditor> cClosingProperty;
    AnimatedFloat cColumnWidthLabel;
    AnimatedFloat cColumnWidthValue;
    bool cHasConfigureColumn;
    bool cHasRemoveColumn;
    
    Action cAction;

    bool cFetching;
    
    void openSubProperties(MenuItemProperty& item);
    void recalculateColumnWidths();
  };
}
