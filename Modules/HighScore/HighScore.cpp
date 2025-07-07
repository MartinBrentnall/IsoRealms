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
#include "HighScore.h"

namespace IsoRealms::HighScore {
  const std::string HighScore::ID_RESOURCE_SCORE_TRACKER  = "ScoreTracker";
  const std::string HighScore::ID_RESOURCE_SCORE_TABLE    = "ScoreTable";
      
  HighScore::HighScore(Project& project, IResourceTypeRegistry* registry):
                    cResourceTypeScoreTable(*this),
                    cResourceTypeScoreTracker(*this) {
    registry->add(&cResourceTypeScoreTable,   ID_RESOURCE_SCORE_TABLE,   "Score Table",   "Score Tables",   IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
    registry->add(&cResourceTypeScoreTracker, ID_RESOURCE_SCORE_TRACKER, "Score Tracker", "Score Trackers", IsoRealmsConstants::RESOURCE_CATEGORY_SYSTEM);
  }

  void HighScore::load(IProject& project, JSONObject object) {
    // Nothing to do.
  }

  void HighScore::save(JSONObject object) {
    // Nothing to do.
  }

  void HighScore::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }
  
  std::vector<std::unique_ptr<IProperty>> HighScore::getProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  void HighScore::updateRuntime(unsigned int milliseconds) {
    updateRuntime2(cResourceTypeScoreTracker, milliseconds);
  }
  
  void HighScore::updateEditing(unsigned int milliseconds) {
    // Nothing to do.
  }
  
  void HighScore::reset() {
    reset2(cResourceTypeScoreTracker);
  }  
  
  std::mutex cModuleInstantiationMutex;
  std::vector<std::unique_ptr<HighScore>> ModuleInstances;
}

#ifdef __linux__
extern "C" IsoRealms::IModuleHandle* create(IsoRealms::Project* project, IsoRealms::IResourceTypeRegistry* registry) {
#elif _WIN32
extern "C" IsoRealms::IModuleHandle* __declspec(dllexport) __stdcall create(IsoRealms::Project * project, IsoRealms::IResourceTypeRegistry * registry) {
#endif
  std::unique_ptr<IsoRealms::HighScore::HighScore> mModule = std::make_unique<IsoRealms::HighScore::HighScore>(*project, registry);
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::HighScore::cModuleInstantiationMutex);
    return IsoRealms::HighScore::ModuleInstances.emplace_back(std::move(mModule)).get();
  }
}

#ifdef __linux__
extern "C" void destroy(IsoRealms::IModuleHandle* module) {
#elif _WIN32
extern "C" void __declspec(dllexport) __stdcall destroy(IsoRealms::IModuleHandle* module) {
#endif
  std::unique_ptr<IsoRealms::HighScore::HighScore> mModule;
  {
    std::lock_guard<std::mutex> mLockGuard(IsoRealms::HighScore::cModuleInstantiationMutex);
    mModule = IsoRealms::Utils::removeElementUnique(IsoRealms::HighScore::ModuleInstances, module);
  }
}
