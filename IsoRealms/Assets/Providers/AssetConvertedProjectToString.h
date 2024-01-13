
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

  class AssetConvertedProjectToString : public IAssetProvider<Project, IString> {
    public:
    AssetConvertedProjectToString(IProject* project) :
              cProject(project) {
    }

    protected:

    IString* getAsset(Project& project, DOMNode& node) const override {
      IAssets* mAssets = cProject->getAssets(&cDummyUser, node.getNode(TAG_PROJECT));
      return mAssets->getString(&cDummyUser, node.getNode(TAG_VALUE));

//      return cConvertedAssets.emplace(std::make_unique<PrimitiveToString<IFloat>>(  cProject, [this, &node](IAssetUser<IFloat>*   user) -> IFloat*   {return cProject->getFloat(  user, node.getNode(TAG_ASSET), nullptr);})).first->get();
    }

    void releaseAsset(const IString* asset) override {
      // TODO: Implement this.
    }

    void info() const override {
      std::cout << "Extracted string from sub-project" << std::endl;
    }


    private:

    class DummyUser : public IAssetUser<IAssets>,
                      public IAssetUser<IString> {
      void relinquish(IAssets* asset) override {
        // TODO: Implement this.
      }

      void relinquish(IString* asset) override {
        // TODO: Implement this.
      }
    };

    inline static const std::string TAG_PROJECT = "Project";
    inline static const std::string TAG_VALUE   = "Value";

    IProject* cProject;
    mutable DummyUser cDummyUser;
    mutable std::set<std::unique_ptr<IString>> cConvertedAssets;
  };
}

