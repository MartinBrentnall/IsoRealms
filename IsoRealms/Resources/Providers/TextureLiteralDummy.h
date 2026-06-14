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

#include "ResourceLiteral.h"

namespace IsoRealms {
  class Project;

  template <typename OWNER, typename TYPE, typename DUMMY> class TextureLiteralDummy : public IResourceProvider<OWNER, TYPE> {
    public:
    TextureLiteralDummy(Project& project) :
              cDummyResource(project) {
    }
    
    /**************************************\
     * Implements IResourceProvider<TYPE> *
    \**************************************/
    TYPE* getResource(OWNER& owner, JSONObject object) override {
      return &cDummyResource;
    }

    TYPE* getResource(OWNER& owner) override {
      return &cDummyResource;
    }
    
    void releaseResource(const TYPE* resource) override {
      // Nothing to do.
    }

    bool hasConfiguration() const override {
      return false;
    }

    bool renderResourceProviderIcon() const override {
      Utils::renderIconNone();
      return true;
    }

    bool isHiddenProvider() const override {
      return false;
    }

    const Metadata& getMetadata() const override {
      throw std::runtime_error("TextureLiteralDummy::getPropertyMetadata: Property metadata is not available for this type.");
    }

    private:
    DUMMY cDummyResource;
  };
}
