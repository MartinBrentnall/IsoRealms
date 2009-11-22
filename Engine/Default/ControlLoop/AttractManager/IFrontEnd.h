/*
 * Copyright 2009 Martin Brentnall
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
#ifndef I_FRONT_END_H
#define I_FRONT_END_H

#include <SDL/SDL.h>

#include "../../../../Global/DOMNodeWrapper.h"

/**
 * A front-end allows the user to control the engine by executing various
 * commands on it through an interface.
 */
class IFrontEnd {
  public:

  /**
   * Update the front-end for the specified number of milliseconds.
   *
   * @param int  Milliseconds to update.
   */
  virtual void update(int) = 0;

  /**
   * Render the front-end.
   */
  virtual void render() = 0;

  /**
   * Notify the front-end of activity and inactivity.
   *
   * @param bool  true if the front-end has become active, otherwise false.
   */
  virtual void setActive(bool) = 0;

  /**
   * Forward user input events to the front-end.
   *
   * @param SDL_Event&  The event to forward.
   */
  virtual void input(SDL_Event&) = 0;

  /**
   * Determine when the front-end wants the engine to terminate.
   */
  virtual bool hasExited() = 0;

  /**
   * Determine when the front-end wants to be de-activated.
   *
   * TODO: Maybe this should be controlled via the engine, or should be genericised (named) for de-activation (rather than idled)?
   */
  virtual bool hasIdled() = 0;

  /**
   * Destroy the front end.
   */
  virtual ~IFrontEnd() {}
};

typedef IFrontEnd* createFrontEnd(DOMNodeWrapper*);
typedef void destroyFrontEnd(IFrontEnd*);

#endif
