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
#include "DummyAssets.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DummyAssets::DummyAssets(IResourceData& owner) {
    // Nothing to do.
  }

  IAction*         DummyAssets::getAction(        IAssetUser<IAction>*         user, JSONObject object, IActionClient& owner,                                      bool required) {return nullptr;}
  IAssets*         DummyAssets::getAssets(        IAssetUser<IAssets>*         user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IBinding*        DummyAssets::getBinding(       IAssetUser<IBinding>*        user, JSONObject object, IActionClient& owner,                                      bool required) {return nullptr;}
  IBindingType*    DummyAssets::getBindingType(   IAssetUser<IBindingType>*    user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IBoolean*        DummyAssets::getBoolean(       IAssetUser<IBoolean>*        user, JSONObject object, IResourceData& owner, IStateListener<IBoolean*>* listener, bool required) {return nullptr;}
  IColour*         DummyAssets::getColour(        IAssetUser<IColour>*         user, JSONObject object, IResourceData& owner, IStateListener<IColour*>*  listener, bool required) {return nullptr;}
  IEditable*       DummyAssets::getEditable(      IAssetUser<IEditable>*       user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IFloat*          DummyAssets::getFloat(         IAssetUser<IFloat>*          user, JSONObject object, IResourceData& owner, IStateListener<IFloat*>*   listener, bool required) {return nullptr;}
  IFont*           DummyAssets::getFont(          IAssetUser<IFont>*           user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IInputHandler*   DummyAssets::getInputHandler(  IAssetUser<IInputHandler>*   user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IInteger*        DummyAssets::getInteger(       IAssetUser<IInteger>*        user, JSONObject object, IResourceData& owner, IStateListener<IInteger*>* listener, bool required) {return nullptr;}
  IModel*          DummyAssets::getModel(         IAssetUser<IModel>*          user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IProjectOptions* DummyAssets::getProjectOptions(IAssetUser<IProjectOptions>* user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IScreen*         DummyAssets::getScreen(        IAssetUser<IScreen>*         user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IString*         DummyAssets::getString(        IAssetUser<IString>*         user, JSONObject object, IResourceData& owner, IStateListener<IString*>*  listener, bool required) {return nullptr;}
  ITexture*        DummyAssets::getTexture(       IAssetUser<ITexture>*        user, JSONObject object, IResourceData& owner, IStateListener<ITexture*>* listener, bool required) {return nullptr;}
  IVertex*         DummyAssets::getVertex(        IAssetUser<IVertex>*         user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}

  IAction*         DummyAssets::getAction(        IAssetUser<IAction>*         user, const std::string& id, IActionClient& owner)                                                {return nullptr;}
  IAssets*         DummyAssets::getAssets(        IAssetUser<IAssets>*         user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IBinding*        DummyAssets::getBinding(       IAssetUser<IBinding>*        user, const std::string& id, IActionClient& owner)                                                {return nullptr;}
  IBindingType*    DummyAssets::getBindingType(   IAssetUser<IBindingType>*    user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IBoolean*        DummyAssets::getBoolean(       IAssetUser<IBoolean>*        user, const std::string& id, IResourceData& owner, IStateListener<IBoolean*>* listener = nullptr) {return nullptr;}
  IColour*         DummyAssets::getColour(        IAssetUser<IColour>*         user, const std::string& id, IResourceData& owner, IStateListener<IColour*>*  listener = nullptr) {return nullptr;}
  IEditable*       DummyAssets::getEditable(      IAssetUser<IEditable>*       user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IFloat*          DummyAssets::getFloat(         IAssetUser<IFloat>*          user, const std::string& id, IResourceData& owner, IStateListener<IFloat*>*   listener = nullptr) {return nullptr;}
  IFont*           DummyAssets::getFont(          IAssetUser<IFont>*           user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IInputHandler*   DummyAssets::getInputHandler(  IAssetUser<IInputHandler>*   user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IInteger*        DummyAssets::getInteger(       IAssetUser<IInteger>*        user, const std::string& id, IResourceData& owner, IStateListener<IInteger*>* listener = nullptr) {return nullptr;}
  IModel*          DummyAssets::getModel(         IAssetUser<IModel>*          user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IProjectOptions* DummyAssets::getProjectOptions(IAssetUser<IProjectOptions>* user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IScreen*         DummyAssets::getScreen(        IAssetUser<IScreen>*         user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IString*         DummyAssets::getString(        IAssetUser<IString>*         user, const std::string& id, IResourceData& owner, IStateListener<IString*>*  listener = nullptr) {return nullptr;}
  ITexture*        DummyAssets::getTexture(       IAssetUser<ITexture>*        user, const std::string& id, IResourceData& owner, IStateListener<ITexture*>* listener = nullptr) {return nullptr;}
  IVertex*         DummyAssets::getVertex(        IAssetUser<IVertex>*         user, const std::string& id, IResourceData& owner)                                                {return nullptr;}

  void DummyAssets::setProperty(const std::string& id, const std::string& value) {
    // TODO: Shouldn't be here.
  }

  bool DummyAssets::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void DummyAssets::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<IProperty>> DummyAssets::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool DummyAssets::isDefaultConfiguration() const {
    return true;
  }
}
