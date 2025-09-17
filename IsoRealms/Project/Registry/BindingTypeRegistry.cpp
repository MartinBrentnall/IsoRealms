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
            AssetClientManager(&cDummy, "Any"),
            cActions(":Action"),
            cBooleans(":Boolean"),
            cColours(":Colour"),
            cFloats(":Float"),
            cFonts(":Font"),
            cInputHandlers(":InputHandler"),
            cIntegers(":Integer"),
            cScreens(":Screen"),
            cStrings(":String"),
            cVertices(":Vertex") {
    add(&cActions,       "Action",        "Conversions");
    add(&cBooleans,      "Boolean",       "Conversions");
    add(&cColours,       "Colour",        "Conversions");
    add(&cFloats,        "Float",         "Conversions");
    add(&cFonts,         "Font",          "Conversions");
    add(&cInputHandlers, "Input Handler", "Conversions");
    add(&cIntegers,      "Integer",       "Conversions");
    add(&cScreens,       "Screen",        "Conversions");
    add(&cStrings,       "String",        "Conversions");
    add(&cVertices,      "Vertex",        "Conversions");
  }

  BindingTypeRegistry::Literal::Literal(const std::string& type) :
            cType(type) {
  }

  std::string BindingTypeRegistry::Literal::getBindingTypeID() const {
    return cType;
  }

  bool BindingTypeRegistry::Literal::renderAssetIcon() const {
    return false;
  }

  void BindingTypeRegistry::Literal::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void BindingTypeRegistry::Literal::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool BindingTypeRegistry::Literal::isDefaultConfiguration() const {
    return true;
  }

  BindingTypeRegistry::Dummy::Dummy(IResourceData& owner) {
    // Nothing to do.
  }

  std::string BindingTypeRegistry::Dummy::getBindingTypeID() const {
    return "";
  }

  bool BindingTypeRegistry::Dummy::renderAssetIcon() const {
    return false;
  }

  void BindingTypeRegistry::Dummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void BindingTypeRegistry::Dummy::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool BindingTypeRegistry::Dummy::isDefaultConfiguration() const {
    return true;
  }
}
