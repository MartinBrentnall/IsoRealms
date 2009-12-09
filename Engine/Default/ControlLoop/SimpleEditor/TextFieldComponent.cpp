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
#include "TextFieldComponent.h"

IFont* TextFieldComponent::cFont = NULL;

void TextFieldComponent::setFont(IFont* font) {
  cFont = font;
}

TextFieldComponent::TextFieldComponent(IRectangularComponent* relative, IRectangularComponent::Edge edge, float offset) {
  cRelative = relative;
  cEdge = edge;
  cOffset = offset;
  cCaret = 0;
}

void TextFieldComponent::render() {
  float mLeft = getLeft();
  float mBottom = getBottom();
  float mRight = getRight();
  float mTop = getTop();
  cFont->print(mLeft + 0.01f, mBottom + 0.01f, 0.02f, 0, cInput.c_str());

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINE_LOOP);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(mLeft,  mTop);
  glVertex2f(mLeft,  mBottom);
  glVertex2f(mRight, mBottom);
  glVertex2f(mRight, mTop);
  glEnd();

  float mCaretOffset = cFont->getWidth(0.02f, cInput.substr(0, cCaret).c_str());
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);
  glVertex2f(mLeft + 0.01f + mCaretOffset, mBottom - 0.01f);
  glVertex2f(mLeft + 0.01f + mCaretOffset, mBottom + 0.04f);
  glEnd();
}

void TextFieldComponent::update(int milliseconds) {
}

bool TextFieldComponent::input(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym, event.key.keysym.mod);
    }
  }
  // TODO: Mouse press to change caret position
  return false;
}

bool TextFieldComponent::keyDown(SDLKey& key, SDLMod& mod) {
  switch (key) {
    case SDLK_LEFT: {
      if (cCaret != 0) {
        cCaret--;
      }
      break;
    }

    case SDLK_RIGHT: {
      cCaret++;
      if (cCaret > cInput.size()) {
        cCaret = cInput.size();
      }
      break;
    }

    case SDLK_HOME: {
      cCaret = 0;
      break;
    }

    case SDLK_END: {
      cCaret = cInput.length();
      break;
    }

    case SDLK_BACKSPACE: {
      if (cCaret > 0) {
        cInput = cInput.substr(0, cCaret - 1) + cInput.substr(cCaret);
        cCaret--;
      }
      break;
    }

    case SDLK_DELETE: {
      if (cCaret < cInput.length()) {
        cInput = cInput.substr(0, cCaret) + cInput.substr(cCaret + 1);
      }
      break;
    }

    default: {
      if (key >= 32 && key <= 127 && cInput.length() < 40) {
        if (mod & KMOD_SHIFT) {
          cInput = cInput.substr(0, cCaret) + (char) toupper((char) key) + cInput.substr(cCaret);
        } else {
          cInput = cInput.substr(0, cCaret) + (char) key + cInput.substr(cCaret);
        }
        cCaret++;
      }
      break;
    }
  }
  return false;
}

std::string TextFieldComponent::getText() {
  return cInput;
}
