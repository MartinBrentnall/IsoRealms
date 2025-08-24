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
#include "IsoRealms/PropertyMaker.h"
#include "IsoRealms/Project.h"

#include "BindingRegistry.h"

namespace IsoRealms {
  BindingRegistry::BindingRegistry(Project& project) :
            AssetClientManager(&cLiteralProvider),
            cConversionProviderAction(project),
            cConversionProviderBoolean(project),
            cConversionProviderColour(project),
            cConversionProviderFloat(project),
            cConversionProviderFont(project),
            cConversionProviderInputHandler(project),
            cConversionProviderInteger(project),
            cConversionProviderScreen(project),
            cConversionProviderString(project),
            cConversionProviderVertex(project) {

    // Support local bindings.
    add(&cLocalProviderBinding, "~", "Local");

    // Support conversions.
    add(&cConversionProviderAction,       ":Action",       "Conversions");
    add(&cConversionProviderBoolean,      ":Boolean",      "Conversions");
    add(&cConversionProviderColour,       ":Colour",       "Conversions");
    add(&cConversionProviderFloat,        ":Float",        "Conversions");
    add(&cConversionProviderFont,         ":Font",         "Conversions");
    add(&cConversionProviderInputHandler, ":InputHandler", "Conversions");
    add(&cConversionProviderInteger,      ":Integer",      "Conversions");
    add(&cConversionProviderScreen,       ":Screen",       "Conversions");
    add(&cConversionProviderString,       ":String",       "Conversions");
    add(&cConversionProviderVertex,       ":Vertex",       "Conversions");
  }

  // IBinding* BindingRegistry::get(IAssetUser<IBinding>* client, IActionClient& owner, JSONObject object, IStateListener<IBinding*>* listener, bool required) {
  //   cLocalProviderBinding.setLocalBindings(owner.getBindingRegistry());
  //   IBinding* mBinding = cBindings.get(client, owner, object, nullptr, required);
  //   cLocalProviderBinding.setLocalBindings(nullptr);
  //   return mBinding;
  // }

  BindingRegistry::Dummy::Dummy(IActionClient& owner) {
    // Nothing to do.
  }

  void BindingRegistry::Dummy::bind(const std::string& function) const {
    // Nothing to do.
  }

  std::vector<std::string> BindingRegistry::Dummy::getAvailableProviders() const {
    return std::vector<std::string>();
  }

  bool BindingRegistry::Dummy::renderProviderIcon(const std::string& id) const {
    return false;
  }

  bool BindingRegistry::Dummy::renderWrappedIcon() const {
    return false;
  }

  bool BindingRegistry::Dummy::isConfigurable() const {
    return false;
  }

  std::string BindingRegistry::Dummy::getID() const {
    return "";
  }

  void BindingRegistry::Dummy::set(const std::string& id) {
    // Nothing to do.
  }

  void BindingRegistry::Dummy::getWrappedProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool BindingRegistry::Dummy::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void BindingRegistry::Dummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void BindingRegistry::Dummy::getAssetProperties(PropertyMaker& owner) {
    // Nothing to do.
  }

  bool BindingRegistry::Dummy::isDefaultConfiguration() const {
    return true;
  }
}
