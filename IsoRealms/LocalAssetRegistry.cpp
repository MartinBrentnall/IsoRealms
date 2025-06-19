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
#include "LocalAssetRegistry.h"

namespace IsoRealms {
  LocalAssetRegistry::LocalAssetRegistry(IAssetRegistry& assetRegistry, const std::string& localPath) :
            cAssetRegistry(assetRegistry),
            cLocalPath(localPath) {
  }
    
  void LocalAssetRegistry::setLocalPath(const std::string& path) {
    cLocalPath = path;
  }

  std::string LocalAssetRegistry::getModule() {
    return cLocalPath;
  }

  void LocalAssetRegistry::add(IAssetProvider<IResourceData, IScreen>* provider, const std::string& id, const std::string& category) {cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void LocalAssetRegistry::add(IAssetProvider<IResourceData, IString>* provider, const std::string& id, const std::string& category) {cAssetRegistry.add(provider, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}

  void                      LocalAssetRegistry::add(IActionType*     asset, const std::string& id, const std::string& category) {       cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      LocalAssetRegistry::add(IAssets*         asset, const std::string& id, const std::string& category) {       cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      LocalAssetRegistry::add(IBinding*        asset, const std::string& id, const std::string& category) {       cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      LocalAssetRegistry::add(IBindingType*    asset, const std::string& id, const std::string& category) {       cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IBoolean>* LocalAssetRegistry::add(IBoolean*        asset, const std::string& id, const std::string& category) {return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IColour>*  LocalAssetRegistry::add(IColour*         asset, const std::string& id, const std::string& category) {return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      LocalAssetRegistry::add(IEditable*       asset, const std::string& id, const std::string& category) {       cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IFloat>*   LocalAssetRegistry::add(IFloat*          asset, const std::string& id, const std::string& category) {return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      LocalAssetRegistry::add(IFont*           asset, const std::string& id, const std::string& category) {       cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      LocalAssetRegistry::add(IInputHandler*   asset, const std::string& id, const std::string& category) {       cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IInteger>* LocalAssetRegistry::add(IInteger*        asset, const std::string& id, const std::string& category) {return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      LocalAssetRegistry::add(IModel*          asset, const std::string& id, const std::string& category) {       cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  void                      LocalAssetRegistry::add(IProjectOptions* asset, const std::string& id, const std::string& category) {       cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IScreen*                  LocalAssetRegistry::add(IScreen*         asset, const std::string& id, const std::string& category) {return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IString>*  LocalAssetRegistry::add(IString*         asset, const std::string& id, const std::string& category) {return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<ITexture>* LocalAssetRegistry::add(ITexture*        asset, const std::string& id, const std::string& category) {return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
  IStateNotifier<IVertex>*  LocalAssetRegistry::add(IVertex*         asset, const std::string& id, const std::string& category) {return cAssetRegistry.add(asset, id == "" ? cLocalPath : cLocalPath + "/" + id, category);}
}
