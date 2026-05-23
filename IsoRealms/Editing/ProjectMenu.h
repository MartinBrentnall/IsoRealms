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

#include <set>
#include <vector>

#include "ActionMenu.h"
#include "CategoryMenu.h"

namespace IsoRealms {
  class ProjectMenu : public ActionMenu,
                      public IPropertyManager {
    public:
    ProjectMenu(UIManager& manager, IUIStyle& style, Project& project);

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

    void renderOverlay(IMenuItem& item, IUIStyle& style, float y, float aspectRatio) const override;
    void updateOverlay(unsigned int milliseconds) override;
    bool input(IMenuItem& item, UISignalID id, float y) override;
    bool input(IMenuItem& item, sf::Event& event) override;

    private:
    Project& cProject;

    PropertyMaker cPropertyMaker;
    ModuleChooser cDefModuleChooser;                  /// Source list for optional module property (must outlive property UI).

    std::unique_ptr<IPropertyEditor> cEditingProperty;
    std::unique_ptr<IPropertyEditor> cClosingProperty;
  };
}
