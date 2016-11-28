/*
 * Copyright 2015 Martin Brentnall
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
#include "LayerInputPlayer.h"

LayerInputPlayer::LayerInputPlayer(IModuleInputPersistence* module) {
  cElapsedTime = 0;
  cModule = module;
  cProject = module->getProject();
  cRecording = module->getRecording();
  cFinished = false;
  std::string mInputMapping;
  std::getline(*cRecording, mInputMapping); // First line is project file
  std::getline(*cRecording, mInputMapping); // Second line is the input mapping that we want
  std::vector<std::string> mInputs = Utils::split(mInputMapping, ',');
  InputCommands* mInputCommands = cProject->getInputConfiguration();
  for (unsigned int i = 0; i < mInputs.size(); i++) {
    cInputs.push_back(nullptr);
  }
  for (std::string mInput : mInputs) {
    std::vector<std::string> mInputMappingPair = Utils::split(mInput, '=');
    if (mInputMappingPair.size() != 2) {
      std::cout << "Bad input mapping string: " << mInput << std::endl;
      exit(1);
    }
    unsigned int mInputID = std::stoi(mInputMappingPair[0]);
    cInputs[mInputID] = mInputCommands->findDigitalInput(mInputMappingPair[1].substr(1, mInputMappingPair[1].size() - 2));
  }
  readNextEvent();
}

void LayerInputPlayer::readNextEvent() {
  if (cRecording->eof()) {
    cFinished = true;
    return;
  }
  std::string mEventString;
  std::getline(*cRecording, mEventString); // Second line is the input mapping that we want
  std::vector<std::string> mEventAndTime = Utils::split(mEventString, '@');
  if (mEventAndTime.size() != 2) {
    if (cRecording->eof()) {
      cFinished = true;
      return;
    }
    std::cout << "Bad event string: " << mEventString << std::endl;
    return;
  }
  std::vector<std::string> mEvent = Utils::split(mEventAndTime[0], '=');
  cNextEvent.cTime  = std::stoi(mEventAndTime[1]);
  cNextEvent.cID    = std::stoi(mEvent[0]);
  cNextEvent.cState = mEvent[1] == "1";
}

void LayerInputPlayer::updateRuntime(unsigned int milliseconds) {
  while (!cFinished && cNextEvent.cTime == cElapsedTime) {
    cInputs[cNextEvent.cID]->trigger(cNextEvent.cState);
    readNextEvent();
  }

  if (cProject->hasCompleted()) {
    cRecording->close();
    cModule->quit();
    return;
  }  
  cElapsedTime += milliseconds;
  cProject->updateRuntime(milliseconds);
}

void LayerInputPlayer::updateEditing(unsigned int milliseconds) {
  // Not supported
}

void LayerInputPlayer::renderRuntime() {
  cProject->renderRuntime();
}

void LayerInputPlayer::renderEditing() {
  // Not supported
}

bool LayerInputPlayer::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      switch (event.key.keysym.sym) {
        case SDLK_ESCAPE: {
          cRecording->close();
          cModule->quit();
          return true;
        }
      
        default: {
          // Nothing to do
        }        
      }
    }
  }
  return false;
}

bool LayerInputPlayer::inputEditor(SDL_Event& event) {
  return false;
}

ElementPickRay* LayerInputPlayer::getPickRay(float x, float y) {
  return nullptr; // Not supported
}

void LayerInputPlayer::initRuntime() {
}

void LayerInputPlayer::initEditor() {
  // Not supported
}

void LayerInputPlayer::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resources) {
  // Not supported
}

void LayerInputPlayer::staticChanged() {
}

void LayerInputPlayer::resourceSelected(IElementType* elementType) {
  // Not supported
}

void LayerInputPlayer::reset() {
  // Not supported
}

void LayerInputPlayer::addObjectSelectionListener(IObjectSelectionListener* listener) {
  // Not supported
}
