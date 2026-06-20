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

#include "IsoRealms/IComponent.h"
#include "IsoRealms/Editing/ComponentEditor.h"

#include "Menu.h"
#include "MenuItemProperty.h"
#include "MenuItemSpacer.h"
#include "Property/IPropertyManager.h"

namespace IsoRealms {
  class PropertiesMenu : public Menu<IMenuItem>,
                         public IPropertyManager {
    public:
    PropertiesMenu(UIManager& manager, IUIStyle& style, IComponentData& owner, std::function<void(IComponentDefiner& definer)> propertyFetcher);

    /******************************\
     * Implements Menu<IMenuItem> *
    \******************************/
    float getWidth(IMenuItem& item, IUIStyle& style) const override;
    float getHeight(IMenuItem& item, IUIStyle& style) const override;
    void renderMenuItem(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const override;
    void renderOverlay(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const override;
    void updateOverlay(unsigned int milliseconds) override;
    float getSelectionHighlightLeft(IMenuItem& item, IUIStyle& style, float aspectRatio) const override;
    float getSelectionHighlightRight(IMenuItem& item, IUIStyle& style, float aspectRatio) const override;
    bool input(IMenuItem& item, UISignalID id, float y) override;
    bool input(IMenuItem& item, sf::Event& event) override;
    void selectedItemChanged(IMenuItem& item) override;
    bool isSelectable(IMenuItem& item) const override;
    void refresh() override;

    /*******************************\
     * Implements IPropertyManager *
    \*******************************/
    void addProperty(std::unique_ptr<IProperty> property) override;
    void addSpacer(float height) override;
    void openProperties(IComponentData& owner, const std::string& name, std::function<void(IComponentDefiner&)> propertyFetcher) override;
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

    ComponentEditor cComponentEditor;
    std::function<void(IComponentDefiner&)> cPropertyFetcher;
    
    std::unique_ptr<IPropertyEditor> cEditingProperty;
    std::unique_ptr<IPropertyEditor> cClosingProperty;
    AnimatedFloat cColumnWidthLabel;
    AnimatedFloat cColumnWidthValue;
    bool cHasConfigureColumn;
    bool cHasRemoveColumn;
    
    Action cAction;

    bool cFetching;
    
    void openSubProperties(IMenuItem& item);
    void recalculateColumnWidths();
  };
}
