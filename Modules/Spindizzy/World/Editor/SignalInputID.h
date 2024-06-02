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
#pragma once

namespace IsoRealms::Spindizzy {
  enum class SignalInputID {
    CANCEL,
    CONFIGURE_TOOL,
    EXIT,
    MOVE_CURSOR_BACKWARD,
    MOVE_CURSOR_DOWN,
    MOVE_CURSOR_FASTER,
    MOVE_CURSOR_FORWARD,
    MOVE_CURSOR_LEFT,
    MOVE_CURSOR_RIGHT,
    MOVE_CURSOR_SLOWER,
    MOVE_CURSOR_UP,
    MOVE_VIEW,
    NEXT_THEME,
    NEXT_TOOL,
    PREVIOUS_THEME,
    PREVIOUS_TOOL,
    ROTATE_VIEW,
    TOOL_MODE,
    USE_TOOL
  };
}

