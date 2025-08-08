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
#include "Tables.h"

namespace IsoRealms::Tables {
  Tables::Tables(Project& project, IResourceTypeRegistry& registry):
                    cResourceTypeTable(*this) {
    registry.add(&cResourceTypeTable, "Table");
  }

  void Tables::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }
  
  void Tables::updateInputs(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void Tables::updateRuntime(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void Tables::updateEditing(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void Tables::reset() {
    // Nothing to do.
  }  
  
  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<Tables>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IResourceTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::Tables::Tables> mModule = std::make_unique<IsoRealms::Tables::Tables>(*project, *registry);
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Tables::cModuleInstantiationMutex);
    return IsoRealms::Tables::ModuleInstances.emplace_back(std::move(mModule)).get();
  }
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::unique_ptr<IsoRealms::Tables::Tables> mModule;
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Tables::cModuleInstantiationMutex);
    mModule = IsoRealms::Utils::removeElementUnique(IsoRealms::Tables::ModuleInstances, module);
  }
}
