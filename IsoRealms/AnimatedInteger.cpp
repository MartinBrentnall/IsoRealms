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
#include "AnimatedInteger.h"

namespace IsoRealms {
  AnimatedInteger::AnimatedInteger() {
    init(0.0f);
  }

  AnimatedInteger::AnimatedInteger(int value) {
    init(value);
  }

  int AnimatedInteger::value() const {
    return cValue;
  }

  double AnimatedInteger::animation() const {
    return cAnimation;
  }

  bool AnimatedInteger::update(unsigned int milliseconds) {
    double mOldValue = cAnimation;
    if (cAnimation != cValue) {
      double mPotentialMovement = (static_cast<double>(cValue) - cAnimation) * 0.015f * milliseconds;
      cAnimation += mPotentialMovement;
      double mDifference = std::abs(mOldValue - cAnimation);
      if (mDifference < 0.0005f) {
        cAnimation = cValue;
      }
    }
    return cAnimation == cValue;
  }

  void AnimatedInteger::init(const int value) {
    cValue     = value;
    cAnimation = value;
  }

  AnimatedInteger& AnimatedInteger::operator=(int value) {
    cValue = value;
    return *this;
  }

  AnimatedInteger& AnimatedInteger::operator+=(int value) {
    cValue += value;
    return *this;
  }

  AnimatedInteger& AnimatedInteger::operator-=(int value) {
    cValue -= value;
    return *this;
  }
}
