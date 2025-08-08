/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <string>
#include <vector>

#include "Assets/Type/IAsset.h"

namespace IsoRealms {
  template<class TYPE> class IAssetUser;
  template<class TYPE> class IStateListener;

  class IAction;
  class IActionClient;
  class IAssetRegistry;
  class IBinding;
  class IBindingRegistry;
  class IBindingType;
  class IBoolean;
  class IColour;
  class IComponentCustomType;
  class IEditable;
  class IEditingContext;
  class IFloat;
  class IFont;
  class IInputHandler;
  class IInteger;
  class IModel;
  class IModelInstance;
  class IScreen;
  class IScreenListener;
  class IProject;
  class IResourceData;
  class IString;
  class ITexture;
  class IVertex;
  class JSONObject;

  class IAssets : public IAsset {
    public:
    virtual IAction*       getAction(      IAssetUser<IAction>*       user, JSONObject object, IActionClient& owner,                                                bool required = true) = 0;
    virtual IBinding*      getBinding(     IAssetUser<IBinding>*      user, JSONObject object, IActionClient& owner,                                                bool required = true) = 0;
    virtual IBindingType*  getBindingType( IAssetUser<IBindingType>*  user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IBoolean*      getBoolean(     IAssetUser<IBoolean>*      user, JSONObject object, IResourceData& owner, IStateListener<IBoolean*>* listener = nullptr, bool required = true) = 0;
    virtual IColour*       getColour(      IAssetUser<IColour>*       user, JSONObject object, IResourceData& owner, IStateListener<IColour*>*  listener = nullptr, bool required = true) = 0;
    virtual IEditable*     getEditable(    IAssetUser<IEditable>*     user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IFloat*        getFloat(       IAssetUser<IFloat>*        user, JSONObject object, IResourceData& owner, IStateListener<IFloat*>*   listener = nullptr, bool required = true) = 0;
    virtual IFont*         getFont(        IAssetUser<IFont>*         user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IInputHandler* getInputHandler(IAssetUser<IInputHandler>* user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IInteger*      getInteger(     IAssetUser<IInteger>*      user, JSONObject object, IResourceData& owner, IStateListener<IInteger*>* listener = nullptr, bool required = true) = 0;
    virtual IModel*        getModel(       IAssetUser<IModel>*        user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IScreen*       getScreen(      IAssetUser<IScreen>*       user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IString*       getString(      IAssetUser<IString>*       user, JSONObject object, IResourceData& owner, IStateListener<IString*>*  listener = nullptr, bool required = true) = 0;
    virtual ITexture*      getTexture(     IAssetUser<ITexture>*      user, JSONObject object, IResourceData& owner, IStateListener<ITexture*>* listener = nullptr, bool required = true) = 0;
    virtual IVertex*       getVertex(      IAssetUser<IVertex>*       user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;

    virtual IAction*       getAction(      IAssetUser<IAction>*       user, const std::string& id, IActionClient& owner) = 0;
    virtual IBinding*      getBinding(     IAssetUser<IBinding>*      user, const std::string& id, IActionClient& owner) = 0;
    virtual IBindingType*  getBindingType( IAssetUser<IBindingType>*  user, const std::string& id, IResourceData& owner) = 0;
    virtual IBoolean*      getBoolean(     IAssetUser<IBoolean>*      user, const std::string& id, IResourceData& owner, IStateListener<IBoolean*>* listener = nullptr) = 0;
    virtual IColour*       getColour(      IAssetUser<IColour>*       user, const std::string& id, IResourceData& owner, IStateListener<IColour*>*  listener = nullptr) = 0;
    virtual IEditable*     getEditable(    IAssetUser<IEditable>*     user, const std::string& id, IResourceData& owner) = 0;
    virtual IFloat*        getFloat(       IAssetUser<IFloat>*        user, const std::string& id, IResourceData& owner, IStateListener<IFloat*>*   listener = nullptr) = 0;
    virtual IFont*         getFont(        IAssetUser<IFont>*         user, const std::string& id, IResourceData& owner) = 0;
    virtual IInputHandler* getInputHandler(IAssetUser<IInputHandler>* user, const std::string& id, IResourceData& owner) = 0;
    virtual IInteger*      getInteger(     IAssetUser<IInteger>*      user, const std::string& id, IResourceData& owner, IStateListener<IInteger*>* listener = nullptr) = 0;
    virtual IModel*        getModel(       IAssetUser<IModel>*        user, const std::string& id, IResourceData& owner) = 0;
    virtual IScreen*       getScreen(      IAssetUser<IScreen>*       user, const std::string& id, IResourceData& owner) = 0;
    virtual IString*       getString(      IAssetUser<IString>*       user, const std::string& id, IResourceData& owner, IStateListener<IString*>*  listener = nullptr) = 0;
    virtual ITexture*      getTexture(     IAssetUser<ITexture>*      user, const std::string& id, IResourceData& owner, IStateListener<ITexture*>* listener = nullptr) = 0;
    virtual IVertex*       getVertex(      IAssetUser<IVertex>*       user, const std::string& id, IResourceData& owner) = 0;

    virtual void setProperty(const std::string& id, const std::string& value) = 0;
  };
}
