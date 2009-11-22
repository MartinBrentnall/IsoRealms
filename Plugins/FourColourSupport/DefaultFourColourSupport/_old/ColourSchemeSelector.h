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
#ifndef COLOUR_SCHEME_SELECTOR_H
#define COLOUR_SCHEME_SELECTOR_H

#include <vector>
#include <map>

#include "C64TextureFactory.h" // TODO: Remove toolkit specific code.
#include "ColourSchemeEditor.h"
#include "ColourScheme.h"
#include "TextureSet.h"

using namespace std;

class ColourSchemeSelector {
  private:

  // TODO: TextureFactory *cTextureFactory = SpindizzyConfig::getTextureFactory();
  C64TextureFactory *cTextureFactory;
  ColourSchemeEditor cColourSchemeEditor;
  vector<ColourScheme*> cColourSchemes;
  vector<TextureSet*> cTextureSets;
  bool cEditingScheme;
  int cSelectedSet;

  public:
  ColourSchemeSelector();
  ColourScheme* getColourScheme();
  ColourScheme* getColourScheme(int index);
  TextureSet& getTextureSet();
  TextureSet& getTextureSet(int index);
  void selectTextureSet(TextureSet&);
  int indexOf(TextureSet&);
  bool isTextureSelected();
  void move(int x, int y, int z);
  void addNewColourScheme();
  void setDefaultMode();
  void changeMode();
  void render();
  void write(FILE *);
  void load(FILE *);
};

#endif
