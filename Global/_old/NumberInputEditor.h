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
#ifndef NUMBER_INPUT_EDITOR
#define NUMBER_INPUT_EDITOR

#include "Editor.h"

#include <string>

class NumberInputEditor:public Editor {
  private:
  string cPrompt;
  string cUserInput;
  bool cConfirmed;
  bool cCancelled;

  public:
  NumberInputEditor(string& prompt);
  bool hasConfirmed();
  bool hasCancelled();
  int getValue();

  /**************************************************************************\
   * Implemented methods of Editor.h                                        *
  \**************************************************************************/
  bool processInputEvent(SDL_Event&);
  void render();
};

#endif
