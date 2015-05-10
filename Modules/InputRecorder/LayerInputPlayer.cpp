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

void LayerInputPlayer::updateRuntime(unsigned int milliseconds) {
}

void LayerInputPlayer::updateEditing(unsigned int milliseconds) {
  // Not supported
}

void LayerInputPlayer::renderRuntime() {
}

void LayerInputPlayer::renderEditing() {
  // Not supported
}

void LayerInputPlayer::input(SDL_Event& event) {
}

void LayerInputPlayer::initRuntime() {
}

void LayerInputPlayer::initEditor() {
  // Not supported
}

void LayerInputPlayer::save(DOMNodeWriter* node, IResourceLocator* resources) {
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
