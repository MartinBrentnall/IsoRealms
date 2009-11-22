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
#ifndef CONDITION_EDITOR_H
#define CONDITION_EDITOR_H

#include "Condition.h"
#include "SpindizzyConstants.h"
#include "MiscFunctions.h"
#include "TextureSet.h"

class ConditionEditor:public SpindizzyConstants {
  private:
  static const int NOT = 0;
  static const int SYMBOL_A = 1;
  static const int SYMBOL_B = 2;

  static TextureSet* cTextures;

  bool cNot;
  int cSymbolA;
  int cSymbolB;
  int cFocus;

  public:
  static void setTextures(TextureSet* textures);
  ConditionEditor();
  Condition* getCondition();
  void move(int x, int y, int z);
  void render();
};

#endif
