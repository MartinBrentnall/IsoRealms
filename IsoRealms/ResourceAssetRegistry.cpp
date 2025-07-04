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
#include "ResourceAssetRegistry.h"

#include "IsoRealms/Project.h"

namespace IsoRealms {
  ResourceAssetRegistry::ResourceAssetRegistry(IAssetRegistry& assetRegistry, const std::string& localPath) :
            cAssetRegistry(assetRegistry),
            cLocalPath(localPath) {
  }
    
  void ResourceAssetRegistry::setLocalPath(const std::string& path) {
    cLocalPath = path;
  }

  std::string ResourceAssetRegistry::getModule() {
    return cLocalPath;
  }

  void ResourceAssetRegistry::unregisterAssets(Project& project) {
    for (AssetVariant& mAsset : cRegisteredAssets) {
      std::visit([this, &project](auto* asset) {
        using RAW = std::decay_t<decltype(*asset)>;
        using TYPE = typename AssetTypeOf<RAW>::type;
        project.remove<TYPE>(asset);
      }, mAsset);
    }
  }

  bool ResourceAssetRegistry::hasReadOnlyReferences(Project& project) const {
    return std::ranges::any_of(cRegisteredAssets, [&project](const AssetVariant& asset) {
      return std::visit([&project](auto* asset) {
        using RAW = std::decay_t<decltype(*asset)>;
        using TYPE = typename AssetTypeOf<RAW>::type;
        return project.hasReadOnlyReferences<TYPE>(asset);
      }, asset);
    });
  }

  void ResourceAssetRegistry::overrideReadOnlyReferences(Project& project) {
    std::ranges::for_each(cRegisteredAssets, [&project](const AssetVariant& asset) {
      std::visit([&project](auto* asset) {
        using RAW = std::decay_t<decltype(*asset)>;
        using TYPE = typename AssetTypeOf<RAW>::type;
        project.overrideReadOnlyReferences<TYPE>(asset);
      }, asset);
    });
  }

  void ResourceAssetRegistry::add(IAssetProvider<IActionClient, IAction>*         provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IAssets>*         provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IBinding>*        provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IBindingType>*    provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IBoolean>*        provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IColour>*         provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IEditable>*       provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IFloat>*          provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IFont>*           provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IInputHandler>*   provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IInteger>*        provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IModel>*          provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IProjectOptions>* provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IScreen>*         provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IString>*         provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, ITexture>*        provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IVertex>*         provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}

  void                      ResourceAssetRegistry::add(IAction*         asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      ResourceAssetRegistry::add(IAssets*         asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      ResourceAssetRegistry::add(IBinding*        asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      ResourceAssetRegistry::add(IBindingType*    asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IBoolean>* ResourceAssetRegistry::add(IBoolean*        asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IColour>*  ResourceAssetRegistry::add(IColour*         asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      ResourceAssetRegistry::add(IEditable*       asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IFloat>*   ResourceAssetRegistry::add(IFloat*          asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      ResourceAssetRegistry::add(IFont*           asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      ResourceAssetRegistry::add(IInputHandler*   asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IInteger>* ResourceAssetRegistry::add(IInteger*        asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      ResourceAssetRegistry::add(IModel*          asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      ResourceAssetRegistry::add(IProjectOptions* asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      ResourceAssetRegistry::add(IScreen*         asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IString>*  ResourceAssetRegistry::add(IString*         asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<ITexture>* ResourceAssetRegistry::add(ITexture*        asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IVertex>*  ResourceAssetRegistry::add(IVertex*         asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
}
