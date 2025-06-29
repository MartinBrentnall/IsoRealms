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

  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IScreen>* provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void ResourceAssetRegistry::add(IAssetProvider<IResourceData, IString>* provider, const std::string& id, const std::string& category) {registerAsset(provider); cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}

  void                      ResourceAssetRegistry::add(IActionType*     asset, const std::string& id, const std::string& category) {registerAsset(asset);        cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
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
  IScreen*                  ResourceAssetRegistry::add(IScreen*         asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IString>*  ResourceAssetRegistry::add(IString*         asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<ITexture>* ResourceAssetRegistry::add(ITexture*        asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IVertex>*  ResourceAssetRegistry::add(IVertex*         asset, const std::string& id, const std::string& category) {registerAsset(asset); return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
}
