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
#include "StringRegistry.h"

#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  StringRegistry::StringRegistry(Project& project) :
            AssetClientManager(&cLiteral, "Literal", "Literal"),
            cProject(project),
            cLiteral(project.getApplication().getMetadata("LiteralString")) {

    // Set up conversion providers.
    cConversionProviders.emplace_back(std::make_unique<Conversion<Float>>(  ":Float",   "Floats"));
    cConversionProviders.emplace_back(std::make_unique<Conversion<Integer>>(":Integer", "Integers"));
          
    // Support conversions.
    for (const std::unique_ptr<ConversionProvider>& mConversionProvider : cConversionProviders) {
      add(mConversionProvider.get(), mConversionProvider->getProviderID(), mConversionProvider->getConversionPath());
    }
  }

  IString* StringRegistry::get(IAssetUser<IString>* client, IResourceData& owner, JSONObject object, IStateListener* listener, bool required) {
    return AssetClientManager::get(client, owner, object, listener, required);
  }

  IString* StringRegistry::get(IAssetUser<IString>* client, IResourceData& owner, const std::string& id, IStateListener* listener) {
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

  void StringRegistry::forEachEntry(const std::function<void(const TreeItemInfo&)>& getTreeItemInfoFunction) const {
    AssetClientManager::forEachEntry(getTreeItemInfoFunction);

    for (const std::unique_ptr<ConversionProvider>& mConversionProvider : cConversionProviders) {
      mConversionProvider->forEachEntry(cProject, getTreeItemInfoFunction);
    }

    // TODO: Add local bindings.
  }

  bool StringRegistry::renderIcon(const std::string& id) const {
    for (const std::unique_ptr<ConversionProvider>& mConversionProvider : cConversionProviders) {
      if (id.starts_with(mConversionProvider->getProviderID() + "/")) {
        return mConversionProvider->renderIcon(cProject, id);
      }
    }
    return AssetClientManager::renderIcon(id);
  }

  StringRegistry::Literal::Instance::Instance(Project& project, const std::string& value) :
            cMetadata(project.getApplication().getMetadata("LiteralString")),
            cValue(value) {
  }

  void StringRegistry::Literal::Instance::setValue(const std::string& value) {
    cValue = value;
  }

  std::string StringRegistry::Literal::Instance::getValue() const {
    return cValue;
  }

  bool StringRegistry::Literal::Instance::renderAssetIcon() const {
    return false;
  }

  void StringRegistry::Literal::Instance::getAssetProperties(IPropertyMaker& owner) {
    owner.createPropertyNativeString(JSON_VALUE, [this]() {return cValue;}, [this](const std::string& value) {cValue = value;});
  }

  bool StringRegistry::Literal::Instance::isDefaultConfiguration() const {
    return cValue == "";
  }

  std::string StringRegistry::Literal::Instance::getConversionPath() const {
    return "";
  }

  bool StringRegistry::Literal::Instance::isConfigurable() const {
    return false;
  }

  void StringRegistry::Literal::Instance::saveAsset(JSONObject object) const {
    object.addString(JSON_VALUE, cValue);
  }
}
