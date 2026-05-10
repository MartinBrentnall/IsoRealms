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
#include "IsoRealms/Project/Project.h"

#include <sol.hpp>

#include "BindingRegistry.h"

namespace IsoRealms {
  template <typename OWNER, typename FROM>
  template <typename OWNER2, typename TYPE>
  void BindingRegistry::Conversion<OWNER, FROM>::Instance<OWNER2, TYPE>::bind(const std::string& bindFunction) const {
    cDefLuaState[bindFunction](*cDefValue);
  }
  
  template class BindingRegistry::Conversion<IActionContext, Action>::Instance<IActionContext, Action>;
  template class BindingRegistry::Conversion<IResourceData,  Boolean>::Instance<IResourceData, Boolean>;  
  template class BindingRegistry::Conversion<IResourceData,  Colour>::Instance<IResourceData, Colour>;
  template class BindingRegistry::Conversion<IResourceData,  Float>::Instance<IResourceData, Float>;  
  template class BindingRegistry::Conversion<IResourceData,  Font>::Instance<IResourceData, Font>;  
  template class BindingRegistry::Conversion<IResourceData,  InputHandler>::Instance<IResourceData, InputHandler>;  
  template class BindingRegistry::Conversion<IResourceData,  Integer>::Instance<IResourceData, Integer>;  
  template class BindingRegistry::Conversion<IResourceData,  Screen>::Instance<IResourceData, Screen>;  
  template class BindingRegistry::Conversion<IResourceData,  String>::Instance<IResourceData, String>;  
  template class BindingRegistry::Conversion<IResourceData,  Vertex>::Instance<IResourceData, Vertex>;  
  
  BindingRegistry::BindingRegistry(Project& project) :
            AssetClientManager(&cDummy),
            cProject(project) {

    // Set up conversion providers.
    cConversionProviders.emplace_back(std::make_unique<Conversion<IActionContext, Action>>(      ":Action",       "Core Assets/Actions"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<IResourceData, Boolean>>(     ":Boolean",      "Core Assets/Booleans"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<IResourceData, Colour>>(      ":Colour",       "Core Assets/Colours"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<IResourceData, Float>>(       ":Float",        "Core Assets/Floats"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<IResourceData, Font>>(        ":Font",         "Core Assets/Fonts"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<IResourceData, InputHandler>>(":InputHandler", "Core Assets/Input Handlers"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<IResourceData, Integer>>(     ":Integer",      "Core Assets/Integers"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<IResourceData, Screen>>(      ":Screen",       "Core Assets/Screens"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<IResourceData, String>>(      ":String",       "Core Assets/Strings"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<IResourceData, Vertex>>(      ":Vertex",       "Core Assets/Vertices"));
            
    // Support local bindings.
    add(&cLocals, "~", "Local");

    // Support conversions.
    for (const std::unique_ptr<ConversionProvider>& mConversionProvider : cConversionProviders) {
      add(mConversionProvider.get(), mConversionProvider->getProviderID(), mConversionProvider->getConversionPath());
    }
  }

  IBinding* BindingRegistry::get(IAssetUser<IBinding>* client, IActionContext& owner, JSONObject object, IStateListener* listener, bool required) {
    cLocals.setBindings(owner.getBindingRegistry());
    return AssetClientManager::get(client, owner, object, listener, required);
  }

  IBinding* BindingRegistry::get(IAssetUser<IBinding>* client, IActionContext& owner, const std::string& id, IStateListener* listener) {
    cLocals.setBindings(owner.getBindingRegistry());

    for (const std::unique_ptr<ConversionProvider>& mConversionProvider : cConversionProviders) {
      if (id.starts_with(mConversionProvider->getProviderID() + "/")) {
        JSONDocument mDocument;
        JSONObject mConversionProviderObject = mDocument.addObject("temp");
        mConversionProviderObject.addString(JSON_KEY, mConversionProvider->getProviderID());
        JSONObject mAssetObject = mConversionProviderObject.addObject(JSON_ASSET);
        mAssetObject.addString(JSON_KEY, id.substr(mConversionProvider->getProviderID().length() + 1));

        return AssetClientManager::get(client, owner, mConversionProviderObject, listener, true);
      }
    }
    return AssetClientManager::get(client, owner, id, listener);
  }

  void BindingRegistry::forEachEntry(const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction) const {
    AssetClientManager::forEachEntry(getTreeItemInfoFunction);

    for (const std::unique_ptr<ConversionProvider>& mConversionProvider : cConversionProviders) {
      mConversionProvider->forEachEntry(cProject, getTreeItemInfoFunction);
    }

    // TODO: Add local bindings.
  }

  bool BindingRegistry::renderIcon(const std::string& id) const {
    for (const std::unique_ptr<ConversionProvider>& mConversionProvider : cConversionProviders) {
      if (id.starts_with(mConversionProvider->getProviderID() + "/")) {
        return mConversionProvider->renderIcon(cProject, id);
      }
    }
    return AssetClientManager::renderIcon(id);
  }

  void BindingRegistry::Dummy::bind(const std::string& function) const {
    // Nothing to do.
  }

  void BindingRegistry::Dummy::forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    // Nothing to do.
  }

  bool BindingRegistry::Dummy::renderTreeItemIcon(const std::string& id) const {
    return false;
  }

  bool BindingRegistry::Dummy::isConfigurable() const {
    return false;
  }

  TreeItemInfo BindingRegistry::Dummy::getTreeItemInfo() const {
    return TreeItemInfo{"", ""};
  }

  void BindingRegistry::Dummy::set(const std::string& id) {
    // Nothing to do.
  }

  void BindingRegistry::Dummy::getWrappedProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool BindingRegistry::Dummy::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void BindingRegistry::Dummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void BindingRegistry::Dummy::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool BindingRegistry::Dummy::isDefaultConfiguration() const {
    return true;
  }

  std::string BindingRegistry::Dummy::getConversionPath() const {
    return "";
  }
}
