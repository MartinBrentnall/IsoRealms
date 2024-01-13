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
#include <vector>

#include "Assets/Registry/IAssetUser.h"
#include "Assets/Type/IAsset.h"
#include "IAssetLiterals.h"
#include "IStateListener.h"

namespace IsoRealms {
  class DOMNode;
  class I3DModel;
  class I3DModelType;
  class IAction;
  class IActionType;
  class IAssetRegistry;
  class IBinding;
  class IBindingRegistry;
  class IBoolean;
  class IColour;
  class IComponentCustomType;
  class IEditable;
  class IEditingContext;
  class IFloat;
  class IFont;
  class IInputHandler;
  class IInteger;
  class IScreen;
  class IScreenListener;
  class IProject;
  class IProjectOptions;
  class IString;
  class ITexture;
  class IVertex;
  class Sequence;

  class IAssets : public IAssetLiterals,
                  public IAsset {
    public:
    virtual I3DModelType*    getModelType(     IAssetUser<I3DModelType>*    user, DOMNode& node,                                                bool required = true) = 0;
    virtual IAction*         getAction(        IAssetUser<IAction>*         user, DOMNode& node, const std::string& tag, IBindingRegistry* = nullptr, const std::string& id = "") = 0;
    virtual IActionType*     getActionType(    IAssetUser<IActionType>*     user, DOMNode& node,                                                bool required = true) = 0;
    virtual IAssets*         getAssets(        IAssetUser<IAssets>*         user, DOMNode& node,                                                bool required = true) = 0;
    virtual IBinding*        getBinding(       IAssetUser<IBinding>*        user, DOMNode& node, IBindingRegistry* locals = nullptr,            bool required = true) = 0;
    virtual IBoolean*        getBoolean(       IAssetUser<IBoolean>*        user, DOMNode& node, IStateListener<IBoolean*>* listener = nullptr, bool required = true) = 0;
    virtual IColour*         getColour(        IAssetUser<IColour>*         user, DOMNode& node, IStateListener<IColour*>*  listener = nullptr, bool required = true) = 0;
    virtual IEditable*       getEditable(      IAssetUser<IEditable>*       user, DOMNode& node,                                                bool required = true) = 0;
    virtual IFloat*          getFloat(         IAssetUser<IFloat>*          user, DOMNode& node, IStateListener<IFloat*>*   listener = nullptr, bool required = true) = 0;
    virtual IFont*           getFont(          IAssetUser<IFont>*           user, DOMNode& node,                                                bool required = true) = 0;
    virtual IInputHandler*   getInputHandler(  IAssetUser<IInputHandler>*   user, DOMNode& node,                                                bool required = true) = 0;
    virtual IInteger*        getInteger(       IAssetUser<IInteger>*        user, DOMNode& node, IStateListener<IInteger*>* listener = nullptr, bool required = true) = 0;
    virtual IProjectOptions* getProjectOptions(IAssetUser<IProjectOptions>* user, DOMNode& node,                                                bool required = true) = 0;
    virtual IScreen*         getScreen(        IAssetUser<IScreen>*         user, DOMNode& node,                                                bool required = true) = 0;
    virtual IString*         getString(        IAssetUser<IString>*         user, DOMNode& node, IStateListener<IString*>*  listener = nullptr, bool required = true) = 0;
    virtual ITexture*        getTexture(       IAssetUser<ITexture>*        user, DOMNode& node, IStateListener<ITexture*>* listener = nullptr, bool required = true) = 0;
    virtual IVertex*         getVertex(        IAssetUser<IVertex>*         user, DOMNode& node,                                                bool required = true) = 0;
    
    virtual void release(IAssetUser<I3DModelType>*    user, I3DModelType*    asset) = 0;
    virtual void release(IAssetUser<IAction>*         user, IAction*         asset) = 0;
    virtual void release(IAssetUser<IActionType>*     user, IActionType*     asset) = 0;
    virtual void release(IAssetUser<IAssets>*         user, IAssets*         asset) = 0;
    virtual void release(IAssetUser<IBinding>*        user, IBinding*        asset) = 0;
    virtual void release(IAssetUser<IBoolean>*        user, IBoolean*        asset) = 0;
    virtual void release(IAssetUser<IColour>*         user, IColour*         asset) = 0;
    virtual void release(IAssetUser<IEditable>*       user, IEditable*       asset) = 0;
    virtual void release(IAssetUser<IFloat>*          user, const IFloat*          asset) = 0;
    virtual void release(IAssetUser<IFont>*           user, IFont*           asset) = 0;
    virtual void release(IAssetUser<IInputHandler>*   user, IInputHandler*   asset) = 0;
    virtual void release(IAssetUser<IInteger>*        user, IInteger*        asset) = 0;
    virtual void release(IAssetUser<IProjectOptions>* user, IProjectOptions* asset) = 0;
    virtual void release(IAssetUser<IScreen>*         user, const IScreen*         asset) = 0;
    virtual void release(IAssetUser<IString>*         user, IString*         asset) = 0;
    virtual void release(IAssetUser<ITexture>*        user, ITexture*        asset) = 0;
    virtual void release(IAssetUser<IVertex>*         user, IVertex*         asset) = 0;
  };
}
