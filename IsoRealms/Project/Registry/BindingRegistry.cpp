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
  
  template class BindingRegistry::Conversion<IActionClient, Action>::Instance<IActionClient, Action>;
  template class BindingRegistry::Conversion<IResourceData, Boolean>::Instance<IResourceData, Boolean>;  
  template class BindingRegistry::Conversion<IResourceData, Colour>::Instance<IResourceData, Colour>;
  template class BindingRegistry::Conversion<IResourceData, Float>::Instance<IResourceData, Float>;  
  template class BindingRegistry::Conversion<IResourceData, Font>::Instance<IResourceData, Font>;  
  template class BindingRegistry::Conversion<IResourceData, InputHandler>::Instance<IResourceData, InputHandler>;  
  template class BindingRegistry::Conversion<IResourceData, Integer>::Instance<IResourceData, Integer>;  
  template class BindingRegistry::Conversion<IResourceData, Screen>::Instance<IResourceData, Screen>;  
  template class BindingRegistry::Conversion<IResourceData, String>::Instance<IResourceData, String>;  
  template class BindingRegistry::Conversion<IResourceData, Vertex>::Instance<IResourceData, Vertex>;  
  
  BindingRegistry::BindingRegistry(Project& project) :
            AssetClientManager(&cDummy),
            cProject(project) {

    // Support local bindings.
    add(&cLocals, "~", "Local");

    // Support conversions.
    add(&cActions,       ":Action",       "Actions");
    add(&cBooleans,      ":Boolean",      "Booleans");
    add(&cColours,       ":Colour",       "Colours");
    add(&cFloats,        ":Float",        "Floats");
    add(&cFonts,         ":Font",         "Fonts");
    add(&cInputHandlers, ":InputHandler", "Input Handlers");
    add(&cIntegers,      ":Integer",      "Integers");
    add(&cScreens,       ":Screen",       "Screens");
    add(&cStrings,       ":String",       "Strings");
    add(&cVertices,      ":Vertex",       "Vertices");
  }

  IBinding* BindingRegistry::get(IAssetUser<IBinding>* client, IActionClient& owner, JSONObject object, IStateListener* listener, bool required) {
    cLocals.setBindings(owner.getBindingRegistry());
    return AssetClientManager::get(client, owner, object, listener, required);
  }

  IBinding* BindingRegistry::get(IAssetUser<IBinding>* client, IActionClient& owner, const std::string& id, IStateListener* listener) {
    cLocals.setBindings(owner.getBindingRegistry());
    if (id.starts_with(":Boolean/")) {
      JSONDocument mDocument;
      JSONObject mBooleanBindingObject = mDocument.addObject("temp");
      mBooleanBindingObject.addString(JSON_KEY, ":Boolean");
      JSONObject mAssetObject = mBooleanBindingObject.addObject(JSON_ASSET);
      mAssetObject.addString(JSON_KEY, id.substr(9));

      return AssetClientManager::get(client, owner, mBooleanBindingObject, listener, true);
    }
    return AssetClientManager::get(client, owner, id, listener);
  }

  void BindingRegistry::forEachEntry(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const {
    AssetClientManager::forEachEntry(getTreeItemInfoFunction);

    cProject.forEachEntry<IBoolean>([&getTreeItemInfoFunction](const TreeItemInfo& mTreeItemInfo) {
      TreeItemInfo mBooleanTreeItemInfo(":Boolean/" + mTreeItemInfo.cID, "Booleans/" + mTreeItemInfo.cPath);
      getTreeItemInfoFunction(mBooleanTreeItemInfo);
    });
    // TODO: Add conversions.
    // TODO: Add local bindings.
  }

  bool BindingRegistry::renderIcon(const std::string& id) const {
    if (id.starts_with(":Boolean/")) {
      return cProject.renderIcon<IBoolean>(id.substr(9));
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

  bool BindingRegistry::Dummy::renderWrappedIcon() const {
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

  std::string BindingRegistry::Dummy::getConversionPath() const {
    return "";
  }
}
