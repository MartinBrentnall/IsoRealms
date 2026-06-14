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

#include <functional>
#include <memory>
#include <string>

#include "IsoRealms/Persistence/JSONObject.h"
#include "IsoRealms/Project/Registry/IResourceProvider.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  template <typename OWNER, typename TYPE> class ResourceLiteral : public IResourceProvider<OWNER, TYPE> {
    public:

    /***********************************\
     * Implements IResourceProvider<TYPE> *
    \***********************************/
    TYPE* getResource(OWNER& owner, JSONObject object) override {
      return cResources.emplace_back(createLiteralResource(owner, object)).get();
    }

    TYPE* getResource(OWNER& owner) override {
      return cResources.emplace_back(createLiteralResource(owner)).get();
    }
    
    void releaseResource(const TYPE* resource) override {
      Utils::removeElementUnique(cResources, resource);
    }

    protected:
    TYPE* addResource(std::function<std::unique_ptr<TYPE>()> createResource) const {
      return cResources.emplace_back(createResource()).get();
    }

    private:
    mutable std::vector<std::unique_ptr<TYPE>> cResources;

    virtual std::unique_ptr<TYPE> createLiteralResource(OWNER& owner) const = 0;
    virtual std::unique_ptr<TYPE> createLiteralResource(OWNER& owner, JSONObject object) const = 0;
  };
}
