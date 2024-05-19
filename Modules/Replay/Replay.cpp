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
#include "Replay.h"

namespace IsoRealms::Replay {
  const std::string Replay::ID_RESOURCE_PLAYBACK  = "Player";
  const std::string Replay::ID_RESOURCE_RECORDER  = "Recorder";
      
  const std::string Replay::NAME_RESOURCE_PLAYBACK = "Players";
  const std::string Replay::NAME_RESOURCE_RECORDER = "Recorders";

  Replay::Replay(IProject* project, IResourceTypeRegistry* registry, IAssetLiterals* literals):
                    cResourceTypePlayer(this),
                    cResourceTypeRecorder(this) {
    registry->add(&cResourceTypePlayer,   ID_RESOURCE_PLAYBACK, NAME_RESOURCE_PLAYBACK, IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
    registry->add(&cResourceTypeRecorder, ID_RESOURCE_RECORDER, NAME_RESOURCE_RECORDER, IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
  }

  void Replay::load(IProject* project, JSONObject object) {
    // Nothing to do.
  }

  void Replay::save(JSONObject object, IAssetIdentifier* identifier) {
    // Nothing to do.
  }

  void Replay::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }
  
  void Replay::unregisterAssets(IAssetRemover* remover, IAssets* releaser) {
    // Nothing to do.
  }

  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<Replay>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::IProject* project, IsoRealms::IResourceTypeRegistry* registry, IsoRealms::IAssetLiterals* literals) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::IProject * project, IsoRealms::IResourceTypeRegistry * registry, IsoRealms::IAssetLiterals * literals) {
#endif
  std::lock_guard<std::mutex> mLockGuard(IsoRealms::Replay::cModuleInstantiationMutex);
  return IsoRealms::Replay::ModuleInstances.emplace_back(std::make_unique<IsoRealms::Replay::Replay>(project, registry, literals)).get();
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::lock_guard<std::mutex> mLockGuard(IsoRealms::Replay::cModuleInstantiationMutex);
  IsoRealms::Utils::removeElementUnique(IsoRealms::Replay::ModuleInstances, module);
}
