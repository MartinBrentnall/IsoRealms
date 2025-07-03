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
#include "IsoRealms/Persistence/JSONDocument.h"
#include "IStateListener.h"

namespace IsoRealms {
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
  class IProjectOptions;
  class IResourceData;
  class IString;
  class ITexture;
  class IVertex;
  class Sequence;

  class IAssets : public IAsset {
    public:
    virtual IAction*         createLiteralAction(        IAssetUser<IAction>*         user, IActionClient& owner) = 0;
    virtual IAssets*         createLiteralAssets(        IAssetUser<IAssets>*         user, IResourceData& owner) = 0;
    virtual IBinding*        createLiteralBinding(       IAssetUser<IBinding>*        user, IResourceData& owner) = 0;
    virtual IBindingType*    createLiteralBindingType(   IAssetUser<IBindingType>*    user, IResourceData& owner) = 0;
    virtual IBoolean*        createLiteralBoolean(       IAssetUser<IBoolean>*        user, IResourceData& owner, const bool value) = 0;
    virtual IColour*         createLiteralColour(        IAssetUser<IColour>*         user, IResourceData& owner, const float red, const float green, const float blue, const float alpha = 0.0f) = 0;
    virtual IEditable*       createLiteralEditable(      IAssetUser<IEditable>*       user, IResourceData& owner) = 0;
    virtual IFloat*          createLiteralFloat(         IAssetUser<IFloat>*          user, IResourceData& owner, const float value = 0.0f) = 0;
    virtual IFont*           createLiteralFont(          IAssetUser<IFont>*           user, IResourceData& owner) = 0;
    virtual IInputHandler*   createLiteralInputHandler(  IAssetUser<IInputHandler>*   user, IResourceData& owner) = 0;
    virtual IInteger*        createLiteralInteger(       IAssetUser<IInteger>*        user, IResourceData& owner, const int value = 0) = 0;
    virtual IModel*          createLiteralModel(         IAssetUser<IModel>*          user, IResourceData& owner) = 0;
    virtual IProjectOptions* createLiteralProjectOptions(IAssetUser<IProjectOptions>* user, IResourceData& owner) = 0;
    virtual IScreen*         createLiteralScreen(        IAssetUser<IScreen>*         user, IResourceData& owner) = 0;
    virtual IString*         createLiteralString(        IAssetUser<IString>*         user, IResourceData& owner, const std::string& value = "") = 0;
    virtual ITexture*        createLiteralTexture(       IAssetUser<ITexture>*        user, IResourceData& owner) = 0;
    virtual IVertex*         createLiteralVertex(        IAssetUser<IVertex>*         user, IResourceData& owner, const float x, const float y, const float z) = 0;

    virtual IAction*         getAction(        IAssetUser<IAction>*         user, JSONObject object, IActionClient& owner,                                                bool required = true) = 0;
    virtual IAssets*         getAssets(        IAssetUser<IAssets>*         user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IBinding*        getBinding(       IAssetUser<IBinding>*        user, JSONObject object, IResourceData& owner, IBindingRegistry* locals = nullptr,            bool required = true) = 0;
    virtual IBindingType*    getBindingType(   IAssetUser<IBindingType>*    user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IBoolean*        getBoolean(       IAssetUser<IBoolean>*        user, JSONObject object, IResourceData& owner, IStateListener<IBoolean*>* listener = nullptr, bool required = true) = 0;
    virtual IColour*         getColour(        IAssetUser<IColour>*         user, JSONObject object, IResourceData& owner, IStateListener<IColour*>*  listener = nullptr, bool required = true) = 0;
    virtual IEditable*       getEditable(      IAssetUser<IEditable>*       user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IFloat*          getFloat(         IAssetUser<IFloat>*          user, JSONObject object, IResourceData& owner, IStateListener<IFloat*>*   listener = nullptr, bool required = true) = 0;
    virtual IFont*           getFont(          IAssetUser<IFont>*           user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IInputHandler*   getInputHandler(  IAssetUser<IInputHandler>*   user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IInteger*        getInteger(       IAssetUser<IInteger>*        user, JSONObject object, IResourceData& owner, IStateListener<IInteger*>* listener = nullptr, bool required = true) = 0;
    virtual IModel*          getModel(         IAssetUser<IModel>*          user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IProjectOptions* getProjectOptions(IAssetUser<IProjectOptions>* user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IScreen*         getScreen(        IAssetUser<IScreen>*         user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;
    virtual IString*         getString(        IAssetUser<IString>*         user, JSONObject object, IResourceData& owner, IStateListener<IString*>*  listener = nullptr, bool required = true) = 0;
    virtual ITexture*        getTexture(       IAssetUser<ITexture>*        user, JSONObject object, IResourceData& owner, IStateListener<ITexture*>* listener = nullptr, bool required = true) = 0;
    virtual IVertex*         getVertex(        IAssetUser<IVertex>*         user, JSONObject object, IResourceData& owner,                                                bool required = true) = 0;

    virtual IAction*         getAction(        IAssetUser<IAction>*         user, const std::string& id, IActionClient& owner) = 0;
    virtual IAssets*         getAssets(        IAssetUser<IAssets>*         user, const std::string& id, IResourceData& owner) = 0;
    virtual IBinding*        getBinding(       IAssetUser<IBinding>*        user, const std::string& id, IResourceData& owner) = 0;
    virtual IBindingType*    getBindingType(   IAssetUser<IBindingType>*    user, const std::string& id, IResourceData& owner) = 0;
    virtual IBoolean*        getBoolean(       IAssetUser<IBoolean>*        user, const std::string& id, IResourceData& owner, IStateListener<IBoolean*>* listener = nullptr) = 0;
    virtual IColour*         getColour(        IAssetUser<IColour>*         user, const std::string& id, IResourceData& owner, IStateListener<IColour*>*  listener = nullptr) = 0;
    virtual IEditable*       getEditable(      IAssetUser<IEditable>*       user, const std::string& id, IResourceData& owner) = 0;
    virtual IFloat*          getFloat(         IAssetUser<IFloat>*          user, const std::string& id, IResourceData& owner, IStateListener<IFloat*>*   listener = nullptr) = 0;
    virtual IFont*           getFont(          IAssetUser<IFont>*           user, const std::string& id, IResourceData& owner) = 0;
    virtual IInputHandler*   getInputHandler(  IAssetUser<IInputHandler>*   user, const std::string& id, IResourceData& owner) = 0;
    virtual IInteger*        getInteger(       IAssetUser<IInteger>*        user, const std::string& id, IResourceData& owner, IStateListener<IInteger*>* listener = nullptr) = 0;
    virtual IModel*          getModel(         IAssetUser<IModel>*          user, const std::string& id, IResourceData& owner) = 0;
    virtual IProjectOptions* getProjectOptions(IAssetUser<IProjectOptions>* user, const std::string& id, IResourceData& owner) = 0;
    virtual IScreen*         getScreen(        IAssetUser<IScreen>*         user, const std::string& id, IResourceData& owner) = 0;
    virtual IString*         getString(        IAssetUser<IString>*         user, const std::string& id, IResourceData& owner, IStateListener<IString*>*  listener = nullptr) = 0;
    virtual ITexture*        getTexture(       IAssetUser<ITexture>*        user, const std::string& id, IResourceData& owner, IStateListener<ITexture*>* listener = nullptr) = 0;
    virtual IVertex*         getVertex(        IAssetUser<IVertex>*         user, const std::string& id, IResourceData& owner) = 0;

    virtual void release(IAssetUser<IAction>*         user, IAction*         asset) = 0;
    virtual void release(IAssetUser<IAssets>*         user, IAssets*         asset) = 0;
    virtual void release(IAssetUser<IBinding>*        user, IBinding*        asset) = 0;
    virtual void release(IAssetUser<IBindingType>*    user, IBindingType*    asset) = 0;
    virtual void release(IAssetUser<IBoolean>*        user, IBoolean*        asset) = 0;
    virtual void release(IAssetUser<IColour>*         user, IColour*         asset) = 0;
    virtual void release(IAssetUser<IEditable>*       user, IEditable*       asset) = 0;
    virtual void release(IAssetUser<IFloat>*          user, IFloat*          asset) = 0;
    virtual void release(IAssetUser<IFont>*           user, IFont*           asset) = 0;
    virtual void release(IAssetUser<IInputHandler>*   user, IInputHandler*   asset) = 0;
    virtual void release(IAssetUser<IInteger>*        user, IInteger*        asset) = 0;
    virtual void release(IAssetUser<IModel>*          user, IModel*          asset) = 0;
    virtual void release(IAssetUser<IProjectOptions>* user, IProjectOptions* asset) = 0;
    virtual void release(IAssetUser<IScreen>*         user, IScreen*         asset) = 0;
    virtual void release(IAssetUser<IString>*         user, IString*         asset) = 0;
    virtual void release(IAssetUser<ITexture>*        user, ITexture*        asset) = 0;
    virtual void release(IAssetUser<IVertex>*         user, IVertex*         asset) = 0;

    virtual void setProperty(const std::string& id, const std::string& value) = 0;
  };
}
