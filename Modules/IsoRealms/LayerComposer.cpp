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
#include "LayerComposer.h"

void LayerComposer::transitionLayer(unsigned int) {
}

void LayerComposer::updateRuntime(unsigned int milliseconds) {
  cLayers[0]->updateRuntime(milliseconds);
}

void LayerComposer::updateEditing(unsigned int milliseconds) {
  cLayers[0]->updateEditing(milliseconds);
}

void LayerComposer::renderRuntime() {
  for (unsigned int i = 0; i < cLayers.size(); i++) {
    cLayers[i]->renderLayer();
  }
}

void LayerComposer::renderEditing() {
  cLayers[0]->renderEditing();
}

bool LayerComposer::input(SDL_Event& event) {
  cLayers[0]->input(event);
}

bool LayerComposer::inputEditor(SDL_Event& event) {
  return false; // TODO?
}

void LayerComposer::initRuntime() {
  cLayers[0]->initRuntime();
}

void LayerComposer::save(DOMNodeWriter* node, IResourceLocator* resourceLocator) {
  // TODO:
}

void LayerComposer::staticChanged() {
  cLayers[0]->staticChanged();
}

