/*
 * Copyright 2023 Martin Brentnall
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
#pragma once

namespace IsoRealms {

  /**
   * Interface to be implemented by those wishing to be notified of changes to
   * the state of an object.
   */
  template<class T> class IStateListener {
    public:
      
    /**
     * The function is to be called when the state of an object to which this
     * listener has been registered changes.  The object whose state has
     * changed is passed to the listener, which is useful in case the listener
     * is registered to multiple objects.
     * 
     * @param object The object whose state changed.
     */
    virtual void stateChanged(T object) = 0;
  };
}
