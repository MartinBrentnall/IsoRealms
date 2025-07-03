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

#include <algorithm>
#include <string>
#include <variant>

#include "IAssetRegistry.h"

namespace IsoRealms {
  class IAssetRemover;

  class ResourceAssetRegistry : public IAssetRegistry {
    public:
    ResourceAssetRegistry(IAssetRegistry& assetRegistry, const std::string& localPath);

    void setLocalPath(const std::string& path);
    std::string getModule();
    
    using AssetVariant = std::variant<IAssetProvider<IActionClient, IAction>*,
                                      IAssetProvider<IResourceData, IBinding>*,
                                      IAssetProvider<IResourceData, IBindingType>*,
                                      IAssetProvider<IResourceData, IBoolean>*,
                                      IAssetProvider<IResourceData, IColour>*,
                                      IAssetProvider<IResourceData, IEditable>*,
                                      IAssetProvider<IResourceData, IFont>*,
                                      IAssetProvider<IResourceData, IFloat>*,
                                      IAssetProvider<IResourceData, IInputHandler>*,
                                      IAssetProvider<IResourceData, IInteger>*,
                                      IAssetProvider<IResourceData, IModel>*,
                                      IAssetProvider<IResourceData, IProjectOptions>*,
                                      IAssetProvider<IResourceData, IAssets>*,
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
                                      IProjectOptions*,
                                      IAssets*,
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
    
    void unregisterAssets(IAssetRemover& remover);
    bool hasReadOnlyReferences(IAssetRemover& remover) const;
    void overrideReadOnlyReferences(IAssetRemover& remover);

    /*****************************\
     * Implements IAssetRegistry *
    \*****************************/
    void add(IAssetProvider<IActionClient, IAction>*         provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IBinding>*        provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IBindingType>*    provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IBoolean>*        provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IColour>*         provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IEditable>*       provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IFloat>*          provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IFont>*           provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IInputHandler>*   provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IInteger>*        provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IModel>*          provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IProjectOptions>* provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IAssets>*         provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IScreen>*         provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IString>*         provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, ITexture>*        provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IVertex>*         provider, const std::string& id, const std::string& category) override;

    void                      add(IAction*         asset, const std::string& id, const std::string& category) override;
    void                      add(IBinding*        asset, const std::string& id, const std::string& category) override;
    void                      add(IBindingType*    asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IBoolean>* add(IBoolean*        asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IColour>*  add(IColour*         asset, const std::string& id, const std::string& category) override;
    void                      add(IEditable*       asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IFloat>*   add(IFloat*          asset, const std::string& id, const std::string& category) override;
    void                      add(IFont*           asset, const std::string& id, const std::string& category) override;
    void                      add(IInputHandler*   asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IInteger>* add(IInteger*        asset, const std::string& id, const std::string& category) override;
    void                      add(IModel*          asset, const std::string& id, const std::string& category) override;
    void                      add(IProjectOptions* asset, const std::string& id, const std::string& category) override;
    void                      add(IAssets*         asset, const std::string& id, const std::string& category) override;
    void                      add(IScreen*         asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IString>*  add(IString*         asset, const std::string& id, const std::string& category) override;
    IStateNotifier<ITexture>* add(ITexture*        asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IVertex>*  add(IVertex*         asset, const std::string& id, const std::string& category) override;

    private:
    IAssetRegistry& cAssetRegistry;
    std::string cLocalPath;    
    std::vector<AssetVariant> cRegisteredAssets;
  };
}
