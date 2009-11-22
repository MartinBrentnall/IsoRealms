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
#ifndef RUNTIME_H
#define RUNTIME_H

#include <SDL/SDL.h>
#include <vector>

#include "Configuration.h"
#include "SpindizzyConstants.h"
#include "ColourSchemeSelector.h"
#include "PlaneSurface.h"
#include "Background.h"
#include "SwitchLogic.h"
#include "SoundSet.h"
#include "GLInit.h"
#include "Craft.h"
#include "Camera.h"
#include "Point.h"
#include "Zone.h"
#include "Map.h"
#include "SimpleCondition.h"

class Runtime:public SpindizzyConstants {
  private:
  static const int MODE_GAME = 1;
  static const int MODE_MAP = 2;

  // Viewpoints
  static const int VIEW_NORTH = 1;
  static const int VIEW_EAST = 2;
  static const int VIEW_SOUTH = 3;
  static const int VIEW_WEST = 4;
  static const int VIEW_MAP = 5;

  static const int MAX_KEYS = 512;  // TODO: How high does this need to be???

  static const int TIMER_RES = 10;

  static const int IMPACT_NONE = 0;
  static const int IMPACT_SLIDE_WALL = 1;
  static const int IMPACT_REGULAR = 2;
  static const int IMPACT_SURFACE = 3;

  float cMapShowing;

  struct RuntimeConfiguration {
    bool cShowSurroundingZones;
    bool cIsometricView;
    bool cHighDetail;
  };

  class AccelerateAction {
  };

  RuntimeConfiguration cConfig;
  SDL_Event cEvent;
  Map *cMap;
  Zone *cCurrentZone;
  PlaneSurface *cCurrentSurface;
  SwitchLogic *cSwitchStates;
  Background* cBackground;
  vector<Zone*> cAlphaZones;
  Camera cViewPoint;
  Craft cPlayer;
  SoundSet cSounds;
//  int cMusicChannel;
  bool cFirstPerson;
  SDL_Joystick* cJoystick;

  float cTimer;
  int cTotalJewels;
  int cRemainingJewels;
  int cTotalScreens;
  int cRemainingScreens;
  int cPitDepth;
  double cHeightDifference;
  bool cQuit;
  int cMode;
  int cView;
  int cMoveLock;
  WallSurface *cSlideWall;
  bool cMakeLandSound;

  bool cKeyDown[MAX_KEYS];

  void checkKeyRelease();
  void checkKeyPress();
  void checkMouseMotion();
  void checkMouseButton();
  void checkInput();
  void checkEndGame();
  void updateGame(int ticks);
  void update(int ticks);
  void activateSwitch();
  void toggleFirstPerson();
  void updateCurrentZone(Point &point);
  void updateAlphaEffects(int ticks);
  void updateJewelCollection(Coordinate&, Coordinate&);
  void updateAlienCollisions(Coordinate&, Coordinate&, float);
  void init();
  void toggleMap();
  void resetView();
  void rotateViewLeft();
  void rotateViewRight();
  void checkAnalogueMotion();
  Coordinate getTrampolineMomentumChange();
  Coordinate getMomentumChange(float ticks);

  /**
   * Get the direction of movement to lock given the specified event type.
   *
   * @param  int  Event type.
   * @returns     Movement direction to lock.
   */
  int getLockDirection(int);

  /**
   * Get the first of any event(s) occurring during movement between the two
   * specified location during the specificed amount of time.
   *
   * @param  Coordinate&  Start location of movement.
   * @param  Coordinate&  End location of movement.
   * @param  float        Time duration of movement.
   * @returns             First of any events occurring during movement.
   *                      If no events occurred, the returned vector is empty.
   */
  vector<ImpactData*> getEvents(Coordinate&, Coordinate&, float);

  public:
  Runtime(Map& map);
  Runtime(string& mapName);
  Map& getMap();
  void mainLoop();
};

#endif
