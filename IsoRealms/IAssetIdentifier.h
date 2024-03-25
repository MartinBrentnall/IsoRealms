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
  class DOMNodeWriter;
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
    virtual void save(DOMNodeWriter* node, IActionType*     asset) const = 0;
    virtual void save(DOMNodeWriter* node, IAssets*         asset) const = 0;
    virtual void save(DOMNodeWriter* node, I3DModelType*    asset) const = 0;
    virtual void save(DOMNodeWriter* node, IBinding*        asset) const = 0;
    virtual void save(DOMNodeWriter* node, IBoolean*        asset) const = 0;
    virtual void save(DOMNodeWriter* node, IColour*         asset) const = 0;
    virtual void save(DOMNodeWriter* node, IEditable*       asset) const = 0;
    virtual void save(DOMNodeWriter* node, IFloat*          asset) const = 0;
    virtual void save(DOMNodeWriter* node, IFont*           asset) const = 0;
    virtual void save(DOMNodeWriter* node, IInputHandler*   asset) const = 0;
    virtual void save(DOMNodeWriter* node, IInteger*        asset) const = 0;
    virtual void save(DOMNodeWriter* node, IScreen*         asset) const = 0;
    virtual void save(DOMNodeWriter* node, IProjectOptions* asset) const = 0;
    virtual void save(DOMNodeWriter* node, IString*         asset) const = 0;
    virtual void save(DOMNodeWriter* node, ITexture*        asset) const = 0;
    virtual void save(DOMNodeWriter* node, IVertex*         asset) const = 0;
  };
}
