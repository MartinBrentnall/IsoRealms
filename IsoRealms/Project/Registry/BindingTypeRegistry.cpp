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
#include "BindingTypeRegistry.h"

namespace IsoRealms {
  BindingTypeRegistry::BindingTypeRegistry() :
            ResourceClientManager(&cDummy, "Any") {
    add(&cActions,       "Action",        "Actions");
    add(&cBooleans,      "Boolean",       "Booleans");
    add(&cColours,       "Colour",        "Colours");
    add(&cFloats,        "Float",         "Floats");
    add(&cFonts,         "Font",          "Fonts");
    add(&cInputHandlers, "Input Handler", "Input Handlers");
    add(&cIntegers,      "Integer",       "Integers");
    add(&cScreens,       "Screen",        "Screens");
    add(&cStrings,       "String",        "Strings");
    add(&cVertices,      "Vertex",        "Vertices");
  }

  BindingTypeRegistry::Literal::Literal(const std::string& type, const std::string& rootFolder) :
            cType(type),
            cRootFolder(rootFolder) {
  }

  std::string BindingTypeRegistry::Literal::getBindingTypeID() const {
    return cType;
  }

  std::string BindingTypeRegistry::Literal::getBindingTypeRootFolder() const {
    return cRootFolder;
  }

  std::string BindingTypeRegistry::Dummy::getBindingTypeID() const {
    return "";
  }

  std::string BindingTypeRegistry::Dummy::getBindingTypeRootFolder() const {
    return "";
  }
}
