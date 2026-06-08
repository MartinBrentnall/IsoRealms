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

#include "AssetLiteral.h"

namespace IsoRealms {
  class Project;

  template <typename OWNER, typename TYPE, typename DUMMY> class TextureLiteralDummy : public IAssetProvider<OWNER, TYPE> {
    public:
    TextureLiteralDummy(Project& project) :
              cDummyAsset(project) {
    }
    
    /***********************************\
     * Implements IAssetProvider<TYPE> *
    \***********************************/
    TYPE* getAsset(OWNER& owner, JSONObject object) override {
      return &cDummyAsset;
    }

    TYPE* getAsset(OWNER& owner) override {
      return &cDummyAsset;
    }
    
    void releaseAsset(const TYPE* asset) override {
      // Nothing to do.
    }

    bool hasConfiguration() const override {
      return false;
    }

    bool renderAssetProviderIcon() const override {
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
    DUMMY cDummyAsset;
  };
}
