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
#include "NumberInputEditor.h"

NumberInputEditor::NumberInputEditor(string& prompt) {
  cPrompt = prompt;
  cConfirmed = false;
  cCancelled = false;
}

bool NumberInputEditor::processInputEvent(SDL_Event& event) {
  if (!cConfirmed && !cCancelled) {

    // TODO: Should allow float values?
    // TODO: Should allow arrow keys, insert in middle etc.
    if (event.key.keysym.sym == SDLK_RETURN) {
      cLiftStage = 0;
      for (int i = 0; i < MAX_KEYS; i++) {
        cKeyDown[i] = false;
      }
      cConfirmed = true;
    } else if (event.key.keysym.sym == SDLK_ESCAPE) {
      cCancelled = true;
    } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
      if (cUserInput.length() > 0) {
        cUserInput.resize(cUserText.length() - 1);
      }
    } else if (!cKeyDown[event.key.keysym.sym]) {
      if (event.key.keysym.sym >= '0' && event.key.keysym.sym <= '9') {
        cKeyDown[event.key.keysym.sym] = true;
        char mKey[2];
        mKey[0] = (char) event.key.keysym.sym;
        mKey[1] = '\0';
        string mChar(mKey);
        cUserInput.append(mChar);
      }
    }
  }
}

void NumberInputEditor::render() {
  if (!cConfirmed && !cCancelled) {
  
    // TODO: Should render cursor
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    string mCompleteText = cPrompt + cUserInput;
    glColor3f(0.0, 0.0, 0.0);
    freetype::print(cFont, 105, 95, mCompleteText.c_str());
    glColor3f(1.0, 1.0, 1.0);
    freetype::print(cFont, 100, 100, mCompleteText.c_str());
    glEnable(GL_DEPTH_TEST);
  }
}

int NumberInputEditor::getValue() {
  return atoi(cUserInput.c_str());
//     cEditingLift->setUpDelay(atoi(cUserText.c_str()));
//     cEditingLift->setDownDelay(atoi(cUserText.c_str()));
}

bool NumberInputEditor::hasConfirmed() {
  return cConfirmed;
}

bool NumberInputEditor::hasCancelled() {
  return cCancelled;
}
