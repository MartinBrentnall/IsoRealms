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

#include <map>
#include <ranges>
#include <string>

#include "IsoRealms.h"

#include "Editor/LayoutEditor.h"
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
    Layout(IProject& project, UI& ui, IResourceData& data);
    Layout(IProject& project, UI& ui, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(ResourceAssetRegistry& assets);  
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IPropertyOwner& owner);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateEditing(unsigned int milliseconds);
    void reset();

    /**********************\
     * Implements IScreen *
    \**********************/
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties(IPropertyOwner& owner) override;
    bool isDefaultConfiguration() const override;

    /************************\
     * Implements IEditable *
    \************************/
    IEditableScreen* createEditableScreen(IsoRealms::Project* project) override;

    /*********************\
     * Editing Interface *
    \*********************/
    UI& getUI() const;
    IResourceData& getResourceData();
    LayoutComponent* createComponent(float x1, float y1, float x2, float y2, float aspectRatio);
    LayoutComponent* createComponent(JSONObject& object);
    void deleteComponent(LayoutComponent* component);
    void moveComponentBackward(LayoutComponent* component);
    void moveComponentForward(LayoutComponent* component);
    void moveComponentToBack(LayoutComponent* component);
    void moveComponentToFront(LayoutComponent* component);
    LayoutComponent* getComponent(const std::string& name);
    std::string getName(const LayoutComponent* component) const;
    void setName(LayoutComponent* component, const std::string& name);
    std::vector<std::string> getAvailableRelativeNames(LayoutComponent* component);
    void renderEditing(float aspectRatio) const;
    LayoutComponent* pickComponent(float x, float y, float aspectRatio) const;
    LayoutComponent* pickPreviousComponent(float x, float y, float aspectRatio, LayoutComponent* current) const;
    LayoutComponent* pickNextComponent(float x, float y, float aspectRatio, LayoutComponent* current) const;

    private:

    // JSON members.
    static const std::string JSON_COMPONENTS;
    static const std::string JSON_ID;

    // External interfaces.
    IResourceData& cResourceData;
    UI& cUI;

    // Definition data.
    std::vector<LayoutComponent*> cComponentsByOrder;         /// Components in order of rendering.
    std::map<std::string, LayoutComponent> cComponentsByName; /// Components mapped by name.

    // Editing data.
    std::map<IEditableScreen*, std::unique_ptr<LayoutEditor>> cEditors;
    
    int getIndex(LayoutComponent* component) const;
  };
}
