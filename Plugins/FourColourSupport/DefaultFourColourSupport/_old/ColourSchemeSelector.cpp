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
#include "ColourSchemeSelector.h"

ColourSchemeSelector::ColourSchemeSelector() {
  cTextureFactory = new C64TextureFactory(); // TODO: Make non-toolkit specific
  cEditingScheme = false;
  cSelectedSet = 0;
}

void ColourSchemeSelector::addNewColourScheme() {
  ColourScheme *mDefaultColourScheme;
  mDefaultColourScheme = new ColourScheme(); 
  TextureSet *mTextureSet;
  mTextureSet = &cTextureFactory->generateTextureSet(*mDefaultColourScheme);
  cColourSchemes.push_back(mDefaultColourScheme);
  cTextureSets.push_back(mTextureSet);
}

bool ColourSchemeSelector::isTextureSelected() {
  return cSelectedSet != cTextureSets.size();
}

TextureSet& ColourSchemeSelector::getTextureSet() {
  return *cTextureSets[cSelectedSet];
}

TextureSet& ColourSchemeSelector::getTextureSet(int index) {
  return *cTextureSets[index];
}

ColourScheme* ColourSchemeSelector::getColourScheme() {
  return cColourSchemes[cSelectedSet];
}

ColourScheme* ColourSchemeSelector::getColourScheme(int index) {
  return cColourSchemes[index];
}

int ColourSchemeSelector::indexOf(TextureSet& textures) {
  for (int i = 0; i < cTextureSets.size(); i++) {
    if (cTextureSets[i] == &textures) {
      return i;
    }
  }
  cout << "Texture not found" << endl;
  exit(1);
}

void ColourSchemeSelector::selectTextureSet(TextureSet& textures) {
  cSelectedSet = indexOf(textures);
}

void ColourSchemeSelector::move(int x, int y, int z) {
  if (cEditingScheme) {
    cColourSchemeEditor.move(x, y, z);
    cColourSchemes[cSelectedSet] = &cColourSchemeEditor.getColourScheme();
    cTextureFactory->updateTextureSet(*cTextureSets[cSelectedSet], *cColourSchemes[cSelectedSet]);
  } else {
    cSelectedSet = restrict(0, cColourSchemes.size(), cSelectedSet - y);
  }
}

void ColourSchemeSelector::setDefaultMode() {
  cEditingScheme = false;
}

void ColourSchemeSelector::changeMode() {
  if (isTextureSelected()) {
    cEditingScheme = !cEditingScheme;
    if (cEditingScheme) {
      cColourSchemeEditor.reset();
      cColourSchemeEditor.setColourScheme(*cColourSchemes[cSelectedSet]);
    }
  } else {
    addNewColourScheme();
  }
}

void ColourSchemeSelector::render() {
  glBindTexture(GL_TEXTURE_2D, 0);
  if (cEditingScheme) {
    cColourSchemeEditor.render();
  } else {
    for (int i = 0; i < cColourSchemes.size(); i++) {
      Colour mFloorColour = (*cColourSchemes[i])[ColourScheme::FLOOR];
      Colour mWallColour = (*cColourSchemes[i])[ColourScheme::WALL];
      Colour mGridColour = (*cColourSchemes[i])[ColourScheme::GRID];
      Colour mBackgroundColour = (*cColourSchemes[i])[ColourScheme::BACKGROUND];
      float yOffset = i * 0.2;

      glBegin(GL_QUADS);

      mFloorColour.set();
      glVertex3f(1.2, -0.0 - yOffset, 0.0);
      glVertex3f(1.2, -0.15 - yOffset, 0.0);
      glVertex3f(1.4, -0.15 - yOffset, 0.0);
      glVertex3f(1.4, -0.0 - yOffset, 0.0);
    
      mWallColour.set();
      glVertex3f(1.4, -0.0 - yOffset, 0.0);
      glVertex3f(1.4, -0.15 - yOffset, 0.0);
      glVertex3f(1.6, -0.15 - yOffset, 0.0);
      glVertex3f(1.6, -0.0 - yOffset, 0.0);
    
      mGridColour.set();
      glVertex3f(1.6, -0.0 - yOffset, 0.0);
      glVertex3f(1.6, -0.15 - yOffset, 0.0);
      glVertex3f(1.8, -0.15 - yOffset, 0.0);
      glVertex3f(1.8, -0.0 - yOffset, 0.0);
    
      mBackgroundColour.set();
      glVertex3f(1.8, -0.0 - yOffset, 0.0);
      glVertex3f(1.8, -0.15 - yOffset, 0.0);
      glVertex3f(2.0, -0.15 - yOffset, 0.0);
      glVertex3f(2.0, -0.0 - yOffset, 0.0);

      glEnd();
    }

    float yOffset = cSelectedSet * 0.2;
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex3f(1.2,  -0.075 - yOffset, 0.0);
    glVertex3f(1.16, -0.055 - yOffset, 0.0);
    glVertex3f(1.16, -0.095 - yOffset, 0.0);
    glEnd();
  }
}

void ColourSchemeSelector::write(FILE *w) {
  for (int i = 0; i < cColourSchemes.size(); i++) {
    fprintf(w, "<COLOURSCHEME %d>\n", i);
    cColourSchemes[i]->write(w);
  }
}

void ColourSchemeSelector::load(FILE *r) {
  char current;
  int value;
  char word[256];
  float red;
  float green;
  float blue;

  do {
    fscanf(r, "%c", &current);
  } while (current != '<');
  fscanf(r, "%s", word);
  while (strcmp("START", word) != 0) {
    if (strcmp("COLOURSCHEME", word) != 0) {
      cout << "ERROR 1";
      exit(1);
    }
    fscanf(r, "%d", &value);
    do {
      fscanf(r, "%c", &current);
    } while (current != '<');
    fscanf(r, "%s", word);
    if (strcmp("COLOUR", word) != 0) {
      cout << "ERROR 2";
      exit(1);
    }
    fscanf(r, "%f", &red);
    fscanf(r, "%f", &green);
    fscanf(r, "%f", &blue);
    Colour mFloor(red, green, blue);
    do {
      fscanf(r, "%c", &current);
    } while (current != '<');
    fscanf(r, "%s", word);
    if (strcmp("COLOUR", word) != 0) {
      cout << "ERROR 3";
      exit(1);
    }
    fscanf(r, "%f", &red);
    fscanf(r, "%f", &green);
    fscanf(r, "%f", &blue);
    Colour mWall(red, green, blue);
    do {
      fscanf(r, "%c", &current);
    } while (current != '<');
    fscanf(r, "%s", word);
    if (strcmp("COLOUR", word) != 0) {
      cout << "ERROR 4";
      exit(1);
    }
    fscanf(r, "%f", &red);
    fscanf(r, "%f", &green);
    fscanf(r, "%f", &blue);
    Colour mGrid(red, green, blue);
    do {
      fscanf(r, "%c", &current);
    } while (current != '<');
    fscanf(r, "%s", word);
    if (strcmp("COLOUR", word) != 0) {
      cout << "ERROR 5";
      exit(1);
    }
    fscanf(r, "%f", &red);
    fscanf(r, "%f", &green);
    fscanf(r, "%f", &blue);
    Colour mBackground(red, green, blue);
    do {
      fscanf(r, "%c", &current);
    } while (current != '<');
    ColourScheme *mNewColourScheme;
    mNewColourScheme = new ColourScheme(mFloor, mWall, mGrid, mBackground); 
    TextureSet *mTextureSet;
    mTextureSet = &cTextureFactory->generateTextureSet(*mNewColourScheme);
    cColourSchemes.push_back(mNewColourScheme);
    cTextureSets.push_back(mTextureSet);
    fscanf(r, "%s", word);
  }
}
