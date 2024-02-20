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
  class LiteralAssets : public IAssets {
    public:

    /**********************\
     * Implements IAssets *
    \**********************/
    I3DModelType* createLiteral3DModel(          IAssetUser<I3DModelType>*    user) override;
    IAction* createLiteralAction(                IAssetUser<IAction>*         user) override;
    IActionType* createLiteralActionType(        IAssetUser<IActionType>*     user) override;
    IAssets* createLiteralAssets(                IAssetUser<IAssets>*         user) override;
    IBinding* createLiteralBinding(              IAssetUser<IBinding>*        user) override;
    IBoolean* createLiteralBoolean(              IAssetUser<IBoolean>*        user, const bool value) override;
    IColour* createLiteralColour(                IAssetUser<IColour>*         user, const float red, const float green, const float blue, const float alpha = 0.0f) override;
    IEditable* createLiteralEditable(            IAssetUser<IEditable>*       user) override;
    IFloat* createLiteralFloat(                  IAssetUser<IFloat>*          user, const float value = 0.0f) override;
    IFont* createLiteralFont(                    IAssetUser<IFont>*           user) override;
    IInputHandler* createLiteralInputHandler(    IAssetUser<IInputHandler>*   user) override;
    IInteger* createLiteralInteger(              IAssetUser<IInteger>*        user, const int value = 0) override;
    IProjectOptions* createLiteralProjectOptions(IAssetUser<IProjectOptions>* user) override;
    IScreen* createLiteralScreen(                IAssetUser<IScreen>*         user) override;
    IString* createLiteralString(                IAssetUser<IString>*         user, const std::string& value = "") override;
    ITexture* createLiteralTexture(              IAssetUser<ITexture>*        user) override;
    IVertex* createLiteralVertex(                IAssetUser<IVertex>*         user, const float x, const float y, const float z) override;

    I3DModelType*    getModelType(     IAssetUser<I3DModelType>*    user, DOMNode& node,                                                bool required = true) override;
    IAction*         getAction(        IAssetUser<IAction>*         user, DOMNode& node, const std::string& tag, IBindingRegistry* = nullptr, const std::string& id = "") override;
    IActionType*     getActionType(    IAssetUser<IActionType>*     user, DOMNode& node,                                                bool required = true) override;
    IAssets*         getAssets(        IAssetUser<IAssets>*         user, DOMNode& node,                                                bool required = true) override;
    IBinding*        getBinding(       IAssetUser<IBinding>*        user, DOMNode& node, IBindingRegistry* locals = nullptr,            bool required = true) override;
    IBoolean*        getBoolean(       IAssetUser<IBoolean>*        user, DOMNode& node, IStateListener<IBoolean*>* listener = nullptr, bool required = true) override;
    IColour*         getColour(        IAssetUser<IColour>*         user, DOMNode& node, IStateListener<IColour*>*  listener = nullptr, bool required = true) override;
    IEditable*       getEditable(      IAssetUser<IEditable>*       user, DOMNode& node,                                                bool required = true) override;
    IFloat*          getFloat(         IAssetUser<IFloat>*          user, DOMNode& node, IStateListener<IFloat*>*   listener = nullptr, bool required = true) override;
    IFont*           getFont(          IAssetUser<IFont>*           user, DOMNode& node,                                                bool required = true) override;
    IInputHandler*   getInputHandler(  IAssetUser<IInputHandler>*   user, DOMNode& node,                                                bool required = true) override;
    IInteger*        getInteger(       IAssetUser<IInteger>*        user, DOMNode& node, IStateListener<IInteger*>* listener = nullptr, bool required = true) override;
    IProjectOptions* getProjectOptions(IAssetUser<IProjectOptions>* user, DOMNode& node,                                                bool required = true) override;
    IScreen*         getScreen(        IAssetUser<IScreen>*         user, DOMNode& node,                                                bool required = true) override;
    IString*         getString(        IAssetUser<IString>*         user, DOMNode& node, IStateListener<IString*>*  listener = nullptr, bool required = true) override;
    ITexture*        getTexture(       IAssetUser<ITexture>*        user, DOMNode& node, IStateListener<ITexture*>* listener = nullptr, bool required = true) override;
    IVertex*         getVertex(        IAssetUser<IVertex>*         user, DOMNode& node,                                                bool required = true) override;

    void release(IAssetUser<I3DModelType>*    user, I3DModelType*    asset) override;
    void release(IAssetUser<IAction>*         user, IAction*         asset) override;
    void release(IAssetUser<IActionType>*     user, IActionType*     asset) override;
    void release(IAssetUser<IAssets>*         user, IAssets*         asset) override;
    void release(IAssetUser<IBinding>*        user, IBinding*        asset) override;
    void release(IAssetUser<IBoolean>*        user, IBoolean*        asset) override;
    void release(IAssetUser<IColour>*         user, IColour*         asset) override;
    void release(IAssetUser<IEditable>*       user, IEditable*       asset) override;
    void release(IAssetUser<IFloat>*          user, const IFloat*          asset) override;
    void release(IAssetUser<IFont>*           user, IFont*           asset) override;
    void release(IAssetUser<IInputHandler>*   user, IInputHandler*   asset) override;
    void release(IAssetUser<IInteger>*        user, IInteger*        asset) override;
    void release(IAssetUser<IProjectOptions>* user, IProjectOptions* asset) override;
    void release(IAssetUser<IScreen>*         user, const IScreen*         asset) override;
    void release(IAssetUser<IString>*         user, IString*         asset) override;
    void release(IAssetUser<ITexture>*        user, ITexture*        asset) override;
    void release(IAssetUser<IVertex>*         user, IVertex*         asset) override;

    bool renderAssetIcon() const override;

    void setProperty(const std::string& id, const std::string& value) override;
  };
}

