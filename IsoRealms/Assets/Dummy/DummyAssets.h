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

#include "IsoRealms/IAssets.h"

namespace IsoRealms {

  /**
   * A dummy Assets implementation.
   */
  class DummyAssets : public IAssets {
    public:
    DummyAssets(IResourceData& owner);

    /**********************\
     * Implements IAssets *
    \**********************/
    ActionExecutor*  createLiteralAction(        IAssetUser<ActionExecutor>*  user, IResourceData& owner) override;
    IActionType*     createLiteralActionType(    IAssetUser<IActionType>*     user, IResourceData& owner) override;
    IAssets*         createLiteralAssets(        IAssetUser<IAssets>*         user, IResourceData& owner) override;
    IBinding*        createLiteralBinding(       IAssetUser<IBinding>*        user, IResourceData& owner) override;
    IBindingType*    createLiteralBindingType(   IAssetUser<IBindingType>*    user, IResourceData& owner) override;
    IBoolean*        createLiteralBoolean(       IAssetUser<IBoolean>*        user, IResourceData& owner, const bool value) override;
    IColour*         createLiteralColour(        IAssetUser<IColour>*         user, IResourceData& owner, const float red, const float green, const float blue, const float alpha = 0.0f) override;
    IEditable*       createLiteralEditable(      IAssetUser<IEditable>*       user, IResourceData& owner) override;
    IFloat*          createLiteralFloat(         IAssetUser<IFloat>*          user, IResourceData& owner, const float value = 0.0f) override;
    IFont*           createLiteralFont(          IAssetUser<IFont>*           user, IResourceData& owner) override;
    IInputHandler*   createLiteralInputHandler(  IAssetUser<IInputHandler>*   user, IResourceData& owner) override;
    IInteger*        createLiteralInteger(       IAssetUser<IInteger>*        user, IResourceData& owner, const int value = 0) override;
    IModel*          createLiteralModel(         IAssetUser<IModel>*          user, IResourceData& owner) override;
    IProjectOptions* createLiteralProjectOptions(IAssetUser<IProjectOptions>* user, IResourceData& owner) override;
    IScreen*         createLiteralScreen(        IAssetUser<IScreen>*         user, IResourceData& owner) override;
    IString*         createLiteralString(        IAssetUser<IString>*         user, IResourceData& owner, const std::string& value = "") override;
    ITexture*        createLiteralTexture(       IAssetUser<ITexture>*        user, IResourceData& owner) override;
    IVertex*         createLiteralVertex(        IAssetUser<IVertex>*         user, IResourceData& owner, const float x, const float y, const float z) override;

    ActionExecutor*  getAction(        IAssetUser<ActionExecutor>*  user, JSONObject object, IResourceData& owner, const std::string& tag, IBindingRegistry* localArgs) override;
    IActionType*     getActionType(    IAssetUser<IActionType>*     user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IAssets*         getAssets(        IAssetUser<IAssets>*         user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IBinding*        getBinding(       IAssetUser<IBinding>*        user, JSONObject object, IResourceData& owner, IBindingRegistry* locals,            bool required = true) override;
    IBindingType*    getBindingType(   IAssetUser<IBindingType>*    user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IBoolean*        getBoolean(       IAssetUser<IBoolean>*        user, JSONObject object, IResourceData& owner, IStateListener<IBoolean*>* listener, bool required = true) override;
    IColour*         getColour(        IAssetUser<IColour>*         user, JSONObject object, IResourceData& owner, IStateListener<IColour*>*  listener, bool required = true) override;
    IEditable*       getEditable(      IAssetUser<IEditable>*       user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IFloat*          getFloat(         IAssetUser<IFloat>*          user, JSONObject object, IResourceData& owner, IStateListener<IFloat*>*   listener, bool required = true) override;
    IFont*           getFont(          IAssetUser<IFont>*           user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IInputHandler*   getInputHandler(  IAssetUser<IInputHandler>*   user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IInteger*        getInteger(       IAssetUser<IInteger>*        user, JSONObject object, IResourceData& owner, IStateListener<IInteger*>* listener, bool required = true) override;
    IModel*          getModel(         IAssetUser<IModel>*          user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IProjectOptions* getProjectOptions(IAssetUser<IProjectOptions>* user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IScreen*         getScreen(        IAssetUser<IScreen>*         user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IString*         getString(        IAssetUser<IString>*         user, JSONObject object, IResourceData& owner, IStateListener<IString*>*  listener, bool required = true) override;
    ITexture*        getTexture(       IAssetUser<ITexture>*        user, JSONObject object, IResourceData& owner, IStateListener<ITexture*>* listener, bool required = true) override;
    IVertex*         getVertex(        IAssetUser<IVertex>*         user, JSONObject object, IResourceData& owner,                                      bool required = true) override;

    ActionExecutor*  getAction(        IAssetUser<ActionExecutor>*  user, const std::string& id, IResourceData& owner) override;
    IActionType*     getActionType(    IAssetUser<IActionType>*     user, const std::string& id, IResourceData& owner) override;
    IAssets*         getAssets(        IAssetUser<IAssets>*         user, const std::string& id, IResourceData& owner) override;
    IBinding*        getBinding(       IAssetUser<IBinding>*        user, const std::string& id, IResourceData& owner) override;
    IBindingType*    getBindingType(   IAssetUser<IBindingType>*    user, const std::string& id, IResourceData& owner) override;
    IBoolean*        getBoolean(       IAssetUser<IBoolean>*        user, const std::string& id, IResourceData& owner, IStateListener<IBoolean*>* listener) override;
    IColour*         getColour(        IAssetUser<IColour>*         user, const std::string& id, IResourceData& owner, IStateListener<IColour*>*  listener) override;
    IEditable*       getEditable(      IAssetUser<IEditable>*       user, const std::string& id, IResourceData& owner) override;
    IFloat*          getFloat(         IAssetUser<IFloat>*          user, const std::string& id, IResourceData& owner, IStateListener<IFloat*>*   listener) override;
    IFont*           getFont(          IAssetUser<IFont>*           user, const std::string& id, IResourceData& owner) override;
    IInputHandler*   getInputHandler(  IAssetUser<IInputHandler>*   user, const std::string& id, IResourceData& owner) override;
    IInteger*        getInteger(       IAssetUser<IInteger>*        user, const std::string& id, IResourceData& owner, IStateListener<IInteger*>* listener) override;
    IModel*          getModel(         IAssetUser<IModel>*          user, const std::string& id, IResourceData& owner) override;
    IProjectOptions* getProjectOptions(IAssetUser<IProjectOptions>* user, const std::string& id, IResourceData& owner) override;
    IScreen*         getScreen(        IAssetUser<IScreen>*         user, const std::string& id, IResourceData& owner) override;
    IString*         getString(        IAssetUser<IString>*         user, const std::string& id, IResourceData& owner, IStateListener<IString*>*  listener) override;
    ITexture*        getTexture(       IAssetUser<ITexture>*        user, const std::string& id, IResourceData& owner, IStateListener<ITexture*>* listener) override;
    IVertex*         getVertex(        IAssetUser<IVertex>*         user, const std::string& id, IResourceData& owner) override;

    void release(IAssetUser<ActionExecutor>*  user, ActionExecutor*  asset) override;
    void release(IAssetUser<IActionType>*     user, IActionType*     asset) override;
    void release(IAssetUser<IAssets>*         user, IAssets*         asset) override;
    void release(IAssetUser<IBinding>*        user, IBinding*        asset) override;
    void release(IAssetUser<IBindingType>*    user, IBindingType*    asset) override;
    void release(IAssetUser<IBoolean>*        user, IBoolean*        asset) override;
    void release(IAssetUser<IColour>*         user, IColour*         asset) override;
    void release(IAssetUser<IEditable>*       user, IEditable*       asset) override;
    void release(IAssetUser<IFloat>*          user, IFloat*          asset) override;
    void release(IAssetUser<IFont>*           user, IFont*           asset) override;
    void release(IAssetUser<IInputHandler>*   user, IInputHandler*   asset) override;
    void release(IAssetUser<IInteger>*        user, IInteger*        asset) override;
    void release(IAssetUser<IModel>*          user, IModel*          asset) override;
    void release(IAssetUser<IProjectOptions>* user, IProjectOptions* asset) override;
    void release(IAssetUser<IScreen>*         user, IScreen*         asset) override;
    void release(IAssetUser<IString>*         user, IString*         asset) override;
    void release(IAssetUser<ITexture>*        user, ITexture*        asset) override;
    void release(IAssetUser<IVertex>*         user, IVertex*         asset) override;

    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    void setProperty(const std::string& id, const std::string& value) override;
  };
}

