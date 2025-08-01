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
#include "Replay.h"

namespace IsoRealms::Replay {
  Replay::Replay(Project& project, IResourceTypeRegistry& registry):
                    cResourceTypeReplayer(*this) {
    registry.add(&cResourceTypeReplayer, "Replayer");
  }

  void Replay::load(IProject& project, JSONObject object) {
    // Nothing to do.
  }

  void Replay::save(JSONObject object) {
    // Nothing to do.
  }

  void Replay::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }
  
  void Replay::getProperties() {
    // Nothing to do.
  }

  void Replay::updateInputs(unsigned int milliseconds) {
    updateInputs2(cResourceTypeReplayer, milliseconds);
  }
  
  void Replay::updateRuntime(unsigned int milliseconds) {
    updateRuntime2(cResourceTypeReplayer, milliseconds);
  }
  
  void Replay::updateEditing(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void Replay::reset() {
    reset2(cResourceTypeReplayer);
  }  
  
  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<Replay>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IResourceTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::Replay::Replay> mModule = std::make_unique<IsoRealms::Replay::Replay>(*project, *registry);
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Replay::cModuleInstantiationMutex);
    return IsoRealms::Replay::ModuleInstances.emplace_back(std::move(mModule)).get();
  }
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::unique_ptr<IsoRealms::Replay::Replay> mModule;
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Replay::cModuleInstantiationMutex);
    mModule = IsoRealms::Utils::removeElementUnique(IsoRealms::Replay::ModuleInstances, module);
  }
}
