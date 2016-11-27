/*
 * Copyright 2009,2010,2011 Martin Brentnall
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
#include "BooleanMenuItem.h"

BooleanMenuItem::BooleanMenuItem(const std::string& name, const std::string& trueValue, const std::string& falseValue) {
  cName = name;
  cTrueValue = trueValue;
  cFalseValue = falseValue;
  cValue = false; // TODO: Initialise this
}

void BooleanMenuItem::render(int i, float fade, IFont* font, bool selected) {
  std::string mValue = cValue ? cTrueValue : cFalseValue;
  glColor4f(0.0f, 0.0f, 0.0f, fade);
  font->print(-0.492f, (-0.10f * i) - 0.008f, 0.05f, IFont::LEFT, cName.c_str());
  font->print(0.508f, (-0.10f * i) - 0.008f, 0.05f, IFont::RIGHT, mValue.c_str());
  if (selected) {
    glColor4f(0.6f, 1.0f, 0.4f, fade);
  } else {
    glColor4f(1.0f, 1.0f, 1.0f, fade);
  }
  font->print(-0.5f, -0.10f * i, 0.05f, IFont::LEFT, cName.c_str());
  font->print(0.5f, -0.10f * i, 0.05f, IFont::RIGHT, mValue.c_str());
}

FocusAction BooleanMenuItem::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_UP: {
      return PREVIOUS;
    }

    case SDLK_DOWN: {
      return NEXT;
    }

    case SDLK_LEFT:
    case SDLK_RIGHT:
    case SDLK_RETURN: {
      cValue = !cValue;
      break;
    }

    default: {
      // Nothing to do.
    }
  }
  return NOTHING;
}

