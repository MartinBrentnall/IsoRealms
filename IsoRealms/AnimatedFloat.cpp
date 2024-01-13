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
#include "AnimatedFloat.h"
 
namespace IsoRealms {
  AnimatedFloat::AnimatedFloat() {
    init(0.0f);
  }
  
  AnimatedFloat::AnimatedFloat(float value) {
    init(value);
  }
  
  float AnimatedFloat::value() const {
    return cValue;
  }
  
  float AnimatedFloat::animation() const {
    return cAnimation;
  }
  
  bool AnimatedFloat::update(unsigned int milliseconds) {
    float mOldValue = cAnimation;
    if (cAnimation != cValue) {
      float mPotentialMovement = (static_cast<float>(cValue) - cAnimation) * 0.015f * milliseconds;
      cAnimation += mPotentialMovement;
      float mDifference = std::abs(mOldValue - cAnimation);
      if (mDifference < 0.0005f) {
        cAnimation = cValue;
      }
    }
    return cAnimation == cValue;
  }

  void AnimatedFloat::init(const float value) {
    cValue     = value;
    cAnimation = value;
  }
  
  AnimatedFloat& AnimatedFloat::operator=(float value) {
    cValue = value;
    return *this;
  }  

  AnimatedFloat& AnimatedFloat::operator+=(float value) {
    cValue += value;
    return *this;
  }

  AnimatedFloat& AnimatedFloat::operator-=(float value) {
    cValue -= value;
    return *this;
  }
}
