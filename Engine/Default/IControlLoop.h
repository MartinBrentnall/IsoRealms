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
#ifndef I_CONTROL_LOOP_H
#define I_CONTROL_LOOP_H

#include <SDL/SDL.h>

#include "../../Global/DOMNodeWrapper.h"

/**
 * This interface is to be implemented by any class wishing to act as a control
 * loop for the engine.  The control function will be called continuously on
 * the current control loop implementation.
 */
class IControlLoop {
  public:
  virtual ~IControlLoop() {}

  /**
   * Input events are forwarded to the control loop via this method.  It will
   * be called by the engine for as many input events remain before the loop is
   * executed.
   *
   * @param SDL_Event&  User input event.
   */
  virtual void input(SDL_Event&) = 0;

  /**
   * Execute this control loop.  The specified number of milliseconds have
   * passed since the last execution.
   *
   * @param int  Milliseconds passed since last call.
   */
  virtual void execute(int) = 0;
};

typedef IControlLoop* createControlLoop(DOMNodeWrapper*);
typedef void destroyControlLoop(IControlLoop*);

#endif
