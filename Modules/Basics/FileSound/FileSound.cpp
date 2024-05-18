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

  FileSound::FileSound(IProject* project, Basics* basics) :
            cDefBasics(basics) {
  }
  
  FileSound::FileSound(IProject* project, Basics* basics, JSONObject object, IOptions* options, IResourceData* data) :
            FileSound(project, basics) {
    cDefFile = object.getString(JSON_FILENAME);
    reloadData();
  }

  void FileSound::registerAssets(IAssetRegistry* assets) {
    assets->add(this, "", "Play Sound");
  }
  
  void FileSound::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    assets->remove(this);
  }
  
  void FileSound::save(JSONObject object, IAssetIdentifier* identifier) const {
    object.addString(JSON_FILENAME, cDefFile);
  }

  void FileSound::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool FileSound::renderIcon() const {
    return false;
  }

  std::vector<IProperty*> FileSound::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
  }

  void FileSound::setVolume(float volume) {
    for (sf::Sound& mSound : cRuntimeSounds) {
      mSound.setVolume(volume * 100.0f);
    }
  }

  IAction* FileSound::createAction(JSONObject object, IProject* project, IBindingRegistry* localObjects) {
    return this;
  }

  IAction* FileSound::createAction(IProject* project, IBindingRegistry* localObjects) {
    return this;
  }
  
  void FileSound::destroyAction(IAction* action, IAssets* assets) {
    // Nothing to do.
  }

  bool FileSound::renderAssetIcon() const {
    return renderIcon();
  }

  void FileSound::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void FileSound::execute() {
    sf::Sound& mNewSound = cRuntimeSounds.emplace_back(cRuntimeSoundData);
    mNewSound.setVolume(cDefBasics->getSoundVolume() * 100.0f);
    mNewSound.play();

    // Clean up any finished instances of this sound.
    while (cRuntimeSounds.front().getStatus() == sf::Sound::Stopped) {
      cRuntimeSounds.pop_front();
    }
  }

  const IActionType* FileSound::getActionType() const {
    return this;
  }

  bool FileSound::hasConfiguration() const {
    return false;
  }

  void FileSound::reloadData() {
    std::lock_guard<std::mutex> mLockGuard(cRuntimeLoadMutex);
    std::string mResource = System::getPath(cDefFile, false);
    if (!cRuntimeSoundData.loadFromFile(mResource)) {
      std::cout << "WARNING: FileSound::reloadData: Unable to sound data from file \"" << mResource << "\"" << std::endl;
    }
  }
}
