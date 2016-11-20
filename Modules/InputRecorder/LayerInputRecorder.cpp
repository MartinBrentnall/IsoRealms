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

void LayerInputRecorder::updateRuntime(unsigned int milliseconds) {
}

void LayerInputRecorder::updateEditing(unsigned int milliseconds) {
  // Not supported
}

void LayerInputRecorder::renderRuntime() {
}

void LayerInputRecorder::renderEditing() {
  // Not supported
}

bool LayerInputRecorder::input(SDL_Event& event) {
  return false;
}

bool LayerInputRecorder::inputEditor(SDL_Event& event) {
  return false;
}

void LayerInputRecorder::initRuntime() {
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

