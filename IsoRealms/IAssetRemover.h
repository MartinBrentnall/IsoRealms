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

#include "Assets/Registry/IAssetUser.h"

namespace IsoRealms {
  class I3DModelType;
  class IAction;
  class IActionType;
  class IAssets;
  class IBinding;
  class IBoolean;
  class IColour;
  class IEditable;
  class IFloat;
  class IFont;
  class IInputHandler;
  class IInteger;
  class IProjectOptions;
  class IScreen;
  class IString;
  class ITexture;
  class IVertex;

  class IAssetRemover {
    public:
    virtual void remove(I3DModelType*    asset) = 0;
    virtual void remove(IActionType*     asset) = 0;
    virtual void remove(IBinding*        asset) = 0;
    virtual void remove(IBoolean*        asset) = 0;
    virtual void remove(IColour*         asset) = 0;
    virtual void remove(IEditable*       asset) = 0;
    virtual void remove(IFloat*          asset) = 0;
    virtual void remove(IFont*           asset) = 0;
    virtual void remove(IInputHandler*   asset) = 0;
    virtual void remove(IInteger*        asset) = 0;
    virtual void remove(IScreen*         asset) = 0;
    virtual void remove(IProjectOptions* asset) = 0;
    virtual void remove(IAssets*         asset) = 0;
    virtual void remove(IString*         asset) = 0;
    virtual void remove(ITexture*        asset) = 0;
    virtual void remove(IVertex*         asset) = 0;
  };
}
