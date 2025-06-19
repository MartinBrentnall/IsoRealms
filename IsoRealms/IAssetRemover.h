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
    virtual void remove(IAssetProvider<IResourceData, IScreen>* provider, bool relinquish) = 0;
    virtual void remove(IAssetProvider<IResourceData, IString>* provider, bool relinquish) = 0;

    virtual void remove(IActionType*     asset, bool relinquish) = 0;
    virtual void remove(IBinding*        asset, bool relinquish) = 0;
    virtual void remove(IBindingType*    asset, bool relinquish) = 0;
    virtual void remove(IBoolean*        asset, bool relinquish) = 0;
    virtual void remove(IColour*         asset, bool relinquish) = 0;
    virtual void remove(IEditable*       asset, bool relinquish) = 0;
    virtual void remove(IFloat*          asset, bool relinquish) = 0;
    virtual void remove(IFont*           asset, bool relinquish) = 0;
    virtual void remove(IInputHandler*   asset, bool relinquish) = 0;
    virtual void remove(IInteger*        asset, bool relinquish) = 0;
    virtual void remove(IModel*          asset, bool relinquish) = 0;
    virtual void remove(IScreen*         asset, bool relinquish) = 0;
    virtual void remove(IProjectOptions* asset, bool relinquish) = 0;
    virtual void remove(IAssets*         asset, bool relinquish) = 0;
    virtual void remove(IString*         asset, bool relinquish) = 0;
    virtual void remove(ITexture*        asset, bool relinquish) = 0;
    virtual void remove(IVertex*         asset, bool relinquish) = 0;
  };
}
