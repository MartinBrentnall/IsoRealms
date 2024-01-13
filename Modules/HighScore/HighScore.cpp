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
#include "HighScore.h"

namespace IsoRealms::HighScore {
  const std::string HighScore::TAG_RESOURCE_SCORE_TRACKER  = "ScoreTracker";
  const std::string HighScore::TAG_RESOURCE_SCORE_TABLE    = "ScoreTable";
      
  const std::string HighScore::NAME_RESOURCE_SCORE_TRACKER = "Score Trackers";
  const std::string HighScore::NAME_RESOURCE_SCORE_TABLE   = "Score Tables";

  HighScore::HighScore(IProject* project, IResourceTypeRegistry* registry, IAssetLiterals* literals):
                    cResourceTypeScoreTable(this),
                    cResourceTypeScoreTracker(this) {
    registry->add(&cResourceTypeScoreTable,   TAG_RESOURCE_SCORE_TABLE,   NAME_RESOURCE_SCORE_TABLE,   IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
    registry->add(&cResourceTypeScoreTracker, TAG_RESOURCE_SCORE_TRACKER, NAME_RESOURCE_SCORE_TRACKER, IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
  }

  void HighScore::load(IProject* project, DOMNode& node) {
    // Nothing to do.
  }

  void HighScore::save(DOMNodeWriter* node, IAssetIdentifier* identifier) {
    // Nothing to do.
  }
  
  void HighScore::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }
  
  void HighScore::unregisterAssets(IAssetRemover* remover, IAssets* releaser) {
    // Nothing to do.
  }
}

#ifdef __linux__
extern "C" std::unique_ptr<IsoRealms::IModuleHandle> create(IsoRealms::IProject* project, IsoRealms::IResourceTypeRegistry* registry, IsoRealms::IAssetLiterals* literals) {
#elif _WIN32
extern "C" std::unique_ptr<IsoRealms::IModuleHandle> __declspec(dllexport) __stdcall create(IsoRealms::IProject * project, IsoRealms::IResourceTypeRegistry * registry, IsoRealms::IAssetLiterals * literals) {
#endif
  return std::make_unique<IsoRealms::HighScore::HighScore>(project, registry, literals);
}
