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

#include <functional>

#include "IsoRealms/Resources/Providers/ResourceLiteralDummy.h"
#include "IsoRealms/Resources/Type/IBindingType.h"
#include "IsoRealms/IComponentData.h"
#include "IsoRealms/Utils.h"

#include "ResourceClientManager.h"
#include "IResourceUser.h"

namespace IsoRealms {
  class BindingTypeRegistry : public ResourceClientManager<BindingTypeRegistry, IComponentData, IBindingType> {
    public:
    BindingTypeRegistry();

    private:
    class Literal : public IBindingType {
      public:
      Literal(const std::string& type, const std::string& rootFolder);

      /***********************\
      * Implements IBinding *
      \***********************/
      std::string getBindingTypeID() const override;
      std::string getBindingTypeRootFolder() const override;
      
      private:
      std::string cType;
      std::string cRootFolder;
    };

    class Dummy : public IBindingType {
      public:

      /***********************\
      * Implements IBinding *
      \***********************/
      std::string getBindingTypeID() const override;
      std::string getBindingTypeRootFolder() const override;
    };

    ResourceLiteralDummy<IComponentData, IBindingType, Dummy> cDummy;
    Literal cActions{      ":Action",       "Core Resources/Actions"};
    Literal cBooleans{     ":Boolean",      "Core Resources/Booleans"};
    Literal cColours{      ":Colour",       "Core Resources/Colours"};
    Literal cFloats{       ":Float",        "Core Resources/Floats"};
    Literal cFonts{        ":Font",         "Core Resources/Fonts"};
    Literal cInputHandlers{":InputHandler", "Core Resources/Input Handlers"};
    Literal cIntegers{     ":Integer",      "Core Resources/Integers"};
    Literal cScreens{      ":Screen",       "Core Resources/Screens"};
    Literal cStrings{      ":String",       "Core Resources/Strings"};
    Literal cVertices{     ":Vertex",       "Core Resources/Vertices"};
  };
}
