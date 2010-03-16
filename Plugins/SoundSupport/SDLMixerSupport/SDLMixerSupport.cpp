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
  std::vector<std::string> mPath;
  mPath.push_back("Configure");
  mPath.push_back("Sounds");
  cConfigureSoundsCommand = new ConfigureSoundsCommand(this);
  cPluginCommands.push_back(new DefaultCommandInfo(mPath, cConfigureSoundsCommand));
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

std::vector<ICommandInfo*> SDLMixerSupport::getCommandInfo() {
  return cPluginCommands;
}

void SDLMixerSupport::setEditingContext(BlockLocation*, IComponentContainer* componentContainer) {
  cConfigureSoundsCommand->setComponentContainer(componentContainer);
}

extern "C" IPlugin* create() {
  return new SDLMixerSupport();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
