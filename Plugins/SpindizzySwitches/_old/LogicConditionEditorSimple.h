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
#ifndef LOGIC_CONDITION_EDITOR_SIMPLE
#define LOGIC_CONDITION_EDITOR_SIMPLE

#include "LogicConditionEditor.h"

/**
 * This implementation provides simple functionality for selecting conditions
 * whereby one or two switches are required to be true or false for
 * satisfication.
 * <br>
 * Currently, it is only suitable for editing conditions based on the
 * SpindizzySwitchLogic implementation.  It is not suitable for more complex
 * conditions that may be possible with other SwitchLogic implementations.
 */
class LogicConditionEditorSimple:public LogicConditionEditor {
  public:

  /**************************************************************************\
   * Implemented methods of Editor.h                                        *
  \**************************************************************************/
  bool processInputEvent(SDL_Event&);
  void render();
}

#endif
