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
#ifndef I_SEQUENCE_H
#define I_SEQUENCE_H

/**
 * Entities wishing to be controlled via a Player plugin should implement this
 * interface.
 */
class ISequence {
  public:
  
  /**
   * This function is called during playback operation.  The function is not
   * called when the playback position is not changed (i.e. has played until the
   * end, has rewound until the beginning or is paused).
   *
   * @param position The playback position in the sequence.
   */
  virtual void update(float position) = 0;
};

#endif
