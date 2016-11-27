/*
 * Copyright 2015 Martin Brentnall
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
#include "TestScreen.h"

TestScreen::TestScreen(std::vector<IApplicableItem*> applicableItems, IMenuStack* menuStack) {
  cApplicableItems = applicableItems;
  cConfirmed = false;
  cMenuStack = menuStack;
  cTimeOut = 15000;
}

bool TestScreen::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_LEFT: {
      cConfirmed = false;
      return true;
    }

    case SDLK_RIGHT: {
      cConfirmed = true;
      return true;
    }

    case SDLK_RETURN: {
      for (unsigned int i = 0; i < cApplicableItems.size(); i++) {
        if (cConfirmed) {
          cApplicableItems[i]->save();
        } else {
          cApplicableItems[i]->reset();
        }
      }
      cMenuStack->pop();
      break;
    }

    default: {
      // Nothing to do.
    }
  }
  return false;
}

bool TestScreen::input(SDL_Event& event) {  
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }
  }
  return false;
}

void TestScreen::render(float fade, IFont* font) {
  glEnable(GL_BLEND);
  glColor4f(0.0f, 0.0f, 0.0f, fade);
  std::stringstream mString;
  mString << "Are these settings OK? (" << (cTimeOut / 1000) << ")";
  std::string mMessage = mString.str();
  font->print( 0.008f, (-0.10f * 0) - 0.008f, 0.05f, IFont::CENTER, mMessage.c_str());
  font->print(-0.492f, (-0.10f * 1) - 0.008f, 0.05f, IFont::LEFT, "No");
  font->print( 0.508f, (-0.10f * 1) - 0.008f, 0.05f, IFont::RIGHT, "Yes");

  glColor4f(1.0f, 1.0f, 1.0f, fade);
  font->print( 0.0f, -0.10f * 0, 0.05f, IFont::CENTER, mMessage.c_str());
  if (!cConfirmed) {
    glColor4f(0.6f, 1.0f, 0.4f, fade);
  } else {
    glColor4f(1.0f, 1.0f, 1.0f, fade);
  }
  font->print(-0.5f, -0.10f * 1, 0.05f, IFont::LEFT, "No");
  if (cConfirmed) {
    glColor4f(0.6f, 1.0f, 0.4f, fade);
  } else {
    glColor4f(1.0f, 1.0f, 1.0f, fade);
  }
  font->print( 0.5f, -0.10f * 1, 0.05f, IFont::RIGHT, "Yes");
  glDisable(GL_BLEND);
}

void TestScreen::update(unsigned int ticks) {
  cTimeOut -= ticks;
  if (cTimeOut <= 0) {
    cTimeOut = 0;
    cancelled();
    cMenuStack->pop();
  }
}

void TestScreen::cancelled() {
  for (unsigned int i = 0; i < cApplicableItems.size(); i++) {
    cApplicableItems[i]->reset();
  }    
}

