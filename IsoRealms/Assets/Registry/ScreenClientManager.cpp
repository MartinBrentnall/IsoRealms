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
#include "ScreenClientManager.h"

#include "IsoRealms/Project.h"

namespace IsoRealms {
  IScreen* ScreenClientManager::add(IScreen* asset, const std::string& id, const std::string& category, bool stateChanges, Project& project) {
    std::map<IScreen*, std::unique_ptr<ScreenProxy>>::iterator mExistingProxy = cScreenProxyMapping.find(asset);
    if (mExistingProxy == cScreenProxyMapping.end()) {
      std::unique_ptr<ScreenProxy> mNewProxy = std::make_unique<ScreenProxy>(*this, asset);
      cScreens.add(mNewProxy.get(), id, category);
      if (!project.isProcessingInput()) {
        for (IScreenListener* mListener : cListeners) {
          mListener->screenAdded(project, mNewProxy.get());
        }
      }
      return cScreenProxyMapping.emplace(asset, std::move(mNewProxy)).first->second.get();
    }
    cScreens.add(mExistingProxy->second.get(), id, category);
    return mExistingProxy->second.get();
  }
}
