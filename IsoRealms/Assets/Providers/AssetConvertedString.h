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

#include <set>

#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/Assets/TypeConverted/PrimitiveToString.h"

namespace IsoRealms {
  class Project;

  template <class FROM> class AssetConvertedString : public IAssetProvider<Project, IString> {
    public:
    AssetConvertedString(IProject& project) :
              cProject(project) {
    }

    IString* getAsset(Project& project, JSONObject object) const override {
      return cConvertedAssets.emplace(std::make_unique<PrimitiveToString<FROM>>(cProject, object)).first->get();
    }

    IString* getAsset(Project& project) const override {
      return cConvertedAssets.emplace(std::make_unique<PrimitiveToString<FROM>>(cProject)).first->get();
    }
    
    void releaseAsset(const IString* asset) override {
      // TODO: Implement this.
    }

    bool hasConfiguration() const override {
      return true;
    }
    
    bool renderAssetProviderIcon() const override {
      return false;
    }

    private:
    IProject& cProject;
    mutable std::set<std::unique_ptr<IString>> cConvertedAssets;
  };
}
