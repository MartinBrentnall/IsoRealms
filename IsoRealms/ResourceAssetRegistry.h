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
#include <string>
#include <variant>
#include <vector>

namespace IsoRealms {
  class IActionClient;
  class IAction;
  class IBinding;
  class IBindingType;
  class IBoolean;
  class IColour;
  class IEditable;
  class IFont;
  class IFloat;
  class IInputHandler;
  class IInteger;
  class IModel;
  class IResourceData;
  class IScreen;
  class IString;
  class ITexture;
  class IVertex;
  class Project;

  template <typename TYPE> struct AssetTypeOf {
    using type = TYPE;
  };

  template <typename TYPE, typename OWNER> struct AssetTypeOf<IAssetProvider<OWNER, TYPE>> {
    using type = TYPE;
  };

  class ResourceAssetRegistry {
    public:
    ResourceAssetRegistry(Project& project, const std::string& localPath);
    void setLocalPath(const std::string& path);
    std::string getModule();

    using AssetVariant = std::variant<IAssetProvider<IActionClient, IAction>*,
                                      IAssetProvider<IActionClient, IBinding>*,
                                      IAssetProvider<IResourceData, IBindingType>*,
                                      IAssetProvider<IResourceData, IBoolean>*,
                                      IAssetProvider<IResourceData, IColour>*,
                                      IAssetProvider<IResourceData, IEditable>*,
                                      IAssetProvider<IResourceData, IFont>*,
                                      IAssetProvider<IResourceData, IFloat>*,
                                      IAssetProvider<IResourceData, IInputHandler>*,
                                      IAssetProvider<IResourceData, IInteger>*,
                                      IAssetProvider<IResourceData, IModel>*,
                                      IAssetProvider<IResourceData, IScreen>*,
                                      IAssetProvider<IResourceData, IString>*,
                                      IAssetProvider<IResourceData, ITexture>*,
                                      IAssetProvider<IResourceData, IVertex>*,
                                      IAction*,
                                      IBinding*,
                                      IBindingType*,
                                      IBoolean*,
                                      IColour*,
                                      IEditable*,
                                      IFont*,
                                      IFloat*,
                                      IInputHandler*,
                                      IInteger*,
                                      IModel*,
                                      IScreen*,
                                      IString*,
                                      ITexture*,
                                      IVertex*>;

    template <typename TYPE> void registerAsset(TYPE* asset) {
      for (const AssetVariant& mAsset : cRegisteredAssets) {
        if (std::holds_alternative<TYPE*>(mAsset) && std::get<TYPE*>(mAsset) == asset) {
          return;
        }
      }
      cRegisteredAssets.emplace_back(asset);
    }
    
    void unregisterAssets(Project& project);
    bool hasReadOnlyReferences(Project& project) const;
    void overrideReadOnlyReferences(Project& project);

    template <typename OWNER, typename TYPE> IStateNotifier<TYPE>* addProvider(IAssetProvider<OWNER, TYPE>* provider, const std::string& id, const std::string& category) {
      registerAsset(provider);
      return cProject.template add<TYPE>(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);
    }

    template <typename TYPE> IStateNotifier<TYPE>* add(TYPE* asset, const std::string& id, const std::string& category) {
      registerAsset(asset);
      return cProject.template add<TYPE>(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);
    }

    private:
    Project& cProject;
    std::string cLocalPath;
    std::vector<AssetVariant> cRegisteredAssets;
  };
}
