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
#ifndef MAIN_EDITOR_H
#define MAIN_EDITOR_H

#include <SDL/SDL.h>
#include <string>

#include "Background.h"
#include "Runtime.h"
#include "Map.h"
#include "ColourSchemeSelector.h"
#include "SpindizzyConstants.h"
#include "BlockEditor.h"
#include "Condition.h"
#include "ConditionEditor.h"
#include "ZoneCursor.h"
#include "GLInit.h"
#include "Jewel.h"
#include "Text.h"

class MainEditor:public SpindizzyConstants {
  private:
  static const int EDIT_ZONE = 0;
  static const int EDIT_COLOUR = 1;
  static const int EDIT_BLOCK = 2;
  static const int EDIT_MAP = 3;
  static const int EDIT_CONDITION = 4;

  static const int MAX_KEYS = 512; // TODO: How high does this need to be???
  bool cKeyDown[MAX_KEYS]; 

  ZoneEditor* cZoneEditor;
  ZoneContentEditor* cZoneContentEditor;
  TextureEditor* cTextureEditor;
  LogicConditionEditor* cLogicConditionEditor;
  

  Editor* cFocusedEditor;

  int cFocus;

  bool quit;
  Map *cMap;
  SDL_Event event;
  string cCurrentMapName;
  Camera cViewPoint;
  int editMode;
  Runtime *cRuntime;
  bool started;
  Point start;
  vector<Block*> cRedoStack;
  ColourSchemeSelector cColourEditor;
  ConditionEditor cConditionEditor;
  TextureSet *cTextureSet;
  TextureSet *cSelectedTexture;
  BlockEditor cBlockEditor;
  Background *cBackground;
  ZoneCursor cursor;
  Lift *cEditingLift;
  int cLiftStage;

  /** The font used to display user interface elements */
  freetype::font_data cFont;

  void testThisMap();
  void undo();
  void redo();

  public:
  MainEditor(string& filename);
  MainEditor(Map& map);
  void checkKeyRelease();
  void checkKeyPress();
  void checkMouseMotion();
  void checkMouseButton();
  void chooseBlockType(int);
  void confirm();
  void move(int, int, int);
  void enter();
  void toggleSplit();
  int mainLoop();
  void setEditMode(int);
  void move(int);
  void setBlock();
  void rotate(int, int);
  void getColours();
  void setColours();
  void write();
  void drawWireFrameCuboid(Point location, Point dimension);
  Map* getMap();
};

#endif
