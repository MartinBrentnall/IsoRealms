/*
 * Copyright 2009,2010 Martin Brentnall
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
#include "SDLMixerSupport.h"

SDLMixerSupport::SDLMixerSupport() {
  int mAudioRate = 44100;
  Uint16 mAudioFormat = AUDIO_S16SYS;
  int mAudioChannels = 2;
  int mAudioBuffers = 1024;
  if (Mix_OpenAudio(mAudioRate, mAudioFormat, mAudioChannels, mAudioBuffers) != 0) {
    std::cout << "Warning: Unable to initialize audio: " <<  Mix_GetError() << std::endl;
  } else {
    Mix_AllocateChannels(32);
  }
}

std::string SDLMixerSupport::getName() {
  return "SDL Mixer Support";
}

std::vector<ISound*> SDLMixerSupport::getSounds() {
  return cSounds;
}

void SDLMixerSupport::addSound(ISound* sound) {
  cSounds.push_back(sound);
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    cListeners[i]->soundAdded(sound);
  }
}

void SDLMixerSupport::removeSound(ISound* sound) {
  for (unsigned int i = 0; i < cSounds.size(); i++) {
    if (sound == cSounds[i]) {
      cSounds.erase(cSounds.begin() + i);
      for (unsigned int j = 0; j < cListeners.size(); j++) {
        cListeners[j]->soundRemoved(sound);
      }
      delete sound;
    }
  }
}

void SDLMixerSupport::addSoundSupportListener(ISoundSupportListener* listener) {
  cListeners.push_back(listener);
}

void SDLMixerSupport::removeSoundSupportListener(ISoundSupportListener* listener) {
  for (unsigned int i = 0; i < cListeners.size(); i++) {
    if (cListeners[i] == listener) {
      cListeners.erase(cListeners.begin() + i);
    }
  }
}

void SDLMixerSupport::save(DOMNodeWriter* node) {
  for (unsigned int i = 0; i < cSounds.size(); i++) {
    DOMNodeWriter* mFileNode = node->addBranch("File");
    mFileNode->addText(cSounds[i]->getName());
  }
}

void SDLMixerSupport::load(DOMNodeWrapper* node) {
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "File") {
      std::string mFilename = mNode->getStringValue();
      SDLMixerSound* mSound = new SDLMixerSound(mFilename);
      addSound(mSound);
    }
  }
}

void SDLMixerSupport::setEditingContext(IEditingContext* editingContext) {
  std::vector<std::string> mPath;
  mPath.push_back("Configure");
  mPath.push_back("Sounds");
  cComponentContainer = editingContext->getComponentContainer();
  OpenDialogCommand* mConfigureSoundsCommand = new OpenDialogCommand(cComponentContainer, this);
  editingContext->registerCommand(new DefaultCommandInfo(mPath, mConfigureSoundsCommand));
}

IHUDComponent* SDLMixerSupport::createComponent() {
  return new SoundConfigurationDialog(cComponentContainer, this);
}

extern "C" IPlugin* create() {
  return new SDLMixerSupport();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
