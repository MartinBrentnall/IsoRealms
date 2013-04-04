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

void SDLMixerSupport::createResources(DOMNodeWrapper* node, IRuntimeContext* runtimeContext) {
  cRuntimeContext = runtimeContext;
  for (int i = 0; i < node->getChildCount(); i++) {
    DOMNodeWrapper *mNode = node->getChild(i);
    std::string mValueAsString = mNode->getNodeName();
    if (mValueAsString == "File") {
      std::string mFilename = mNode->getStringValue();
      SDLMixerSound* mSound = new SDLMixerSound();
      cSounds.push_back(mSound);
      runtimeContext->add(mSound, mFilename, mNode);
    }
  }
}

void SDLMixerSupport::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  for (unsigned int i = 0; i < cSounds.size(); i++) {
    DOMNodeWriter* mFileNode = node->addBranch("File");
    mFileNode->addText(cSounds[i]->getName());
  }
}

void SDLMixerSupport::setEditingContext(IEditingContext* editingContext, IResourceManager* resourceManager) {
  resourceManager->addResourceType(this, "From File...");
  cComponentContainer = editingContext->getComponentContainer();
}

void SDLMixerSupport::createResource(IResourceAccessor* resources) {
  DialogAudioFileSelector* mDialog = new DialogAudioFileSelector(cComponentContainer, resources, this);
  cCreateDialogs.push_back(mDialog);
  cComponentContainer->addComponent(mDialog);
  // TODO: Implement this
}

void SDLMixerSupport::editResource(ISound* sound, IResourceAccessor* resources) {
  // TODO: Implement this
}

void SDLMixerSupport::removeResource(ISound* sound, IResourceAccessor* resources, IResourceInstanceListener<ISound>* listener) {
  for (unsigned int i = 0; i < cSounds.size(); i++) {
    if (cSounds[i] == sound) {
      listener->resourcePendingDestruction(sound);
      delete cSounds[i];
      cSounds.erase(cSounds.begin() + i);
      return;
    }
  }
}

void SDLMixerSupport::dialogConfirmed(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
      std::string mFileName = cCreateDialogs[i]->getFileName();
      mFileName = mFileName.substr(mFileName.find_last_of('/') + 1);
      SDLMixerSound* mNewSound = new SDLMixerSound(mFileName);
      cRuntimeContext->add(mNewSound, mNewSound->getName());
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      cSounds.push_back(mNewSound);
      return;
    }
  }
}

void SDLMixerSupport::dialogCancelled(IRectangularComponent* dialog) {
  for (unsigned int i = 0; i < cCreateDialogs.size(); i++) {
    if (dialog == cCreateDialogs[i]) {
//       Colour* mNewColour = cCreateDialogs[i]->getColour();
//       delete mNewColour;
      cCreateDialogs.erase(cCreateDialogs.begin() + i);
      return;
    }
  }
}

extern "C" IPlugin* create() {
  return new SDLMixerSupport();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
