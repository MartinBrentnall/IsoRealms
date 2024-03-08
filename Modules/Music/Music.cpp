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
#include "Music.h"

namespace IsoRealms::Music {
  const std::string Music::TAG_RESOURCE_MUSIC_PLAYER = "MusicPlayer";

  const std::string Music::NAME_RESOURCE_MUSIC_PLAYER = "Music Player";

  Music::Music(IProject* project, IResourceTypeRegistry* registry, IAssetLiterals* literals) :
                    cResourceTypeMusicPlayer(this) {
    registry->add(&cResourceTypeMusicPlayer, TAG_RESOURCE_MUSIC_PLAYER, NAME_RESOURCE_MUSIC_PLAYER, IsoRealmsConstants::RESOURCE_CATEGORY_SIMPLE_VARIABLES);
  }

  void Music::load(IProject* project, DOMNode& node) {
    // Nothing to do.
  }

  void Music::save(DOMNodeWriter* node, IAssetIdentifier* identifier) {
    // Nothing to do.
  }
  
  void Music::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }
  
  void Music::unregisterAssets(IAssetRemover* remover, IAssets* releaser) {
    // Nothing to do.
  }

  std::vector<std::unique_ptr<Music>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::IProject* project, IsoRealms::IResourceTypeRegistry* registry, IsoRealms::IAssetLiterals* literals) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::IProject * project, IsoRealms::IResourceTypeRegistry * registry, IsoRealms::IAssetLiterals * literals) {
#endif
  return IsoRealms::Music::ModuleInstances.emplace_back(std::make_unique<IsoRealms::Music::Music>(project, registry, literals)).get();
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  IsoRealms::Utils::removeElementUnique(IsoRealms::Music::ModuleInstances, module);
}
