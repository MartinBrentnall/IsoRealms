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
#ifndef MISC_FUNCTIONS_H
#define MISC_FUNCTIONS_H

float restrict(float low, float high, float val);
int restrict(int low, int high, int val);
float min(float valA, float valB);
float max(float valA, float valB);
int min(int valA, int valB);
int max(int valA, int valB);
int max(int valA, int valB, int valC);
float sine(float min, float max, float gradient);

#endif
