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
#include "LayerInputRecorder.h"

LayerInputRecorder::LayerInputRecorder(IModuleInputPersistence* module) {
  cModule = module;
  cProject = module->getProject();
  cElapsedTime = 0;
  InputCommands* mInputCommands = cProject->getInputConfiguration();
  std::map<std::string, DigitalInput*> mDigitalInputs = mInputCommands->getDigitalInputs();
  for (std::pair<std::string, DigitalInput*> mDigitalInput : mDigitalInputs) {
    bool* mInput = mDigitalInput.second->getDigitalInput();
    cInputs[mInput] = new InputState(mDigitalInput.first);
  }
}

void LayerInputRecorder::updateRuntime(unsigned int milliseconds) {
  if (cProject->hasCompleted()) {
    DOMNodeWriter* mRecordingNode = new DOMNodeWriter("InputRecording");
    for (InputEvent* mEvent : cRecordedEvents) {
      DOMNodeWriter* mEventNode = mRecordingNode->addBranch("Event");
      mEventNode->addAttribute("name", cInputs[mEvent->getInput()]->cName);
      mEventNode->addAttribute("time", mEvent->getTime());
      mEventNode->addAttribute("on", mEvent->getState());
    }
    std::string mFileName = cProject->getFileName();
    std::string mRecordingDirectory = mFileName.substr(0, mFileName.length() - 10);
    System::makeDirectory(mRecordingDirectory);
    mRecordingNode->save(mRecordingDirectory + "/recording.recording");
    cModule->quit();
    return;
  }
  
  for (std::pair<bool*, InputState*> mInputState : cInputs) {
    if (*(mInputState.first) != mInputState.second->cState) {
      cRecordedEvents.push_back(new InputEvent(mInputState.first, *(mInputState.first), cElapsedTime));
      mInputState.second->cState = *(mInputState.first);
    }
  }
  cElapsedTime += milliseconds;
  cProject->updateRuntime(milliseconds);
}

void LayerInputRecorder::updateEditing(unsigned int milliseconds) {
  // Not supported
}

void LayerInputRecorder::renderRuntime() {
  cProject->renderRuntime();
}

void LayerInputRecorder::renderEditing() {
  // Not supported
}

bool LayerInputRecorder::input(SDL_Event& event) {
  cProject->inputRuntime(event);
  return true;
}

bool LayerInputRecorder::inputEditor(SDL_Event& event) {
  return false;
}

ElementPickRay* LayerInputRecorder::getPickRay(float x, float y) {
  return nullptr; // Not supported
}

void LayerInputRecorder::initRuntime() {
//  cProject->initRuntime();
}

void LayerInputRecorder::initEditor() {
  // Not supported
}

void LayerInputRecorder::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resources) {
  // Not supported
}

void LayerInputRecorder::staticChanged() {
}

void LayerInputRecorder::resourceSelected(IElementType* elementType) {
  // Not supported
}

void LayerInputRecorder::reset() {
  // Not supported
}

void LayerInputRecorder::addObjectSelectionListener(IObjectSelectionListener* listener) {
  // Not supported
}

LayerInputRecorder::InputState::InputState(const std::string& name) {
  cName = name;
  cState = false;
}
