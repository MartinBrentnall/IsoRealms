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

#include "IsoRealms/Assets/Registry/AssetClientManager.h"
#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/Assets/TypeConverted/PrimitiveToString.h"

namespace IsoRealms {
  class Project;

  class AssetConvertedString : public IAssetProvider<Project, IString> {
    public:
    AssetConvertedString(IProject* project) :
              cProject(project) {
    }

    protected:
    
    IString* getAsset(Project& project, DOMNode& node) const override {
      std::string mKey = node.getAttribute(ATTRIBUTE_KEY);
      if      (mKey == ":Float")   {return cConvertedAssets.emplace(std::make_unique<PrimitiveToString<IFloat>>(  cProject, [this, &node](IAssetUser<IFloat>*   user) -> IFloat*   {return cProject->getFloat(  user, node.getNode(TAG_ASSET), nullptr);})).first->get();}
      else if (mKey == ":Integer") {return cConvertedAssets.emplace(std::make_unique<PrimitiveToString<IInteger>>(cProject, [this, &node](IAssetUser<IInteger>* user) -> IInteger* {return cProject->getInteger(user, node.getNode(TAG_ASSET), nullptr);})).first->get();}
      return nullptr;
    }

    void releaseAsset(const IString* asset) override {
      // TODO: Implement this.
    }

    void info() const override {
      std::cout << "Assets converted to Bindings" << std::endl;
    }

    private:
    inline static const std::string TAG_ASSET = "Asset";

    inline static const std::string ATTRIBUTE_KEY = "key";

    IProject* cProject;
    mutable std::set<std::unique_ptr<IString>> cConvertedAssets;

    // TODO: Would be nice to figure this out to avoid repetition; shouldn't be too hard.
//     template <class TYPE> std::unique_ptr<IString> createConversion(std::function<TYPE*(IAssetUser<TYPE>*)> assetFunction) {
//       return std::make_unique<PrimitiveToString<TYPE>>(cAssets, [assetFunction](IAssetUser<TYPE>* user) -> TYPE* {
//         return assetFunction(user);
//       });
//      cTypeConvertedAssets[mConvertedAsset] = mConvertedAsset;
//      return mConvertedAsset;
//    }

    /***********************************\
     * Implements IAssetUser<IInteger> *
    \***********************************/
//     void relinquish(IInteger* asset) override {
//       std::cout << "TODO: AssetConvertedString::relinquish: Implement this." << std::endl;
//     }
  };
}
