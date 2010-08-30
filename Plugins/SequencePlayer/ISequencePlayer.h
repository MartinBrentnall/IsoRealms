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
#ifndef I_SEQUENCE_PLAYER_H
#define I_SEQUENCE_PLAYER_H

#include <IsoRealms/IPlugin.h>

#include "ISequence.h"

/**
 * The Player interface provides two primary functions.  Firstly, it provides
 * a mechanism with which to register sequences for playback, and secondly, it
 * provides the interface to control playback of those registered sequences.
 */
class ISequencePlayer:public virtual IPlugin {
  public:
  
  /**
   * Register the specified sequence to be controlled by this player.  It is
   * possible to register a sequence to multiple players at the same time, but
   * the behaviour of the sequence is undefined in this case.
   *
   * @param ISequence*  The sequence to register with this player.
   */
  virtual void addSequence(ISequence*) = 0;
  
  /**
   * Unregister the specified sequence from this player.
   *
   * @param ISequence*  The sequence to unregister with this player.
   */
  virtual void removeSequence(ISequence*) = 0;
  
  /**
   * Begin playback of sequences controlled by this player.  Playback should
   * begins at 0.0 if this function is called for the first time, otherwise
   * playback should begin at the last playback position.  If the position is
   * 1.0, this function has no effect.
   */
  virtual void play() = 0;
  
  /**
   * Rewind the sequences controlled by this player.  Rewind should begin at the
   * last playback position.  If the position is 0.0 (or playback has not yet
   * occurred the sequence), this function has no effect.
   */
  virtual void rewind() = 0;
  
  /**
   * Pause playback at the current position.
   */
  virtual void pause() = 0;
  
  /**
   * Resume the last playback operation from the last playback position.
   */
  virtual void unpause() = 0;
};

#endif
