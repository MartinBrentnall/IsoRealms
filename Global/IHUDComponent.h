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
#ifndef I_HUD_COMPONENT_H
#define I_HUD_COMPONENT_H

#include <SDL/SDL.h>

class IHUDComponent {
  private:
//   static IFont* cFont;

  public:
//   static void setFont(IFont*);
//   static IFont* getFont();
  virtual ~IHUDComponent() {}

  virtual void update(int) = 0;
  virtual void render() = 0;
  virtual bool input(SDL_Event&) = 0;
  virtual bool contains(float, float) = 0;
};

#endif
