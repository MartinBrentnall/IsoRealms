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
#include "Hue.h"

namespace IsoRealms::Hue {
  const std::string Hue::TAG_RESOURCE_HUE_MANAGER = "HueManager";

  const std::string Hue::NAME_RESOURCE_HUE_MANAGER = "Hue Manager";

  Hue::Hue(IProject* project, IResourceTypeRegistry* registry, IAssetLiterals* literals) :
                    cResourceTypeHueManager(this) {
    registry->add(&cResourceTypeHueManager, TAG_RESOURCE_HUE_MANAGER, NAME_RESOURCE_HUE_MANAGER, IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
  }

  void Hue::load(IProject* project, DOMNode& node) {
    // Nothing to do.
  }

  void Hue::save(DOMNodeWriter* node, IAssetIdentifier* identifier) {
    // Nothing to do.
  }
  
  void Hue::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }
  
  void Hue::unregisterAssets(IAssetRemover* remover, IAssets* releaser) {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<Hue>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::IProject* project, IsoRealms::IResourceTypeRegistry* registry, IsoRealms::IAssetLiterals* literals) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::IProject * project, IsoRealms::IResourceTypeRegistry * registry, IsoRealms::IAssetLiterals * literals) {
#endif
  return IsoRealms::Hue::ModuleInstances.emplace_back(std::make_unique<IsoRealms::Hue::Hue>(project, registry, literals)).get();
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  IsoRealms::Utils::removeElementUnique(IsoRealms::Hue::ModuleInstances, module);
}
