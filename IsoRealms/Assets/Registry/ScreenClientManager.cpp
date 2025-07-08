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
#include "ScreenClientManager.h"

#include "IsoRealms/Project.h"

namespace IsoRealms {
  IStateNotifier<IScreen>* ScreenClientManager::add(IScreen* asset, const std::string& id, const std::string& category, bool stateChanges) {
    std::map<IScreen*, std::unique_ptr<ScreenProxy>>::iterator mExistingProxy = cScreenProxyMapping.find(asset);
    if (mExistingProxy == cScreenProxyMapping.end()) {
      std::unique_ptr<ScreenProxy> mNewProxy = std::make_unique<ScreenProxy>(*this, asset);
      cScreens.add(mNewProxy.get(), id, category);
      if (!cProject.isProcessingInput()) {
        for (IScreenListener* mListener : cListeners) {
          mListener->screenAdded(cProject, mNewProxy.get());
        }
      }
      cScreenProxyMapping.emplace(asset, std::move(mNewProxy)).first->second.get();
      return nullptr;
    }
    cScreens.add(mExistingProxy->second.get(), id, category);
    return nullptr;
  }
}
