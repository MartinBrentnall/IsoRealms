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
#include "ScreenRegistry.h"

#include "IsoRealms/Project/Project.h"

namespace IsoRealms {
  IStateNotifier* ScreenRegistry::add(IScreen* asset, const std::string& id, const std::string& path, bool stateChanges) {
    std::map<IScreen*, std::unique_ptr<Proxy>>::iterator mExistingProxy = cProxyMapping.find(asset);
    if (mExistingProxy == cProxyMapping.end()) {
      std::unique_ptr<Proxy> mNewProxy = std::make_unique<Proxy>(*this, asset);
      AssetClientManager::add(mNewProxy.get(), id, path);
      if (!cProject.isLoading()) {
        for (IScreenListener* mListener : cListeners) {
          mListener->screenAdded(mNewProxy.get());
        }
      }
      cProxyMapping.emplace(asset, std::move(mNewProxy)).first->second.get();
      return nullptr;
    }
    AssetClientManager::add(mExistingProxy->second.get(), id, path);
    return nullptr;
  }

  IStateNotifier* ScreenRegistry::add(IAssetProvider<IComponentData, IScreen>* provider, const std::string& id, const std::string& path, bool stateChanges) {
    return AssetClientManager::add(provider, id, path, stateChanges);
  }

  void ScreenRegistry::Dummy::renderScreen(float scale, float aspectRatio) const {
    // Nothing to do
  }

  bool ScreenRegistry::Dummy::renderAssetIcon() const {
    Utils::renderIconNone();
    return true;
  }

  void ScreenRegistry::Dummy::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void ScreenRegistry::Dummy::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool ScreenRegistry::Dummy::isDefaultConfiguration() const {
    return true;
  }
}
