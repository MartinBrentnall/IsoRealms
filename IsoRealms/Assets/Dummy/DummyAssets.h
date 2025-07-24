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
    IAction*         getAction(        IAssetUser<IAction>*         user, JSONObject object, IActionClient& owner,                                      bool required = true) override;
    IAssets*         getAssets(        IAssetUser<IAssets>*         user, JSONObject object, IResourceData& owner,                                      bool required = true) override;
    IBinding*        getBinding(       IAssetUser<IBinding>*        user, JSONObject object, IActionClient& owner,                                      bool required = true) override;
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

    IAction*         getAction(        IAssetUser<IAction>*         user, const std::string& id, IActionClient& owner) override;
    IAssets*         getAssets(        IAssetUser<IAssets>*         user, const std::string& id, IResourceData& owner) override;
    IBinding*        getBinding(       IAssetUser<IBinding>*        user, const std::string& id, IActionClient& owner) override;
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

    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(PropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    void setProperty(const std::string& id, const std::string& value) override;
  };
}

