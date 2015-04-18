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
#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <string>
#include <vector>

class Utils {
  public:

  /**
   * Convert the specified integer value to a string.
   */
  static std::string toString(int);
  static std::string toString(unsigned int);

  /**
   * Convert the specified float value to a string.
   */
  static std::string toString(float);
  
  static std::vector<std::string> split(const std::string&, char = '\n');  
  static std::vector<std::string> splitWords(const std::string&, char = ' ');
};

#endif
