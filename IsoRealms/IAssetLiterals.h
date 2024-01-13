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

#include "IsoRealms/Assets/Registry/IAssetUser.h"

namespace IsoRealms {
  class I3DModelType;
  class IAction;
  class IActionType;
  class IAssets;
  class IBinding;
  class IBoolean;
  class IColour;
  class IEditable;
  class IFloat;
  class IFont;
  class IInputHandler;
  class IInteger;
  class IProjectOptions;
  class IScreen;
  class IString;
  class ITexture;
  class IVertex;

  class IAssetLiterals {
    public:
    virtual I3DModelType* createLiteral3DModel(          IAssetUser<I3DModelType>*    user) = 0;
    virtual IAction* createLiteralAction(                IAssetUser<IAction>*         user) = 0;
    virtual IActionType* createLiteralActionType(        IAssetUser<IActionType>*     user) = 0;
    virtual IAssets* createLiteralAssets(                IAssetUser<IAssets>*         user) = 0;
    virtual IBinding* createLiteralBinding(              IAssetUser<IBinding>*        user) = 0;
    virtual IBoolean* createLiteralBoolean(              IAssetUser<IBoolean>*        user, const bool value) = 0;
    virtual IColour* createLiteralColour(                IAssetUser<IColour>*         user, const float red, const float green, const float blue, const float alpha = 0.0f) = 0;
    virtual IEditable* createLiteralEditable(            IAssetUser<IEditable>*       user) = 0;
    virtual IFloat* createLiteralFloat(                  IAssetUser<IFloat>*          user, const float value = 0.0f) = 0;
    virtual IFont* createLiteralFont(                    IAssetUser<IFont>*           user) = 0;
    virtual IInputHandler* createLiteralInputHandler(    IAssetUser<IInputHandler>*   user) = 0;
    virtual IInteger* createLiteralInteger(              IAssetUser<IInteger>*        user, const int value = 0) = 0;
    virtual IProjectOptions* createLiteralProjectOptions(IAssetUser<IProjectOptions>* user) = 0;
    virtual IScreen* createLiteralScreen(                IAssetUser<IScreen>*         user) = 0;
    virtual IString* createLiteralString(                IAssetUser<IString>*         user, const std::string& value = "") = 0;
    virtual ITexture* createLiteralTexture(              IAssetUser<ITexture>*        user) = 0;
    virtual IVertex* createLiteralVertex(                IAssetUser<IVertex>*         user, const float x, const float y, const float z) = 0;
  };
}
