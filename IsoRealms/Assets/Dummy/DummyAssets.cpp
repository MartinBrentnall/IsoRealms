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
#include "DummyAssets.h"

#include "IsoRealms/Editing/Property/IProperty.h"
#include "IsoRealms/Utils.h"

namespace IsoRealms {
  DummyAssets::DummyAssets(IResourceData& owner) {
    // Nothing to do.
  }

  ActionExecutor*  DummyAssets::createLiteralAction(        IAssetUser<ActionExecutor>*  user, IResourceData& owner) {return nullptr;}
  IActionType*     DummyAssets::createLiteralActionType(    IAssetUser<IActionType>*     user, IResourceData& owner) {return nullptr;}
  IAssets*         DummyAssets::createLiteralAssets(        IAssetUser<IAssets>*         user, IResourceData& owner) {return nullptr;}
  IBinding*        DummyAssets::createLiteralBinding(       IAssetUser<IBinding>*        user, IResourceData& owner) {return nullptr;}
  IBindingType*    DummyAssets::createLiteralBindingType(   IAssetUser<IBindingType>*    user, IResourceData& owner) {return nullptr;}
  IBoolean*        DummyAssets::createLiteralBoolean(       IAssetUser<IBoolean>*        user, IResourceData& owner, const bool value) {return nullptr;}
  IColour*         DummyAssets::createLiteralColour(        IAssetUser<IColour>*         user, IResourceData& owner, const float red, const float green, const float blue, const float alpha) {return nullptr;}
  IEditable*       DummyAssets::createLiteralEditable(      IAssetUser<IEditable>*       user, IResourceData& owner) {return nullptr;}
  IFloat*          DummyAssets::createLiteralFloat(         IAssetUser<IFloat>*          user, IResourceData& owner, const float value) {return nullptr;}
  IFont*           DummyAssets::createLiteralFont(          IAssetUser<IFont>*           user, IResourceData& owner) {return nullptr;}
  IInputHandler*   DummyAssets::createLiteralInputHandler(  IAssetUser<IInputHandler>*   user, IResourceData& owner) {return nullptr;}
  IInteger*        DummyAssets::createLiteralInteger(       IAssetUser<IInteger>*        user, IResourceData& owner, const int value) {return nullptr;}
  IModel*          DummyAssets::createLiteralModel(         IAssetUser<IModel>*          user, IResourceData& owner) {return nullptr;}
  IProjectOptions* DummyAssets::createLiteralProjectOptions(IAssetUser<IProjectOptions>* user, IResourceData& owner) {return nullptr;}
  IScreen*         DummyAssets::createLiteralScreen(        IAssetUser<IScreen>*         user, IResourceData& owner) {return nullptr;}
  IString*         DummyAssets::createLiteralString(        IAssetUser<IString>*         user, IResourceData& owner, const std::string& value) {return nullptr;}
  ITexture*        DummyAssets::createLiteralTexture(       IAssetUser<ITexture>*        user, IResourceData& owner) {return nullptr;}
  IVertex*         DummyAssets::createLiteralVertex(        IAssetUser<IVertex>*         user, IResourceData& owner, const float x, const float y, const float z) {return nullptr;}

  ActionExecutor*  DummyAssets::getAction(        IAssetUser<ActionExecutor>*  user, JSONObject object, IResourceData& owner, const std::string& tag, IBindingRegistry* registry) {return nullptr;}
  IActionType*     DummyAssets::getActionType(    IAssetUser<IActionType>*     user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IAssets*         DummyAssets::getAssets(        IAssetUser<IAssets>*         user, JSONObject object, IResourceData& owner,                                      bool required) {return nullptr;}
  IBinding*        DummyAssets::getBinding(       IAssetUser<IBinding>*        user, JSONObject object, IResourceData& owner, IBindingRegistry* locals,            bool required) {return nullptr;}
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

  ActionExecutor*  DummyAssets::getAction(        IAssetUser<ActionExecutor>*  user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IActionType*     DummyAssets::getActionType(    IAssetUser<IActionType>*     user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IAssets*         DummyAssets::getAssets(        IAssetUser<IAssets>*         user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
  IBinding*        DummyAssets::getBinding(       IAssetUser<IBinding>*        user, const std::string& id, IResourceData& owner)                                                {return nullptr;}
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

  void DummyAssets::release(IAssetUser<ActionExecutor>*  user, ActionExecutor*  asset) {}
  void DummyAssets::release(IAssetUser<IActionType>*     user, IActionType*     asset) {}
  void DummyAssets::release(IAssetUser<IAssets>*         user, IAssets*         asset) {}
  void DummyAssets::release(IAssetUser<IBinding>*        user, IBinding*        asset) {}
  void DummyAssets::release(IAssetUser<IBindingType>*    user, IBindingType*    asset) {}
  void DummyAssets::release(IAssetUser<IBoolean>*        user, IBoolean*        asset) {}
  void DummyAssets::release(IAssetUser<IColour>*         user, IColour*         asset) {}
  void DummyAssets::release(IAssetUser<IEditable>*       user, IEditable*       asset) {}
  void DummyAssets::release(IAssetUser<IFloat>*          user, IFloat*          asset) {}
  void DummyAssets::release(IAssetUser<IFont>*           user, IFont*           asset) {}
  void DummyAssets::release(IAssetUser<IInputHandler>*   user, IInputHandler*   asset) {}
  void DummyAssets::release(IAssetUser<IInteger>*        user, IInteger*        asset) {}
  void DummyAssets::release(IAssetUser<IModel>*          user, IModel*          asset) {}
  void DummyAssets::release(IAssetUser<IProjectOptions>* user, IProjectOptions* asset) {}
  void DummyAssets::release(IAssetUser<IScreen>*         user, IScreen*         asset) {}
  void DummyAssets::release(IAssetUser<IString>*         user, IString*         asset) {}
  void DummyAssets::release(IAssetUser<ITexture>*        user, ITexture*        asset) {}
  void DummyAssets::release(IAssetUser<IVertex>*         user, IVertex*         asset) {}

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
