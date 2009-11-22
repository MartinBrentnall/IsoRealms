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
#include "MainEditor.h"

MainEditor::MainEditor(Map& map) {
  cMap = &map;
  cMap->initialiseForEditor();
  cTextureSet = &cMap->getCurrentZoneColourScheme();
  started = false;
  editMode = EDIT_ZONE;
  quit = false;
  cRuntime = NULL;
  cLiftStage = 0;
  cFont.init("Font.ttf", 32);
  for (int i = 0; i < MAX_KEYS; i++) {
    cKeyDown[i] = false;
  }
}

MainEditor::MainEditor(string& filename) {
  FILE *r;
  r = fopen(filename.c_str(), "rb");
  cMap = new Map();
  if (r == NULL) {
    cColourEditor.addNewColourScheme();
    cTextureSet = &cColourEditor.getTextureSet();
    ColourScheme* mColours = cColourEditor.getColourScheme();
    cMap->init(*cTextureSet, mColours);
  } else {
    cColourEditor.load(r);
    cMap->load(r, cColourEditor);
    cTextureSet = &cMap->getCurrentZoneColourScheme();
    fclose(r);
  }
  cMap->initialiseForEditor();
  cCurrentMapName = filename;
  started = false;
  editMode = EDIT_ZONE;
  quit = false;
  cRuntime = NULL;

  // Set up camera viewpoints
  Coordinate mNormalDistance(0.0, 0.0, -20.0);
  cViewPoint.addViewPoint(0, mNormalDistance,  45.0, -50.0, 0.0);
  cViewPoint.setViewPoint(0);
  cLiftStage = 0;
  cFont.init("Font.ttf", 32);
  for (int i = 0; i < MAX_KEYS; i++) {
    cKeyDown[i] = false;
  }
}

void MainEditor::move(int y) {
  cViewPoint.move(y);
}

Map* MainEditor::getMap() {
  return cMap;
}

void MainEditor::setBlock() {
  Point end;
  int temp;
  int xstep;
  int ystep;

  if (!started) {
    start = cursor.getLocation();
  } else {
    end = cursor.getLocation();
    if (start.x > end.x) {
      temp = start.x;
      start.x = end.x;
      end.x = temp;       
    }
    if (start.y > end.y) {
      temp = start.y;
      start.y = end.y;
      end.y = temp;       
    }
    if (start.z > end.z) {
      temp = start.z;
      start.z = end.z;
      end.z = temp;       
    }
  }

  switch (editMode) {
    case EDIT_ZONE: 
      if (cBlockEditor.isJewel()) {
        Jewel *mJewel = new Jewel();
        mJewel->setLocation(cursor.getLocation());
        cMap->addJewel(*mJewel);
        started = false;
        return;
      } else if (cBlockEditor.isEnemy()) {
        Craft *mCraft = new Craft();
        Point mCraftLocation = cursor.getLocation();
        mCraft->setLocation(mCraftLocation);
        cMap->addEnemy(mCraft);
        started = false;
        return;
      } else if (cBlockEditor.isLift()) {
        cEditingLift = new Lift(cBlockEditor.getSymbol());
        Point mLiftLocation = cursor.getLocation();
        cEditingLift->setLocation(mLiftLocation);
        cMap->addLift(cEditingLift);
        cLiftStage = 1;
        started = false;
        return;
      } else if (started) {
        Block *mNewBlock = new Block(start, end, cBlockEditor);
        Condition* mCondition = cConditionEditor.getCondition();
        if (mCondition != NULL) {
          cout << "Conditional block has been set!" << endl;
          mNewBlock->setCondition(mCondition);
        }
        cRedoStack.clear();
        cMap->addBlock(*mNewBlock);
      }
      break;

    case EDIT_MAP:
      if (started) {
        if (cMap->overlaps(start, end)) {
          return;
        }
        end.x++;
        end.y++;
        end.z++;
        end.x -= start.x;
        end.y -= start.y;
        end.z -= start.z;
        Zone *mNewZone;
        ColourScheme *mColourScheme = cColourEditor.getColourScheme();
        mNewZone = new Zone(start, end, *cTextureSet, mColourScheme);
        cMap->addZone(*mNewZone);
      }
  }
  started = !started;
}

void MainEditor::confirm() {
  if (editMode == EDIT_MAP) {
    cRedoStack.clear();
    cMap->setActiveZone(cursor.getLocation());
    cTextureSet = &cMap->getCurrentZoneColourScheme();
    cColourEditor.selectTextureSet(*cTextureSet);
  } else if (editMode == EDIT_COLOUR) {
    if (cColourEditor.isTextureSelected()) {
      cTextureSet = &cColourEditor.getTextureSet();
    }
    cColourEditor.setDefaultMode();
  }
  setEditMode(EDIT_ZONE);
}

void MainEditor::move(int x, int y, int z) {
  if (cLiftStage != 0) {
    x = 0; y = 0;
  }

  switch (editMode) {
    case EDIT_ZONE: {
      cursor.move(x, y, z);
      Point mStartPoint = cMap->getStartPoint();
      Point mEndPoint = cMap->getEndPoint();
      cursor.restrainTo(mStartPoint, mEndPoint);
      int mCursorZ = cursor.getLocation().z;
      if (cLiftStage == 1) {
        cEditingLift->setHighPoint(mCursorZ);
        cEditingLift->setLowPoint(mCursorZ);
      }
      if (cLiftStage == 2) {
        cEditingLift->setLowPoint(mCursorZ);
      }
      break;
    }

    case EDIT_COLOUR:
      cColourEditor.move(x, y, z);
      if (cColourEditor.isTextureSelected()) {
        cMap->setCurrentZoneColourScheme(cColourEditor.getTextureSet());
      }
      break;

    case EDIT_BLOCK:
      cBlockEditor.move(x, y, z);
      break;

    case EDIT_CONDITION:
      cConditionEditor.move(x, y, z);
      break;

    case EDIT_MAP:
      cursor.move(x, y, z);
      break;
  }
}

void MainEditor::undo() {
  if (!cMap->isCurrentZoneEmpty() > 0) {
    Block* mBlock = cMap->popBlock();
    cRedoStack.push_back(mBlock);
  }
}

void MainEditor::redo() {
  if (cRedoStack.size() > 0) {
    cMap->addBlock(*cRedoStack[cRedoStack.size() - 1]);
    cRedoStack.pop_back();
  }
}

void MainEditor::checkKeyRelease() {
  cKeyDown[event.key.keysym.sym] = false;
}

void MainEditor::checkKeyPress() {
  switch (event.key.keysym.sym) {       
    case SDLK_ESCAPE:   quit = true;                     break;
    case SDLK_F12:      toggleFullScreen();              break;
    case SDLK_LEFT:     move(-1,  0,  0);                break;
    case SDLK_RIGHT:    move( 1,  0,  0);                break;
    case SDLK_UP:       move( 0,  1,  0);                break;
    case SDLK_DOWN:     move( 0, -1,  0);                break;
    case SDLK_PAGEUP:   move( 0,  0,  1);                break;
    case SDLK_PAGEDOWN: move( 0,  0, -1);                break;
    case SDLK_F1:       setEditMode(EDIT_ZONE);          break;
    case SDLK_F2:       setEditMode(EDIT_COLOUR);        break;
    case SDLK_F3:       setEditMode(EDIT_BLOCK);         break;
    case SDLK_F4:       setEditMode(EDIT_MAP);           break;
    case SDLK_F5:       setEditMode(EDIT_CONDITION);     break;
    case SDLK_SPACE:    setBlock();                      break;
    case SDLK_RETURN:   confirm();                       break;
    case 'u':           undo();                          break;
    case 'r':           redo();                          break;
    case 's':           toggleSplit();                   break;
    case 'e':           enter();                         break;
    case '1':           chooseBlockType(NORMAL);         break;
    case '2':           chooseBlockType(SPECIAL);        break;
    case '3':           chooseBlockType(JEWEL);          break;
    case '4':           chooseBlockType(ENEMY);          break;
    case '5':           chooseBlockType(LIFT);           break;
    case SDLK_F10:      write();                         break;
    case '9':           cMap->toggleSlope();             break;
    case 'g':           getColours();                    break;
    case 'p':           setColours();                    break;
    case 't':           testThisMap();                   break;
    case SDLK_DELETE:   if (editMode == EDIT_MAP) cMap->deleteZone(cursor.getLocation()); break;
  }
}

void MainEditor::testThisMap() {
  if (cRuntime == NULL) {
    cRuntime = new Runtime(*cMap);
  }
  cRuntime->mainLoop();
}

void MainEditor::write() {
  FILE *w;
  w = fopen(cCurrentMapName.c_str(), "wb");
  if (w == NULL) {
    cout << "Could not open file " << cCurrentMapName << " for writing" << endl;
  }
  cColourEditor.write(w);
  cMap->write(w, cColourEditor);
  fclose(w);
}

void MainEditor::enter() {
  if (editMode == EDIT_COLOUR) {
    cColourEditor.changeMode();
  }
}

void MainEditor::getColours() {
  cMap->setActiveZone(cursor.getLocation());
  cSelectedTexture = &cMap->getCurrentZoneColourScheme();
}

void MainEditor::setColours() {
  cMap->setActiveZone(cursor.getLocation());
  cMap->setCurrentZoneColourScheme(*cSelectedTexture);
}

void MainEditor::setEditMode(int mode) {
  started = false;
  editMode = mode;
  if (editMode == EDIT_MAP) {
    glDisable(GL_FOG);
  } else {
    glEnable(GL_FOG);
  }
}

void MainEditor::checkMouseMotion() {
  if (event.button.button == SDL_BUTTON_LEFT) {
    move(event.motion.yrel);
  } else {
    rotate(event.motion.xrel, event.motion.yrel);
  }
}

void MainEditor::checkMouseButton() {
}

void MainEditor::toggleSplit() {
  switch (editMode) {
    case EDIT_BLOCK: cBlockEditor.toggleSplit(); break;
    
    default:
      Point mCursorLocation = cursor.getLocation();
      cMap->setStartLocation(mCursorLocation);
      break;
  } 
}

void MainEditor::chooseBlockType(int type) {
  switch (editMode) {
    case EDIT_BLOCK: cBlockEditor.chooseType(type); break;
  }
}

void MainEditor::rotate(int x, int y) {
  cViewPoint.rotate(x, y);
}

int MainEditor::mainLoop() {
  Point s;
  Point e;
  int temp;
  int x;
  int y;
  Point dimension;

  cMap->clearZoneCaches();

  // Flush event buffer before we start
  while (SDL_PollEvent(&event));

  do {
    float angle = cViewPoint.getAngle();
    float tilt = cViewPoint.getTilt();
    if (editMode == EDIT_COLOUR) {
      glLoadIdentity();
      glTranslatef(1.45, 2.75, -6.0);
      cColourEditor.render();
    }
    if (editMode == EDIT_BLOCK) {
      glLoadIdentity();
      glTranslatef(0.0, 0.0, -10.0);
      glRotatef(-55.0, 1.0, 0.0, 0.0);
      glRotatef(45.0, 0.0, 0.0, 1.0);
      cBlockEditor.render(*cTextureSet, angle, tilt);
    }
    if (editMode == EDIT_CONDITION) {
      glLoadIdentity();
      glTranslatef(0.0, 0.0, -6.0);
      cConditionEditor.render();
    }
    SDL_GL_SwapBuffers();
    while (SDL_PollEvent(&event)) {
      cCurrentEditor->processInputEvent(event);
    }
  } while (!quit);
  write();
  quit = false;
}
