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

#include <limits>
#include <functional>
#include <string>

#include "Property.h"
#include "PropertyAsset.h"

namespace IsoRealms {
  class PropertyList : public Property {
    public:
    PropertyList(IProject& project, const std::string& name, const std::string& tooltip, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter, std::function<void()> removeFunction = nullptr);

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
      ListSelection(IProject& project, const std::vector<std::string>& options, std::function<std::string()> getter, std::function<void(const std::string& value)> setter);
      
      /*******************************\
       * Interface for PropertyAsset *
      \*******************************/
      std::string getID() const;
      bool renderAssetIcon() const;
      bool hasConfiguration() const;
      bool isDefaultConfigured() const;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties();
      IApplication& getApplication() const;
      std::vector<std::string> getAvailableProviders() const;
      bool renderProviderIcon(const std::string& id) const;
      void setID(const std::string& id);
      
      private:
      IProject& cProject;
      std::vector<std::string> cOptions;
      std::function<std::string()> cGetter;
      std::function<void(const std::string&)> cSetter;
    };
  
    ListSelection cInternalSelection;
    PropertyAsset<ListSelection> cInternalProperty;
  };
}
