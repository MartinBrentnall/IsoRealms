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

TextFieldComponent::TextFieldComponent(IRectangularComponent* relative, IRectangularComponent::Edge edge, float offset, float width) {
  cRelative = relative;
  cEdge = edge;
  cOffset = offset;
  cWidth = width;
  cCaret = 0;
}

void TextFieldComponent::render() {
  float mX = getX();
  float mY = getY();
  float mWidth = getWidth();
  float mHeight = getHeight();
  cFont->print(mX + 0.01f, mY + 0.01f, 0.02f, 0, cInput.c_str());

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINE_LOOP);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(mX,          mY + mHeight);
  glVertex2f(mX,          mY);
  glVertex2f(mX + mWidth, mY);
  glVertex2f(mX + mWidth, mY + mHeight);
  glEnd();

  float mCaretOffset = cFont->getWidth(0.02f, cInput.substr(0, cCaret).c_str());
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);
  glVertex2f(getX() + 0.01f + mCaretOffset, getY() - 0.01f);
  glVertex2f(getX() + 0.01f + mCaretOffset, getY() + 0.04f);
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

float TextFieldComponent::getX() {
  switch (cEdge) {
    case TOP:    return cRelative->getX();
    case BOTTOM: return cRelative->getX();
    case LEFT:   return (cRelative->getX() - cOffset) - getWidth();
    case RIGHT:  return cRelative->getX() + cRelative->getWidth() + cOffset;
  }
  throw new IllegalStateException(); // TODO: Fill in
}

float TextFieldComponent::getY() {
  switch (cEdge) {
    case TOP:    return cRelative->getY() + cRelative->getHeight() + cOffset;
    case BOTTOM: return (cRelative->getY() - cOffset) - getHeight();
    case LEFT:   return cRelative->getY();
    case RIGHT:  return cRelative->getY();
  }
  throw new IllegalStateException(); // TODO Fill in
}

float TextFieldComponent::getWidth() {
  return cWidth;
}

float TextFieldComponent::getHeight() {
  return 0.05f;
}

bool TextFieldComponent::contains(float x, float y) {
  return x >= getX() && x <= getX() + getWidth() && y >= getY() && y <= getY() + getHeight();
}

std::string TextFieldComponent::getText() {
  return cInput;
}
