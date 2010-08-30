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
#include "DummySequencePlayer.h"

void DummySequencePlayer::addSequence(ISequence*) {
  // Nothing to do.
}

void DummySequencePlayer::removeSequence(ISequence*) {
  // Nothing to do.
}

void DummySequencePlayer::play() {
  // Nothing to do.
}

void DummySequencePlayer::rewind() {
  // Nothing to do.
}

void DummySequencePlayer::pause() {
  // Nothing to do.
}

void DummySequencePlayer::unpause() {
  // Nothing to do.
}

extern "C" IPlugin* create() {
  return new DummySequencePlayer();
}

extern "C" void destroy(IPlugin* plugin) {
  delete plugin;
}
