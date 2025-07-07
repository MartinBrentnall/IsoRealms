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

#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/IProject.h"

namespace IsoRealms {
  class AssetConvertedProjectToInteger : public IAssetProvider<IResourceData, IInteger> {
    public:
    AssetConvertedProjectToInteger(IProject& project) :
              cProject(project) {
    }

    protected:

    IInteger* getAsset(IResourceData& owner, JSONObject object) override {
      IAssets* mAssets = cProject.getAssets(&cDummyUser, object.getObject(JSON_PROJECT), owner);
      return mAssets->getInteger(&cDummyUser, object.getObject(JSON_VALUE), owner);

//      return cConvertedAssets.emplace(std::make_unique<PrimitiveToString<IFloat>>(  cProject, [this, &node](IAssetUser<IFloat>*   user) -> IFloat*   {return cProject->getFloat(  user, node.getNode(JSON_VALUE), nullptr);})).first->get();
    }

    IInteger* getAsset(IResourceData& owner) override {
      return nullptr; // TODO: Implement this.
    }

    void releaseAsset(const IInteger* asset) override {
      // TODO: Implement this.
    }

    bool hasConfiguration() const override {
      return true;
    }

    bool renderAssetProviderIcon() const override {
      return false;
    }

    private:
    inline static const std::string JSON_PROJECT = "project";
    inline static const std::string JSON_VALUE   = "value";

    class DummyUser : public IAssetUser<IAssets>,
                      public IAssetUser<IInteger> {
      void relinquish(IAssets* asset) override {
        // TODO: Implement this.
      }

      void relinquish(IInteger* asset) override {
        // TODO: Implement this.
      }

      bool isReadOnly() const override {
        return false; // TODO: Implement this.
      }

      void setOwner(File* owner) override {
        // TODO: Implement this.
      }
    };

    IProject& cProject;
    mutable DummyUser cDummyUser;
    mutable std::set<std::unique_ptr<IString>> cConvertedAssets;
  };
}

