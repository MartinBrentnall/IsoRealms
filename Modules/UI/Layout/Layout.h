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

#include <map>
#include <string>

#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "LayoutComponent.h"

namespace IsoRealms::UI {
  class UI;
  class LayoutEditor;

  class Layout final : public IScreen,
                       public IEditable {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    Layout(IProject* project, UI* ui);
    Layout(IProject* project, UI* ui, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);  
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;

    /************************\
     * Implements IEditable *
    \************************/
    IEditableScreen* createEditableScreen(IsoRealms::Project* project) override;

    /*********************\
     * Editing Interface *
    \*********************/
    LayoutComponent* getComponent(const std::string& name);
    std::string getName(LayoutComponent* component);
    void setName(LayoutComponent* component, const std::string& name);
    std::vector<std::string> getAvailableRelativeNames(LayoutComponent* component);
    void renderEditing(float aspectRatio) const;
    LayoutComponent* pickComponent(float x, float y, float aspectRatio) const;

    private:

    // DOM strings.
    static const std::string TAG_COMPONENT;

    static const std::string ATTRIBUTE_NAME;
    
    // Definition data.
    std::vector<LayoutComponent*> cComponentsByOrder;         /// Components in order of rendering.
    std::map<std::string, LayoutComponent> cComponentsByName; /// Components mapped by name.

    // Editing data.
// TODO    std::map<IEditableScreen*, std::unique_ptr<LayoutEditor>> cEditors;
  };
}
