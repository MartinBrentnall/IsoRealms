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
#include "OptionMenuItem.h"

OptionMenuItem::OptionMenuItem(const std::string& name, std::vector<IOption*> options, unsigned int selected) {
  cName = name;
  cOptions = options;
  cSelectedOption = selected;
  cPreviousOption = selected;
}

void OptionMenuItem::apply() {
  cOptions[cSelectedOption]->apply();
}

void OptionMenuItem::reset() {
  cSelectedOption = cPreviousOption;
  apply();
}

void OptionMenuItem::save() {
  cPreviousOption = cSelectedOption;
}

void OptionMenuItem::render(int i, float fade, IFont* font, bool selected) {
  std::string mOptionText = cOptions[cSelectedOption]->getOption();
  glColor4f(0.0f, 0.0f, 0.0f, fade);
  font->print(-0.492f, (-0.10f * i) - 0.008f, 0.05f, 0, cName.c_str());
  font->print(0.508f, (-0.10f * i) - 0.008f, 0.05f, 2, mOptionText.c_str());
  if (selected) {
    glColor4f(0.6f, 1.0f, 0.4f, fade);
  } else {
    glColor4f(1.0f, 1.0f, 1.0f, fade);
  }
  font->print(-0.5f, -0.10f * i, 0.05f, 0, cName.c_str());
  font->print(0.5f, -0.10f * i, 0.05f, 2, mOptionText.c_str());
}

FocusAction OptionMenuItem::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_UP: {
      return PREVIOUS;
    }

    case SDLK_DOWN: {
      return NEXT;
    }

    case SDLK_LEFT: {
      if (cSelectedOption-- == 0) {
        cSelectedOption = cOptions.size() - 1;
      }
      break;
    }

    case SDLK_RIGHT:
    case SDLK_RETURN: {
      if (++cSelectedOption == cOptions.size()) {
        cSelectedOption = 0;
      }
      break;
    }

    default: {
      // Nothing to do.
    }
  }
  return NOTHING;
}

