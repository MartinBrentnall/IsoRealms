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

#include <stdexcept>
#include <string>

#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Project/Registry/IResourceProvider.h"

namespace IsoRealms {
  template <typename OWNER, typename TYPE> class ResourceSingleton : public IResourceProvider<OWNER, TYPE> {
    public:
    ResourceSingleton(const std::string& id, TYPE* resource) :
              cID(id),
              cResource(resource) {
    }
    
    virtual ~ResourceSingleton() {
    }

    std::string getID() const {
      return cID;
    }

    /*********************************************\
     * Implements IResourceProvider<OWNER, TYPE> *
    \*********************************************/
    TYPE* getResource(OWNER& owner) override {
      return cResource;
    }
    
    TYPE* getResource(OWNER& owner, JSONObject object) override {
      return cResource;
    }

    void releaseResource(const TYPE* resource) override {
      // Nothing to do.
    }

    bool hasConfiguration() const override {
      return false;
    }
    
    bool renderResourceProviderIcon() const override {
      return cResource->renderResourceIcon();
    }

    bool isHiddenProvider() const override {
      return false;
    }

    const Metadata& getMetadata() const override {
      throw std::runtime_error("ResourceSingleton::getPropertyMetadata: Property metadata is not available for this type.");
    }

    private:
    std::string cID;
    TYPE* cResource;
  };
}
