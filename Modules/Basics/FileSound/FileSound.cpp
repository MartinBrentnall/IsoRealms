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
#include "FileSound.h"

#include "Modules/Basics/Basics.h"

namespace IsoRealms::Basics {
  std::mutex FileSound::cRuntimeLoadMutex;

  FileSound::FileSound(Basics& basics, IComponentData& data) :
            cDefBasics(basics),
            cDefFile(data.getProject(), [this]() {
              reloadData();
            }) {
  }
  
  void FileSound::registerAssets(ComponentAssetRegistry& assets) {
    assets.add<IAction>(this, "", "Play a Sound");
  }
  
  void FileSound::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool FileSound::renderIcon() const {
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(0.6f, 1.0f, 1.0f);
    Utils::renderOval(-0.75f, 0.0f, 0.2f, 0.4f, 0.0f, 1.0f);
    Utils::renderOval(-0.65f, 0.0f, 0.2f, 0.4f, 0.0f, 1.0f);
    Utils::renderOval(-0.55f, 0.0f, 0.2f, 0.4f, 0.0f, 1.0f);
    Utils::renderOval(-0.45f, 0.0f, 0.2f, 0.4f, 0.0f, 1.0f);
    Utils::renderOval(-0.0f,  0.0f, 0.45f,  0.9f, 0.0f, 1.0f);
    glColor3f(0.07f, 0.47f, 0.47f);
    Utils::renderOval(-0.0f,  0.0f, 0.3f,  0.75f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    return true;
  }

  void FileSound::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    owner.createPropertyTreeSelector("filename", cDefFile);
  }

  void FileSound::removed() {
    // Nothing to do.
  }

  void FileSound::setVolume(float volume) {
    for (sf::Sound& mSound : cRuntimeSounds) {
      mSound.setVolume(volume * 100.0f);
    }
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

  bool FileSound::renderAssetIcon() const {
    return renderIcon();
  }

  void FileSound::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void FileSound::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool FileSound::isDefaultConfiguration() const {
    return true;
  }

  void FileSound::reloadData() {
    std::lock_guard<std::mutex> mLockGuard(cRuntimeLoadMutex);
    std::string mComponent = cDefFile.getPath();
    if (!cRuntimeSoundData.loadFromFile(mComponent)) {
      std::cout << "WARNING: FileSound::reloadData: Unable to sound data from file \"" << mComponent << "\"" << std::endl;
    }
  }
}
