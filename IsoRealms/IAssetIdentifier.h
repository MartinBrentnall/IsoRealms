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

namespace IsoRealms {
  
  // Forward declaration of asset type classes.
  class JSONObject;
  class I3DModel;
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
  class IScreen;
  class IProjectOptions;
  class IString;
  class ITexture;
  class IVertex;

  /**
   * Interface via which ID's of assets can be queried.
   */
  class IAssetIdentifier {
    public:
    virtual void save(JSONObject object, const IActionType*     asset) const = 0;
    virtual void save(JSONObject object, const IAssets*         asset) const = 0;
    virtual void save(JSONObject object, const I3DModelType*    asset) const = 0;
    virtual void save(JSONObject object, const IBinding*        asset) const = 0;
    virtual void save(JSONObject object, const IBoolean*        asset) const = 0;
    virtual void save(JSONObject object, const IColour*         asset) const = 0;
    virtual void save(JSONObject object, const IEditable*       asset) const = 0;
    virtual void save(JSONObject object, const IFloat*          asset) const = 0;
    virtual void save(JSONObject object, const IFont*           asset) const = 0;
    virtual void save(JSONObject object, const IInputHandler*   asset) const = 0;
    virtual void save(JSONObject object, const IInteger*        asset) const = 0;
    virtual void save(JSONObject object, const IScreen*         asset) const = 0;
    virtual void save(JSONObject object, const IProjectOptions* asset) const = 0;
    virtual void save(JSONObject object, const IString*         asset) const = 0;
    virtual void save(JSONObject object, const ITexture*        asset) const = 0;
    virtual void save(JSONObject object, const IVertex*         asset) const = 0;
  };
}
