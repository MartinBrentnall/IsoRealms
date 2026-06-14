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

#include <algorithm>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "Resources/Type/IStateNotifier.h"
#include "Project/Registry/IResourceProvider.h"

namespace IsoRealms {
  template <typename TYPE> struct ResourceTypeOf {
    using type = TYPE;
  };

  template <typename TYPE, typename OWNER> struct ResourceTypeOf<IResourceProvider<OWNER, TYPE>> {
    using type = TYPE;
  };

  template <typename RESOURCE_VARIANT, typename MANAGER> class AbstractResourceRegistry {
    public:
    AbstractResourceRegistry(MANAGER& manager) :
              cManager(manager) {
    }

    void clear() {
      for (RESOURCE_VARIANT& mResource : cRegisteredResources) {
        std::visit([this](auto* resource) {
          using RAW = std::decay_t<decltype(*resource)>;
          using TYPE = typename ResourceTypeOf<RAW>::type;
          cManager.getResourceManager().template remove<TYPE>(resource);
        }, mResource);
      }
    }

    template <typename TYPE> void registerResource(TYPE* resource) {
      for (const RESOURCE_VARIANT& mResource : cRegisteredResources) {
        if (std::holds_alternative<TYPE*>(mResource) && std::get<TYPE*>(mResource) == resource) {
          return;
        }
      }
      cRegisteredResources.emplace_back(resource);
    }
    
    bool hasReadOnlyReferences() const {
      return std::ranges::any_of(cRegisteredResources, [this](const RESOURCE_VARIANT& resource) {
        return std::visit([this](auto* resource) {
          using RAW = std::decay_t<decltype(*resource)>;
          using TYPE = typename ResourceTypeOf<RAW>::type;
          return cManager.getResourceManager().template hasReadOnlyReferences<TYPE>(resource);
        }, resource);
      });
    }

    void overrideReadOnlyReferences() {
      std::ranges::for_each(cRegisteredResources, [this](const RESOURCE_VARIANT& resource) {
        std::visit([this](auto* resource) {
          using RAW = std::decay_t<decltype(*resource)>;
          using TYPE = typename ResourceTypeOf<RAW>::type;
          cManager.getResourceManager().template overrideReadOnlyReferences<TYPE>(resource);
        }, resource);
      });
    }

    template <typename OWNER, typename TYPE> IStateNotifier* publishProvider(IResourceProvider<OWNER, TYPE>* provider, const std::string& resourceID, const std::string& category) {
      if (category.empty()) {
        std::cout << "WARNING: AbstractResourceRegistry::publishProvider: Category cannot be empty." << std::endl;
      }
      registerResource(provider);
      std::string mComponentID = cManager.getComponentID();
      std::string mName = cManager.getComponentName();
      return cManager.getResourceManager().template add<TYPE>(provider, resourceID == "" ? mComponentID : mComponentID + "/" + resourceID, 
                                                                        mName      == "" ? category
                                                                      : resourceID == "" ? category + "/" + mName 
                                                                      :                    category + "/" + mName + "/" + resourceID);
    }

    template <typename TYPE> IStateNotifier* publish(TYPE* resource, const std::string& resourceID, const std::string& category) {
      if (category.empty()) {
        std::cout << "WARNING: AbstractResourceRegistry::publish: Category cannot be empty." << std::endl;
      }
      registerResource(resource);
      std::string mComponentID = cManager.getComponentID();
      std::string mName = cManager.getComponentName();
      return cManager.getResourceManager().template add<TYPE>(resource, mComponentID == "" ? resourceID
                                                                      : resourceID   == "" ? mComponentID
                                                                      :                      mComponentID + "/" + resourceID,
                                                                          resourceID == "" ? category + "/" + mName 
                                                                      :                      category + "/" + mName + "/" + resourceID);
    }

    private:
    MANAGER& cManager;
    std::vector<RESOURCE_VARIANT> cRegisteredResources;
  };
}
