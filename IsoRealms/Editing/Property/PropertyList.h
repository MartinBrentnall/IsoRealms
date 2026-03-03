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

#include <limits>
#include <functional>
#include <string>

#include "Property.h"
#include "PropertyTreeSelector.h"

namespace IsoRealms {
  class Project;
  
  class PropertyList : public Property {
    public:
    PropertyList(PropertyMaker& owner, IResourceAccessManager& resourceAccessManager, Project& project, const PropertyData& data, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction = nullptr);

    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override;
    float getValueWidth(IUIStyle& style) const override;
    void confirm(IPropertyManager& manager, float y) override;
    bool hasConfiguration() const override;
    void configure(IPropertyManager& manager) override;
    
    private:
    class ListSelection {
      public:
      ListSelection(Project& project, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter);
      
      /**************************************\
       * Interface for PropertyTreeSelector *
      \**************************************/
      TreeItemInfo getTreeItemInfo() const;
      bool renderAssetIcon() const;
      bool hasConfiguration() const;
      bool isDefaultConfigured() const;
      void getAssetProperties(PropertyMaker& owner);
      Application& getApplication() const;
      std::vector<TreeItemInfo> getAvailableTreeItems() const;
      bool renderProviderIcon(const std::string& id) const;
      void setID(const std::string& id);
      
      private:
      Project& cProject;
      std::vector<std::string> cOptions;
      std::function<std::string()> cGetter;
      std::function<void(const std::string&)> cSetter;
    };
  
    ListSelection cInternalSelection;
    PropertyTreeSelector<ListSelection> cInternalProperty;
  };
}
