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

#include <iostream>

namespace IsoRealms {

  /**
   * Smoothly animate a integer value for graphics.
   */
  class AnimatedInteger {
    public:
    AnimatedInteger();
    AnimatedInteger(int value);

    int value() const;
    double animation() const;
    bool update(unsigned int milliseconds);
    void init(const int value);
    AnimatedInteger& operator=(int value);
    AnimatedInteger& operator+=(int value);
    AnimatedInteger& operator-=(int value);

    private:
    int cValue;
    double cAnimation;
  };
}
