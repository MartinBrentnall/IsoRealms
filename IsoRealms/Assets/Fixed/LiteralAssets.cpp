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
#include "LiteralAssets.h"

namespace IsoRealms {
  I3DModelType* LiteralAssets::createLiteral3DModel(          IAssetUser<I3DModelType>*    user) {return nullptr;}
  IAction* LiteralAssets::createLiteralAction(                IAssetUser<IAction>*         user) {return nullptr;}
  IActionType* LiteralAssets::createLiteralActionType(        IAssetUser<IActionType>*     user) {return nullptr;}
  IAssets* LiteralAssets::createLiteralAssets(                IAssetUser<IAssets>*         user) {return nullptr;}
  IBinding* LiteralAssets::createLiteralBinding(              IAssetUser<IBinding>*        user) {return nullptr;}
  IBoolean* LiteralAssets::createLiteralBoolean(              IAssetUser<IBoolean>*        user, const bool value) {return nullptr;}
  IColour* LiteralAssets::createLiteralColour(                IAssetUser<IColour>*         user, const float red, const float green, const float blue, const float alpha) {return nullptr;}
  IEditable* LiteralAssets::createLiteralEditable(            IAssetUser<IEditable>*       user) {return nullptr;}
  IFloat* LiteralAssets::createLiteralFloat(                  IAssetUser<IFloat>*          user, const float value) {return nullptr;}
  IFont* LiteralAssets::createLiteralFont(                    IAssetUser<IFont>*           user) {return nullptr;}
  IInputHandler* LiteralAssets::createLiteralInputHandler(    IAssetUser<IInputHandler>*   user) {return nullptr;}
  IInteger* LiteralAssets::createLiteralInteger(              IAssetUser<IInteger>*        user, const int value) {return nullptr;}
  IProjectOptions* LiteralAssets::createLiteralProjectOptions(IAssetUser<IProjectOptions>* user) {return nullptr;}
  IScreen* LiteralAssets::createLiteralScreen(                IAssetUser<IScreen>*         user) {return nullptr;}
  IString* LiteralAssets::createLiteralString(                IAssetUser<IString>*         user, const std::string& value) {return nullptr;}
  ITexture* LiteralAssets::createLiteralTexture(              IAssetUser<ITexture>*        user) {return nullptr;}
  IVertex* LiteralAssets::createLiteralVertex(                IAssetUser<IVertex>*         user, const float x, const float y, const float z) {return nullptr;}

  I3DModelType*    LiteralAssets::getModelType(     IAssetUser<I3DModelType>*    user, DOMNode& node,                                      bool required) {return nullptr;}
  IAction*         LiteralAssets::getAction(        IAssetUser<IAction>*         user, DOMNode& node, const std::string& tag, IBindingRegistry* registry, const std::string& id) {return nullptr;}
  IActionType*     LiteralAssets::getActionType(    IAssetUser<IActionType>*     user, DOMNode& node,                                      bool required) {return nullptr;}
  IAssets*         LiteralAssets::getAssets(        IAssetUser<IAssets>*         user, DOMNode& node,                                      bool required) {return nullptr;}
  IBinding*        LiteralAssets::getBinding(       IAssetUser<IBinding>*        user, DOMNode& node, IBindingRegistry* locals,            bool required) {return nullptr;}
  IBoolean*        LiteralAssets::getBoolean(       IAssetUser<IBoolean>*        user, DOMNode& node, IStateListener<IBoolean*>* listener, bool required) {return nullptr;}
  IColour*         LiteralAssets::getColour(        IAssetUser<IColour>*         user, DOMNode& node, IStateListener<IColour*>*  listener, bool required) {return nullptr;}
  IEditable*       LiteralAssets::getEditable(      IAssetUser<IEditable>*       user, DOMNode& node,                                      bool required) {return nullptr;}
  IFloat*          LiteralAssets::getFloat(         IAssetUser<IFloat>*          user, DOMNode& node, IStateListener<IFloat*>*   listener, bool required) {return nullptr;}
  IFont*           LiteralAssets::getFont(          IAssetUser<IFont>*           user, DOMNode& node,                                      bool required) {return nullptr;}
  IInputHandler*   LiteralAssets::getInputHandler(  IAssetUser<IInputHandler>*   user, DOMNode& node,                                      bool required) {return nullptr;}
  IInteger*        LiteralAssets::getInteger(       IAssetUser<IInteger>*        user, DOMNode& node, IStateListener<IInteger*>* listener, bool required) {return nullptr;}
  IProjectOptions* LiteralAssets::getProjectOptions(IAssetUser<IProjectOptions>* user, DOMNode& node,                                      bool required) {return nullptr;}
  IScreen*         LiteralAssets::getScreen(        IAssetUser<IScreen>*         user, DOMNode& node,                                      bool required) {return nullptr;}
  IString*         LiteralAssets::getString(        IAssetUser<IString>*         user, DOMNode& node, IStateListener<IString*>*  listener, bool required) {return nullptr;}
  ITexture*        LiteralAssets::getTexture(       IAssetUser<ITexture>*        user, DOMNode& node, IStateListener<ITexture*>* listener, bool required) {return nullptr;}
  IVertex*         LiteralAssets::getVertex(        IAssetUser<IVertex>*         user, DOMNode& node,                                      bool required) {return nullptr;}

  void LiteralAssets::release(IAssetUser<I3DModelType>*    user, I3DModelType*    asset) {}
  void LiteralAssets::release(IAssetUser<IAction>*         user, IAction*         asset) {}
  void LiteralAssets::release(IAssetUser<IActionType>*     user, IActionType*     asset) {}
  void LiteralAssets::release(IAssetUser<IAssets>*         user, IAssets*         asset) {}
  void LiteralAssets::release(IAssetUser<IBinding>*        user, IBinding*        asset) {}
  void LiteralAssets::release(IAssetUser<IBoolean>*        user, IBoolean*        asset) {}
  void LiteralAssets::release(IAssetUser<IColour>*         user, IColour*         asset) {}
  void LiteralAssets::release(IAssetUser<IEditable>*       user, IEditable*       asset) {}
  void LiteralAssets::release(IAssetUser<IFloat>*          user, IFloat*          asset) {}
  void LiteralAssets::release(IAssetUser<IFont>*           user, IFont*           asset) {}
  void LiteralAssets::release(IAssetUser<IInputHandler>*   user, IInputHandler*   asset) {}
  void LiteralAssets::release(IAssetUser<IInteger>*        user, IInteger*        asset) {}
  void LiteralAssets::release(IAssetUser<IProjectOptions>* user, IProjectOptions* asset) {}
  void LiteralAssets::release(IAssetUser<IScreen>*         user, IScreen*         asset) {}
  void LiteralAssets::release(IAssetUser<IString>*         user, IString*         asset) {}
  void LiteralAssets::release(IAssetUser<ITexture>*        user, ITexture*        asset) {}
  void LiteralAssets::release(IAssetUser<IVertex>*         user, IVertex*         asset) {}

  void LiteralAssets::setProperty(const std::string& id, const std::string& value) {
    // TODO: Shouldn't be here.
  }

  bool LiteralAssets::renderAssetIcon() const {
    return false;
  }
}
