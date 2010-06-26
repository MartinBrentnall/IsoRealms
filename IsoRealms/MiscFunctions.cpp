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
#include "MiscFunctions.h"

// TODO: Port to C++
float restrict(float low, float high, float val) {
  if (val < low) {
    return low;
  }
  if (val > high) {
    return high;
  }
  return val;
}

int restrict(int low, int high, int val) {
  if (val < low) {
    return low;
  }
  if (val > high) {
    return high;
  }
  return val;
}

float min(float valA, float valB) {
  return valA < valB ? valA : valB;
}

float max(float valA, float valB) {
  return valA > valB ? valA : valB;
}

int min(int valA, int valB) {
  return valA < valB ? valA : valB;
}

int max(int valA, int valB) {
  return valA > valB ? valA : valB;
}

int max(int valA, int valB, int valC) {
  if (valA > valB) {
    return valA > valC ? valA : valC;
  } else {
    return valB > valC ? valB : valC;
  }
}

float sine(float min, float max, float gradient) {
  float mMiddle = (max - min) * gradient + min;
  float mLowerMiddle = (mMiddle - min) * gradient + min;
  float mUpperMiddle = (max - mMiddle) * gradient + mMiddle;
  return (mUpperMiddle - mLowerMiddle) * gradient + mLowerMiddle;
}
