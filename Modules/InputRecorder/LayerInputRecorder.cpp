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

  // Construct date and time string for filename
  time_t mCurrentTime = time(0);
  struct tm* mNow = localtime(&mCurrentTime);
  std::stringstream mDateTimeReader;
  mDateTimeReader << std::setfill('0') << std::setw(2) << (mNow->tm_year - 100)
                                       << std::setw(2) << (mNow->tm_mon + 1) 
                                       << std::setw(2) <<  mNow->tm_mday << "-"
                                       << std::setw(2) <<  mNow->tm_hour
                                       << std::setw(2) <<  mNow->tm_min
                                       << std::setw(2) <<  mNow->tm_sec;
  std::string mFullPath = System::getUserResource("Recordings/"); 
  cModule->setFilename("Recordings/" + mDateTimeReader.str());
  
  // Make the directory if it doesn't exist yet
  System::makeDirectory(mFullPath);
  mFullPath += "/" + mDateTimeReader.str();
                                       
  // Combine the path and name, then open the file
  std::cout << "Writing to: " << mFullPath << std::endl;
  cRecordingFile.open(mFullPath);
  
  // Write the Project name first
  std::string mProjectName = cProject->getFileName();
  cRecordingFile << mProjectName << std::endl;
  
  // Write input mapping
  InputCommands* mInputCommands = cProject->getInputConfiguration();
  std::map<std::string, DigitalInput*> mDigitalInputs = mInputCommands->getDigitalInputs();
  unsigned int mInputID = 0;
  for (std::pair<std::string, DigitalInput*> mDigitalInput : mDigitalInputs) {
    if (mInputID > 0) {
      cRecordingFile << ",";
    }
    cRecordingFile << mInputID << "=\"" << mDigitalInput.first << "\"";
    bool* mInput = mDigitalInput.second->getDigitalInput();
    cInputs[mInput] = new InputState(mDigitalInput.first, mInputID);
    mInputID++;
  }
  cRecordingFile << std::endl;
}

void LayerInputRecorder::updateRuntime(unsigned int milliseconds) {
  for (std::pair<bool*, InputState*> mInputState : cInputs) {
    if (*(mInputState.first) != mInputState.second->cState) {
      mInputState.second->cState = *(mInputState.first);
      cRecordingFile << mInputState.second->cID << "=" << mInputState.second->cState << "@" << cElapsedTime << std::endl;      
      cRecordingFile.flush();
    }
  }
  
  if (cProject->hasCompleted()) {
    cRecordingFile.close();
    cModule->quit();
    return;
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

LayerInputRecorder::InputState::InputState(const std::string& name, unsigned int id) {
  cName = name;
  cState = false;
  cID = id;
}
