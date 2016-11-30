/*
 * Copyright 2016 Martin Brentnall
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
#include "LayerHighScore.h"

LayerHighScore::LayerHighScore(IModuleHighScore* module) {
  cModule = module;
  cProject = module->getProject();
  cNotifiedProjectComplete = false;
}

void LayerHighScore::updateRuntime(unsigned int milliseconds) {
  if (cProject->hasCompleted()) {
    if (!cNotifiedProjectComplete) {
      cModule->projectCompleted();
      cNotifiedProjectComplete = true;
    }
    return;
  }
  cProject->updateRuntime(milliseconds);
}

void LayerHighScore::updateEditing(unsigned int milliseconds) {
  // Not supported
}

void LayerHighScore::renderRuntime() {
  cProject->renderRuntime();
}

void LayerHighScore::renderEditing() {
  // Not supported
}

bool LayerHighScore::input(SDL_Event& event) {
  cProject->inputRuntime(event);
  return true;
}

bool LayerHighScore::inputEditor(SDL_Event& event) {
  return false;
}

ElementPickRay* LayerHighScore::getPickRay(float x, float y) {
  return nullptr; // Not supported
}

void LayerHighScore::initRuntime() {
//  cProject->initRuntime();
}

void LayerHighScore::initEditor() {
  // Not supported
}

void LayerHighScore::save(DOMNodeWriter* node, DOMNodeWriter* cache, IResourceLocator* resources) {
  // Not supported
}

void LayerHighScore::staticChanged() {
}

void LayerHighScore::resourceSelected(IElementType* elementType) {
  // Not supported
}

void LayerHighScore::reset() {
  // Not supported
}

void LayerHighScore::addObjectSelectionListener(IObjectSelectionListener* listener) {
  // Not supported
}
