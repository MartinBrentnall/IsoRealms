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

#include <string>

#include "IAssetRegistry.h"

namespace IsoRealms {
  class LocalAssetRegistry : public IAssetRegistry {
    public:
    LocalAssetRegistry(IAssetRegistry& assetRegistry, const std::string& localPath);

    void setLocalPath(const std::string& path);
    std::string getModule();
    
    /*****************************\
     * Implements IAssetRegistry *
    \*****************************/
    void add(IAssetProvider<IResourceData, IScreen>* provider, const std::string& id, const std::string& category) override;
    void add(IAssetProvider<IResourceData, IString>* provider, const std::string& id, const std::string& category) override;

    void                      add(IActionType*     asset, const std::string& id, const std::string& category) override;
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
    IScreen*                  add(IScreen*         asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IString>*  add(IString*         asset, const std::string& id, const std::string& category) override;
    IStateNotifier<ITexture>* add(ITexture*        asset, const std::string& id, const std::string& category) override;
    IStateNotifier<IVertex>*  add(IVertex*         asset, const std::string& id, const std::string& category) override;

    private:
    IAssetRegistry& cAssetRegistry;
    std::string cLocalPath;
  };
}
