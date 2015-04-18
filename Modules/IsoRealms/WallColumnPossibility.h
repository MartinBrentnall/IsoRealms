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
#ifndef WALL_COLUMN_POSSIBILITY_H
#define WALL_COLUMN_POSSIBILITY_H

#include <vector>

#include <IsoRealms/Condition.h>

#include "WallColumn.h"

class WallColumnPossibility {
  private:
  std::vector<WallColumn*> cSegments;
  Condition* cCondition;

  WallColumnPossibility(WallColumnPossibility*, Condition*);

  public:
  WallColumnPossibility();
  WallColumnPossibility(WallColumn*, Condition*);
  
  std::vector<WallColumn*> getSections();
  void applyOverlapping(WallColumnPossibility*);
  void unite(WallColumnPossibility*);
  WallColumnPossibility* split(Condition*);
  bool isCompatibleWith(Condition*);
  Condition* getCondition();
  bool empty();
  void convertToAddition();
  void shaveTop(int, Condition*);
  bool isSubtraction();
  void removeHiddenSections(WallColumnPossibility*);
  
  void debug();
  
  ~WallColumnPossibility();
};

#endif
