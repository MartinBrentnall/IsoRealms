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

namespace IsoRealms {
  class IAction;
  class IActionType;
  class IAssets;
  class IBinding;
  class IBindingType;
  class IBoolean;
  class IColour;
  class IEditable;
  class IFloat;
  class IFont;
  class IInputHandler;
  class IInteger;
  class IModel;
  class IProjectOptions;
  class IResourceData;
  class IScreen;
  class IString;
  class ITexture;
  class IVertex;
  class Project;

  class IAssetRemover {
    public:
    virtual void remove(IAssetProvider<IResourceData, IActionType>*     provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IBinding>*        provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IBindingType>*    provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IBoolean>*        provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IColour>*         provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IEditable>*       provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IFloat>*          provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IFont>*           provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IInputHandler>*   provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IInteger>*        provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IModel>*          provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IScreen>*         provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IString>*         provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IProjectOptions>* provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IAssets>*         provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, ITexture>*        provider) = 0;
    virtual void remove(IAssetProvider<IResourceData, IVertex>*         provider) = 0;

    virtual void remove(IActionType*     asset) = 0;
    virtual void remove(IBinding*        asset) = 0;
    virtual void remove(IBindingType*    asset) = 0;
    virtual void remove(IBoolean*        asset) = 0;
    virtual void remove(IColour*         asset) = 0;
    virtual void remove(IEditable*       asset) = 0;
    virtual void remove(IFloat*          asset) = 0;
    virtual void remove(IFont*           asset) = 0;
    virtual void remove(IInputHandler*   asset) = 0;
    virtual void remove(IInteger*        asset) = 0;
    virtual void remove(IModel*          asset) = 0;
    virtual void remove(IScreen*         asset) = 0;
    virtual void remove(IString*         asset) = 0;
    virtual void remove(IProjectOptions* asset) = 0;
    virtual void remove(IAssets*         asset) = 0;
    virtual void remove(ITexture*        asset) = 0;
    virtual void remove(IVertex*         asset) = 0;

    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IActionType>*     provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IBinding>*        provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IBindingType>*    provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IBoolean>*        provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IColour>*         provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IEditable>*       provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IFloat>*          provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IFont>*           provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IInputHandler>*   provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IInteger>*        provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IModel>*          provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IScreen>*         provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IString>*         provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IProjectOptions>* provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IAssets>*         provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, ITexture>*        provider) = 0;
    virtual bool hasReadOnlyReferences(IAssetProvider<IResourceData, IVertex>*         provider) = 0;

    virtual bool hasReadOnlyReferences(IActionType*     asset) = 0;
    virtual bool hasReadOnlyReferences(IBinding*        asset) = 0;
    virtual bool hasReadOnlyReferences(IBindingType*    asset) = 0;
    virtual bool hasReadOnlyReferences(IBoolean*        asset) = 0;
    virtual bool hasReadOnlyReferences(IColour*         asset) = 0;
    virtual bool hasReadOnlyReferences(IEditable*       asset) = 0;
    virtual bool hasReadOnlyReferences(IFloat*          asset) = 0;
    virtual bool hasReadOnlyReferences(IFont*           asset) = 0;
    virtual bool hasReadOnlyReferences(IInputHandler*   asset) = 0;
    virtual bool hasReadOnlyReferences(IInteger*        asset) = 0;
    virtual bool hasReadOnlyReferences(IModel*          asset) = 0;
    virtual bool hasReadOnlyReferences(IScreen*         asset) = 0;
    virtual bool hasReadOnlyReferences(IString*         asset) = 0;
    virtual bool hasReadOnlyReferences(IProjectOptions* asset) = 0;
    virtual bool hasReadOnlyReferences(IAssets*         asset) = 0;
    virtual bool hasReadOnlyReferences(ITexture*        asset) = 0;
    virtual bool hasReadOnlyReferences(IVertex*         asset) = 0;
    
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IActionType>*     provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IBinding>*        provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IBindingType>*    provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IBoolean>*        provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IColour>*         provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IEditable>*       provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IFloat>*          provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IFont>*           provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IInputHandler>*   provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IInteger>*        provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IModel>*          provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IScreen>*         provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IString>*         provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IProjectOptions>* provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IAssets>*         provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, ITexture>*        provider) = 0;
    virtual void overrideReadOnlyReferences(IAssetProvider<IResourceData, IVertex>*         provider) = 0;

    virtual void overrideReadOnlyReferences(IActionType*     asset) = 0;
    virtual void overrideReadOnlyReferences(IBinding*        asset) = 0;
    virtual void overrideReadOnlyReferences(IBindingType*    asset) = 0;
    virtual void overrideReadOnlyReferences(IBoolean*        asset) = 0;
    virtual void overrideReadOnlyReferences(IColour*         asset) = 0;
    virtual void overrideReadOnlyReferences(IEditable*       asset) = 0;
    virtual void overrideReadOnlyReferences(IFloat*          asset) = 0;
    virtual void overrideReadOnlyReferences(IFont*           asset) = 0;
    virtual void overrideReadOnlyReferences(IInputHandler*   asset) = 0;
    virtual void overrideReadOnlyReferences(IInteger*        asset) = 0;
    virtual void overrideReadOnlyReferences(IModel*          asset) = 0;
    virtual void overrideReadOnlyReferences(IScreen*         asset) = 0;
    virtual void overrideReadOnlyReferences(IString*         asset) = 0;
    virtual void overrideReadOnlyReferences(IProjectOptions* asset) = 0;
    virtual void overrideReadOnlyReferences(IAssets*         asset) = 0;
    virtual void overrideReadOnlyReferences(ITexture*        asset) = 0;
    virtual void overrideReadOnlyReferences(IVertex*         asset) = 0;
  };
}
