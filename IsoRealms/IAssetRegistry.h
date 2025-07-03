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

#include <functional>
#include <string>

#include "IsoRealms/Assets/Registry/IAssetProvider.h"
#include "IsoRealms/Assets/Type/IStateNotifier.h"

namespace IsoRealms {
  class IAction;
  class IActionClient;
  class IAssets;
  class IBinding;
  class IBindingType;
  class IBoolean;
  class IColour;
  class IDialogGenerator;
  class IEditable;
  class IFloat;
  class IFont;
  class IInputHandler;
  class IInteger;
  class IModelInstance;
  class IModel;
  class IResourceData;
  class IScreen;
  class IProjectOptions;
  template <class ASSET_TYPE> class IAssetUser;
  class IString;
  class ITexture;
  class IVertex;
  class Project;

  class IAssetRegistry {
    public:
    virtual void add(IAssetProvider<IActionClient, IAction>*         provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IBinding>*        provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IBindingType>*    provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IBoolean>*        provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IColour>*         provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IEditable>*       provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IFloat>*          provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IFont>*           provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IInputHandler>*   provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IInteger>*        provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IModel>*          provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IProjectOptions>* provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IAssets>*         provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IScreen>*         provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IString>*         provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, ITexture>*        provider, const std::string& id, const std::string& category) = 0;
    virtual void add(IAssetProvider<IResourceData, IVertex>*         provider, const std::string& id, const std::string& category) = 0;

    virtual void                      add(IAction*         asset, const std::string& id, const std::string& category) = 0;
    virtual void                      add(IAssets*         asset, const std::string& id, const std::string& category) = 0;
    virtual void                      add(IBinding*        asset, const std::string& id, const std::string& category) = 0;
    virtual void                      add(IBindingType*    asset, const std::string& id, const std::string& category) = 0;
    virtual IStateNotifier<IBoolean>* add(IBoolean*        asset, const std::string& id, const std::string& category) = 0;
    virtual IStateNotifier<IColour>*  add(IColour*         asset, const std::string& id, const std::string& category) = 0;
    virtual void                      add(IEditable*       asset, const std::string& id, const std::string& category) = 0;
    virtual IStateNotifier<IFloat>*   add(IFloat*          asset, const std::string& id, const std::string& category) = 0;
    virtual void                      add(IFont*           asset, const std::string& id, const std::string& category) = 0;
    virtual void                      add(IInputHandler*   asset, const std::string& id, const std::string& category) = 0;
    virtual IStateNotifier<IInteger>* add(IInteger*        asset, const std::string& id, const std::string& category) = 0;
    virtual void                      add(IModel*          asset, const std::string& id, const std::string& category) = 0;
    virtual void                      add(IProjectOptions* asset, const std::string& id, const std::string& category) = 0;
    virtual void                      add(IScreen*         asset, const std::string& id, const std::string& category) = 0;
    virtual IStateNotifier<IString>*  add(IString*         asset, const std::string& id, const std::string& category) = 0;
    virtual IStateNotifier<ITexture>* add(ITexture*        asset, const std::string& id, const std::string& category) = 0;
    virtual IStateNotifier<IVertex>*  add(IVertex*         asset, const std::string& id, const std::string& category) = 0;
  };
}
