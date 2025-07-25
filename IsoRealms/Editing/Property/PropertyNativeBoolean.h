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

#include "PropertyAsset.h"

namespace IsoRealms {
  class PropertyNativeBoolean;

  class PropertyNativeBoolean : public Property {
    public:
    PropertyNativeBoolean(PropertyMaker& owner, IConfirmationManager& confirmationManager, const PropertyData& data, std::function<bool()> getter, std::function<void(bool)> setter, IProject& project, std::function<void()> removeFunction = nullptr);

    /************************\
     * Implements IProperty *
    \************************/
    void renderValue(IUIStyle& style, float y, float x, float aspectRatio) const override;
    float getValueWidth(IUIStyle& style) const override;
    void confirm(IPropertyManager& manager, float y) override;
    bool hasConfiguration() const override;
    void configure(IPropertyManager& manager) override;

    private:
    class BooleanSelection {
      public:
      BooleanSelection(std::function<void(bool)> setter, IProject& project, bool value);

      /*******************************\
      * Interface for PropertyAsset *
      \*******************************/
      std::string getID() const;
      bool renderAssetIcon() const;
      bool hasConfiguration() const;
      bool isDefaultConfigured() const;
      void getAssetProperties(PropertyMaker& owner);
      IApplication& getApplication() const;
      std::vector<std::string> getAvailableProviders() const;
      bool renderProviderIcon(const std::string& id) const;
      void setID(const std::string& id);

      private:
      static const std::string ID_TRUE;
      static const std::string ID_FALSE;

      IProject& cProject;
      std::function<void(bool)> cSetter;
      bool cValue;
    };

    BooleanSelection cInternalSelection;
    PropertyAsset<BooleanSelection> cInternalProperty;
  };
}
