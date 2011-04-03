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
#ifndef I_ATTRACT_H
#define I_ATTRACT_H

#include <IsoRealms/IFont.h>

/**
 * An attraction is a non-interactive animated scene that is intended to draw
 * the attention of the user.  The attract scene will play via continuous calls
 * to its render() function until the hasFinished() function returns true.
 * <br>
 * An attract scene can be anything that you wish to show the user before he
 * starts playing the game.
 */
class IAttract {
  public:

  /**
   * This method is called when the scene is about to start.
   */
  virtual void init() = 0;

  /**
   * Render the attract scene.  This method will be called continuously from
   * the time at which the attraction starts.
   */
  virtual void render() = 0;

  /**
   * Update the attract scene.  This method will be called continuously from
   * the time at which the attraction starts.
   *
   * @param int  Ticks passed since last frame.
   */
  virtual void update(int ticks) = 0;

  /**
   * Determine whether the attract mode has completed.  After this method
   * returns true, the render() method will not be called again until this
   * attraction is restarted by the engine.
   *
   * @returns  true if the attract mode has completed, otherwise false.
   */
  virtual bool hasFinished() = 0;

  /**
   * This method is called when the front-end is invoked or removed. where it
   * may be desirable to change certain behaviour of the scene (e.g. to remove
   * text that might overlap the front-end).
   */
  virtual void frontEndActive(bool) = 0;

  /**
   * Destructor.
   */
  virtual ~IAttract() {}
};

typedef IAttract* createAttract(IFont*);
typedef void destroyAttract(IAttract*);

#endif
