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

#include <string>

#include "IPropertyOwner.h"

namespace IsoRealms {
  class IConfirmationManager;
  class IResourceData;

  class PropertyMaker : public IPropertyOwner {
    public:
    PropertyMaker(IResourceData& parent, IConfirmationManager& confirmationManager);

    /*****************************\
     * Implements IPropertyOwner *
    \*****************************/
    Project& getProject() override;
    IActionClient& getDummyActionClient() override;
    const PropertyData& getPropertyData(const std::string& key) const override;
    std::string getPropertyName(const std::string& key) const override;
    std::string getPropertyDescription(const std::string& key) const override;

    std::unique_ptr<IProperty> createPropertyNativeBoolean(const std::string& metadataKey, std::function<bool()>        getter, std::function<void(bool               value)> setter, std::function<void()> removeFunction = nullptr) override;
    std::unique_ptr<IProperty> createPropertyNativeFloat(  const std::string& metadataKey, std::function<float()>       getter, std::function<bool(float              value)> setter, std::function<void()> removeFunction = nullptr) override;
    std::unique_ptr<IProperty> createPropertyNativeInteger(const std::string& metadataKey, std::function<int()>         getter, std::function<bool(int                value)> setter, std::function<void()> removeFunction = nullptr) override;
    std::unique_ptr<IProperty> createPropertyNativeString( const std::string& metadataKey, std::function<std::string()> getter, std::function<bool(const std::string& value)> setter, std::function<void()> removeFunction = nullptr) override;

    private:
    IResourceData& cParent;
    IConfirmationManager& cConfirmationManager;
  };
}
