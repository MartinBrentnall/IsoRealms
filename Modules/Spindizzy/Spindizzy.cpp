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
#include "Spindizzy.h"

namespace IsoRealms::Spindizzy {
  Spindizzy::Spindizzy(Project& project, IComponentTypeRegistry& registry) :
                    cComponentBall(*this),
                    cComponentC64LiftGraphics(*this),
                    cComponentC64TerrainGraphics(*this),
                    cComponentDebrisChunk(*this),
                    cComponentGyroscope(*this),
                    cComponentJewel(*this),
                    cComponentTop(*this) {
    registry.add(&cComponentBall,               "Ball");
    registry.add(&cComponentC64LiftGraphics,    "C64LiftGraphics");
    registry.add(&cComponentC64TerrainGraphics, "C64TerrainGraphics");
    registry.add(&cComponentDebrisChunk,        "DebrisChunk");
    registry.add(&cComponentGyroscope,          "Gyroscope");
    registry.add(&cComponentJewel,              "Jewel");
    registry.add(&cComponentTop,                "Top");
  }
  
  void Spindizzy::publish(ResourcePublisher& publisher) {
    // Nothing to do.
  }

  void Spindizzy::updateInputs(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void Spindizzy::updateRuntime(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void Spindizzy::updateEditing(unsigned int milliseconds) {
    updateEditing2(cComponentDebrisChunk, milliseconds);
    updateEditing2(cComponentGyroscope,   milliseconds);
    updateEditing2(cComponentJewel,       milliseconds);
    updateEditing2(cComponentTop,         milliseconds);
  }
  
  void Spindizzy::reset() {
    // Nothing to do.
  }  
  
  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<Spindizzy>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IComponentTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IComponentTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::Spindizzy::Spindizzy> mModule = std::make_unique<IsoRealms::Spindizzy::Spindizzy>(*project, *registry);
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Spindizzy::cModuleInstantiationMutex);
    return IsoRealms::Spindizzy::ModuleInstances.emplace_back(std::move(mModule)).get();
  }
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::unique_ptr<IsoRealms::Spindizzy::Spindizzy> mModule;
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::Spindizzy::cModuleInstantiationMutex);
    mModule = IsoRealms::Utils::removeElementUnique(IsoRealms::Spindizzy::ModuleInstances, module);
  }
}
