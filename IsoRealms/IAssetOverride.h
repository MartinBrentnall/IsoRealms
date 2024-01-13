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

#include "IsoRealms/IStateListener.h"
#include "IsoRealms/Persistence/DOMNode.h"

#include <string>

namespace IsoRealms {
  class I3DModelType;
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
   * Asset Override can be used to override assets within a project with
   * external assets.
   */
  class IAssetOverride {
    public:
    virtual I3DModelType*    getModelType(     DOMNode& node, IStateListener<I3DModelType*>*    listener) const = 0;
    virtual IActionType*     getActionType(    DOMNode& node, IStateListener<IActionType*>*     listener) const = 0;
    virtual IAssets*         getAssets(        DOMNode& node, IStateListener<IAssets*>*         listener) const = 0;
    virtual IBinding*        getBinding(       DOMNode& node, IStateListener<IBinding*>*        listener) const = 0;
    virtual IBoolean*        getBoolean(       DOMNode& node, IStateListener<IBoolean*>*        listener) const = 0;
    virtual IColour*         getColour(        DOMNode& node, IStateListener<IColour*>*         listener) const = 0;
    virtual IEditable*       getEditable(      DOMNode& node, IStateListener<IEditable*>*       listener) const = 0;
    virtual IFloat*          getFloat(         DOMNode& node, IStateListener<IFloat*>*          listener) const = 0;
    virtual IFont*           getFont(          DOMNode& node, IStateListener<IFont*>*           listener) const = 0;
    virtual IInputHandler*   getInputHandler(  DOMNode& node, IStateListener<IInputHandler*>*   listener) const = 0;
    virtual IInteger*        getInteger(       DOMNode& node, IStateListener<IInteger*>*        listener) const = 0;
    virtual IProjectOptions* getProjectOptions(DOMNode& node, IStateListener<IProjectOptions*>* listener) const = 0;
    virtual IScreen*         getScreen(        DOMNode& node, IStateListener<IScreen*>*         listener) const = 0;
    virtual IString*         getString(        DOMNode& node, IStateListener<IString*>*         listener) const = 0;
    virtual ITexture*        getTexture(       DOMNode& node, IStateListener<ITexture*>*        listener) const = 0;
    virtual IVertex*         getVertex(        DOMNode& node, IStateListener<IVertex*>*         listener) const = 0;
  };
}
