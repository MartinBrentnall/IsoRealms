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
#include "PaletteConfigurationComponent.h"

PaletteConfigurationComponent::PaletteConfigurationComponent(IComponentContainer* componentContainer, std::map<IFourColourSupport::PaletteEntry, Colour*> palette, std::vector<IFourColourSupportListener*>& listeners) : Dialog(componentContainer, "Palette Editor", 0.18f, 0.68f, 0.8f, 0.3f) {
  for (std::map<IFourColourSupport::PaletteEntry, Colour*>::iterator i = palette.begin(); i != palette.end(); i++) {
    cPaletteEntries.push_back(i->first);
    cPalette.push_back(i->second);
  }
  cChangeListeners = &listeners;
  cSelectedField = 0;
  cSelectedEntry = 0;
}

void PaletteConfigurationComponent::renderChannel(float x, float y, float width, float height, Colour& start, Colour& end, unsigned int field) {
  glBegin(GL_QUADS);
  start.set(); glVertex2f(x,          y);
               glVertex2f(x,          y - height);
  end.set();   glVertex2f(x + width,  y - height);
               glVertex2f(x + width,  y);
  glEnd();

  float mBrightness = field == cSelectedField ? 1.0f : 0.4f;
  glColor3f(1.0f * mBrightness, 1.0f * mBrightness, 1.0f * mBrightness);
  glBegin(GL_LINE_LOOP);
  glVertex2f(x,          y);
  glVertex2f(x,          y - height);
  glVertex2f(x + width,  y - height);
  glVertex2f(x + width,  y);
  glEnd();

  float mAmount;
  switch (field) {
    case 1: mAmount = cPalette[cSelectedEntry]->getRed(); break;
    case 2: mAmount = cPalette[cSelectedEntry]->getGreen(); break;
    case 3: mAmount = cPalette[cSelectedEntry]->getBlue(); break;
  }
  mAmount = x + width * mAmount;
  glColor3f(0.0f * mBrightness, 1.0f * mBrightness, 0.0f * mBrightness);
  glBegin(GL_LINES);
  glVertex2f(mAmount, y);
  glVertex2f(mAmount, y - height);
  glEnd();
}

void PaletteConfigurationComponent::renderContent() {
  glBindTexture(GL_TEXTURE_2D, 0);
  Configuration* mConfiguration = Configuration::getInstance();
  ScreenConfiguration* mScreenConfiguration = mConfiguration->getScreenConfiguration();
  float mAspectRatio = mScreenConfiguration->getAspectRatio();
  float mSpacing = 0.02f;
  float mX = getLeft() + mSpacing * mAspectRatio;
  float mY = getTop() - mSpacing;
  float mWidth = 0.05f * mAspectRatio;
  float mHeight = 0.05f;
  for (unsigned int i = 0; i < cPalette.size(); i++) {
    cPalette[i]->set();
    glBegin(GL_QUADS);
    glVertex2f(mX,          mY - mHeight);
    glVertex2f(mX + mWidth, mY - mHeight);
    glVertex2f(mX + mWidth, mY);
    glVertex2f(mX,          mY);
    glEnd();
    float mBrightness = cSelectedField == 0 ? 1.0f : 0.4f;
    if (cSelectedEntry == i) {
      glColor3f(1.0f * mBrightness, 1.0f * mBrightness, 1.0f * mBrightness);
    } else {
      glColor3f(0.3f * mBrightness, 0.3f * mBrightness, 0.3f * mBrightness);
    }
    glBegin(GL_LINE_LOOP);
    glVertex2f(mX,          mY - mHeight);
    glVertex2f(mX + mWidth, mY - mHeight);
    glVertex2f(mX + mWidth, mY);
    glVertex2f(mX,          mY);
    glEnd();
    mX += mWidth + mSpacing * mAspectRatio;
  }

  mX = getLeft() + mSpacing * mAspectRatio;
  mWidth = (getRight() - getLeft()) - mSpacing * 2.0f * mAspectRatio;

  Colour mMax = *cPalette[cSelectedEntry];
  Colour mMin = *cPalette[cSelectedEntry];
  mMax.change( 1.0,  0.0,  0.0,  0.0);
  mMin.change(-1.0,  0.0,  0.0,  0.0);
  mY -= mHeight + mSpacing;
  renderChannel(mX, mY, mWidth, mHeight, mMin, mMax, 1);

  mMax = *cPalette[cSelectedEntry];
  mMin = *cPalette[cSelectedEntry];
  mMax.change( 0.0,  1.0,  0.0,  0.0);
  mMin.change( 0.0, -1.0,  0.0,  0.0);
  mY -= mHeight + mSpacing;
  renderChannel(mX, mY, mWidth, mHeight, mMin, mMax, 2);

  mMax = *cPalette[cSelectedEntry];
  mMin = *cPalette[cSelectedEntry];
  mMax.change( 0.0,  0.0,  1.0,  0.0);
  mMin.change( 0.0,  0.0, -1.0,  0.0);
  mY -= mHeight + mSpacing;
  renderChannel(mX, mY, mWidth, mHeight, mMin, mMax, 3);
}

void PaletteConfigurationComponent::adjustChannel(float amount) {
  Colour* mColourToChange = cPalette[cSelectedEntry];
  switch (cSelectedField) {
    case 1: mColourToChange->change(amount, 0.0f, 0.0f, 0.0f); break;
    case 2: mColourToChange->change(0.0f, amount, 0.0f, 0.0f); break;
    case 3: mColourToChange->change(0.0f, 0.0f, amount, 0.0f); break;
  }
  fireChangeEvent();
}

void PaletteConfigurationComponent::fireChangeEvent() {
  for (unsigned int i = 0; i < cChangeListeners->size(); i++) {
    // TODO: DON'T FIRE NULL!
    (*cChangeListeners)[i]->fourColourPaletteChanged(NULL);
  }
}

void PaletteConfigurationComponent::updateContent(int) {
}

bool PaletteConfigurationComponent::keyDown(SDLKey& key) {
  switch (key) {
    case SDLK_ESCAPE: {
      close();
      return true;
    }

    case SDLK_DOWN: {
      if (cSelectedField < 3) {
        cSelectedField++;
      }
      return true;
    }
 
    case SDLK_UP: {
      if (cSelectedField > 0) {
        cSelectedField--;
      }
      return true;
    }

    case SDLK_RIGHT: {
      if (cSelectedField == 0) {
        if (cSelectedEntry < cPalette.size() - 1) {
          cSelectedEntry++;
        }        
      } else {
        adjustChannel(1.0f / 255.0f);
      }
      return true;
    }

    case SDLK_LEFT: {
      if (cSelectedField == 0) {
        if (cSelectedEntry > 0) {
          cSelectedEntry--;
        }        
      } else {
        adjustChannel(-1.0f / 255.0f);
      }
      return true;
    }

    case SDLK_PAGEUP: {
      if (cSelectedField != 0) {
        adjustChannel(1.0f / 25.0f);
      }
      return true;
    }

    case SDLK_PAGEDOWN: {
      if (cSelectedField != 0) {
        adjustChannel(-1.0f / 25.0f);
      }
      return true;
    }

    default: {
      return false;
    }
  }
  return false;
}

bool PaletteConfigurationComponent::inputContent(SDL_Event& event) {
  switch (event.type) {
    case SDL_KEYDOWN: {
      return keyDown(event.key.keysym.sym);
    }

    case SDL_MOUSEBUTTONDOWN: {
      // TODO: return mouseButtonDown
      return false;
    }

    default: {
      return false;
    }
  }
  return false;
}

