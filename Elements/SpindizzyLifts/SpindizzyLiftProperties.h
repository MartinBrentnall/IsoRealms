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
#ifndef SPINDIZZY_LIFT_PROPERTIES_H
#define SPINDIZZY_LIFT_PROPERTIES_H

#include "../../Global/DOMNodeWrapper.h"

class SpindizzyLiftProperties {
  private:
  unsigned int cTopDelay;
  unsigned int cBottomDelay;
  unsigned int cUpSpeed;
  unsigned int cDownSpeed;

  public:
  SpindizzyLiftProperties();

  unsigned int getTopDelay();
  unsigned int getBottomDelay();
  unsigned int getUpSpeed();
  unsigned int getDownSpeed();

  void setTopDelay(unsigned int);
  void setBottomDelay(unsigned int);
  void setUpSpeed(unsigned int);
  void setDownSpeed(unsigned int);

  void setup(DOMNodeWrapper*);

  void reset();
};

#endif
