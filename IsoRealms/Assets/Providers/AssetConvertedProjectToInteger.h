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

#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/Assets/TypeConverted/PrimitiveToString.h"

namespace IsoRealms {
  class Project;

  class AssetConvertedProjectToInteger : public IAssetProvider<Project, IInteger> {
    public:
    AssetConvertedProjectToInteger(IProject* project) :
              cProject(project) {
    }

    protected:

    IInteger* getAsset(Project& project, DOMNode& node) const override {
      IAssets* mAssets = cProject->getAssets(&cDummyUser, node.getNode(TAG_PROJECT));
      return mAssets->getInteger(&cDummyUser, node.getNode(TAG_VALUE));

//      return cConvertedAssets.emplace(std::make_unique<PrimitiveToString<IFloat>>(  cProject, [this, &node](IAssetUser<IFloat>*   user) -> IFloat*   {return cProject->getFloat(  user, node.getNode(TAG_ASSET), nullptr);})).first->get();
    }

    void releaseAsset(const IInteger* asset) override {
      // TODO: Implement this.
    }

    void info() const override {
      std::cout << "Extracted integer from sub-project" << std::endl;
    }

    private:
    inline static const std::string TAG_PROJECT = "Project";
    inline static const std::string TAG_VALUE   = "Value";

    class DummyUser : public IAssetUser<IAssets>,
                      public IAssetUser<IInteger> {
      void relinquish(IAssets* asset) override {
        // TODO: Implement this.
      }

      void relinquish(IInteger* asset) override {
        // TODO: Implement this.
      }
    };

    IProject* cProject;
    mutable DummyUser cDummyUser;
    mutable std::set<std::unique_ptr<IString>> cConvertedAssets;
  };
}

