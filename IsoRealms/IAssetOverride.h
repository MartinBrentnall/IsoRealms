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
#include "IsoRealms/Persistence/JSONDocument.h"

#include <string>

namespace IsoRealms {
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
  class IScreen;
  class IString;
  class ITexture;
  class IVertex;

  /**
   * Asset Override can be used to override assets within a project with
   * external assets.
   */
  class IAssetOverride {
    public:
    virtual IActionType*     getActionType(    JSONObject object, IStateListener<IActionType*>*     listener) const = 0;
    virtual IAssets*         getAssets(        JSONObject object, IStateListener<IAssets*>*         listener) const = 0;
    virtual IBinding*        getBinding(       JSONObject object, IStateListener<IBinding*>*        listener) const = 0;
    virtual IBindingType*    getBindingType(   JSONObject object, IStateListener<IBindingType*>*    listener) const = 0;
    virtual IBoolean*        getBoolean(       JSONObject object, IStateListener<IBoolean*>*        listener) const = 0;
    virtual IColour*         getColour(        JSONObject object, IStateListener<IColour*>*         listener) const = 0;
    virtual IEditable*       getEditable(      JSONObject object, IStateListener<IEditable*>*       listener) const = 0;
    virtual IFloat*          getFloat(         JSONObject object, IStateListener<IFloat*>*          listener) const = 0;
    virtual IFont*           getFont(          JSONObject object, IStateListener<IFont*>*           listener) const = 0;
    virtual IInputHandler*   getInputHandler(  JSONObject object, IStateListener<IInputHandler*>*   listener) const = 0;
    virtual IInteger*        getInteger(       JSONObject object, IStateListener<IInteger*>*        listener) const = 0;
    virtual IModel*          getModel(         JSONObject object, IStateListener<IModel*>*          listener) const = 0;
    virtual IProjectOptions* getProjectOptions(JSONObject object, IStateListener<IProjectOptions*>* listener) const = 0;
    virtual IScreen*         getScreen(        JSONObject object, IStateListener<IScreen*>*         listener) const = 0;
    virtual IString*         getString(        JSONObject object, IStateListener<IString*>*         listener) const = 0;
    virtual ITexture*        getTexture(       JSONObject object, IStateListener<ITexture*>*        listener) const = 0;
    virtual IVertex*         getVertex(        JSONObject object, IStateListener<IVertex*>*         listener) const = 0;
  };
}
