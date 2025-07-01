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
#include "FileSound.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  const std::string FileSound::JSON_FILENAME = "filename";

  std::mutex FileSound::cRuntimeLoadMutex;

  FileSound::FileSound(IProject& project, Basics& basics, IResourceData& data) :
            cDefBasics(basics),
            cDefFile(project) {
  }
  
  FileSound::FileSound(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options) :
            FileSound(project, basics, data) {
    cDefFile.load(JSON_FILENAME, object);
    reloadData();
  }

  void FileSound::registerAssets(IAssetRegistry& assets) {
    assets.add(this, "", "Play Sound");
  }
  
  void FileSound::save(JSONObject object) const {
    cDefFile.save(JSON_FILENAME, object);
  }

  void FileSound::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool FileSound::renderIcon() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> FileSound::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<File>>("File", "TODO", cDefFile));
    return mProperties;
  }

  void FileSound::setVolume(float volume) {
    for (sf::Sound& mSound : cRuntimeSounds) {
      mSound.setVolume(volume * 100.0f);
    }
  }

  IAction* FileSound::createAction(JSONObject object, IResourceData& owner, IBindingRegistry* localObjects) {
    return this;
  }

  IAction* FileSound::createAction(IResourceData& owner, IBindingRegistry* localObjects) {
    return this;
  }
  
  void FileSound::destroyAction(IAction* action, IAssets& assets) {
    // Nothing to do.
  }

  bool FileSound::renderAssetIcon() const {
    return renderIcon();
  }

  void FileSound::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  bool FileSound::hasConfiguration() const {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> FileSound::getAssetProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  bool FileSound::isDefaultConfiguration() const {
    return true;
  }

  void FileSound::execute() {
    sf::Sound& mNewSound = cRuntimeSounds.emplace_back(cRuntimeSoundData);
    mNewSound.setVolume(cDefBasics.getSoundVolume() * 100.0f);
    mNewSound.play();

    // Clean up any finished instances of this sound.
    while (cRuntimeSounds.front().getStatus() == sf::Sound::Stopped) {
      cRuntimeSounds.pop_front();
    }
  }

  void FileSound::reloadData() {
    std::lock_guard<std::mutex> mLockGuard(cRuntimeLoadMutex);
    std::string mResource = cDefFile.getPath();
    if (!cRuntimeSoundData.loadFromFile(mResource)) {
      std::cout << "WARNING: FileSound::reloadData: Unable to sound data from file \"" << mResource << "\"" << std::endl;
    }
  }
}
