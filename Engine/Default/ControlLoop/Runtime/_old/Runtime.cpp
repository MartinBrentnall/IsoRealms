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
#include "Runtime.h"

Runtime::Runtime(Map& map) {
  cMap = &map;
  init();
}

Runtime::Runtime(string& filename) {
  FILE *r;
  r = fopen(filename.c_str(), "rb");
  if (r == NULL) {
    cout << "Map does not exist" << endl;
    exit(1);
  }
  ColourSchemeSelector* mColourSchemes = new ColourSchemeSelector();
  mColourSchemes->load(r);
  cMap = new Map();
  cMap->load(r, *mColourSchemes);
  init();
}

void Runtime::init() {
  cConfiguration = Configuration::getInstance();
  cSwitchLogic = cConfiguration->getSwitchLogic();
  cMap->initialiseForRuntime();
  cCurrentZone = NULL;
  cBackground = new Background();
  cTotalScreens = cMap->getZoneCount();
  cTotalJewels = cMap->getJewelCount();
  cRemainingScreens = cTotalScreens;
  cRemainingJewels = cTotalJewels;
  Lift::setSwitchStates(&cSwitchStates);
  for (int i = 0; i < MAX_KEYS; i++) {
    cKeyDown[i] = false;
  }
  cMode = MODE_GAME;

  Point mStartLocation = cMap->getStartLocation();
  Coordinate mActualStartLocation(mStartLocation.x + 0.5, mStartLocation.y + 0.5, mStartLocation.z);
  cPlayer.setLocation(mActualStartLocation);
  Point mPoint = cPlayer.getGridLocation();
  updateCurrentZone(mPoint);
  cCurrentSurface = NULL;
  cPitDepth = cMap->getPitDepth();
  cQuit = false;
  cFirstPerson = false;

  // Set up camera viewpoints
  Coordinate mNormalDistance(0.0, 0.0, -20.0);
  Coordinate mMapDistance(0.0, 0.0, -200.0);
  cViewPoint.addViewPoint(NORTH, mNormalDistance,  45.0, -50.0, 0.0);
  cViewPoint.addViewPoint(EAST,  mNormalDistance, 135.0, -50.0, 0.0);
  cViewPoint.addViewPoint(SOUTH, mNormalDistance, 225.0, -50.0, 0.0);
  cViewPoint.addViewPoint(WEST,  mNormalDistance, 315.0, -50.0, 0.0);
  cViewPoint.addViewPoint(MAP,   mMapDistance,     90.0,   0.0, 0.0);
  cView = NORTH;
  cViewPoint.setViewPoint(MAP);
  cViewPoint.setViewPoint(cView);
  Lift::setSounds(&cSounds);
  SimpleCondition::setSwitchStates(&cSwitchStates);
  cout << "Map successfully initialised for runtime" << endl;

  if (SDL_NumJoysticks() > 0) {
    cJoystick = SDL_JoystickOpen(0);
    SDL_JoystickEventState(SDL_ENABLE);
    if (cJoystick) {
      cout << "Opened Joystick 0" << endl;
      printf("Number of Axes: %d\n", SDL_JoystickNumAxes(cJoystick));
    } else {
      cout << "Couldn't open Joystick 0" << endl;
    }
  }
  cConfig.cShowSurroundingZones = true;
  cConfig.cIsometricView = false;
  cConfig.cHighDetail = true;
  cSlideWall = NULL;
  cMapShowing = 1.0f;
}

Map& Runtime::getMap() {
  return *cMap;
}

void Runtime::checkKeyRelease() {
  cKeyDown[cEvent.key.keysym.sym] = false;
}

void Runtime::checkKeyPress() {
  if (cEvent.key.keysym.sym >= MAX_KEYS) {
    cout << "WARNING: Unrecognised key: " << cEvent.key.keysym.sym << " - Please submit a bug report" << endl;
  } else {
    cKeyDown[cEvent.key.keysym.sym] = true;
  }
  switch (cEvent.key.keysym.sym) {       
    case SDLK_ESCAPE:   cQuit = true;                     break;
    case SDLK_F12:      toggleFullScreen();               break;
    case SDLK_SPACE:    if (cCurrentSurface != NULL) {cPlayer.stop();} break; // TODO Cannot stop on ice
    case SDLK_F1:       resetView();                      break;
    case SDLK_F3:       rotateViewLeft();                 break;
    case SDLK_F5:       rotateViewRight();                break;
    case SDLK_F8:       cConfig.cShowSurroundingZones = !cConfig.cShowSurroundingZones; break;
    case 'f':           toggleFirstPerson();              break;
    case 'm':           toggleMap();                      break;
    case 'c':           Craft::toggleCraftTypes();        break;
    case 'x':           /* Change craft */                break;
  }
}

void Runtime::checkAnalogueMotion() {
  cout << cEvent.jaxis.axis << " axis changed to " << cEvent.jaxis.value << endl;
  switch (cEvent.jaxis.axis) {
    case 0: cout << "X CHANGE!" << endl; break;
    case 1: cout << "Y CHANGE!" << endl; break;
  }
}

void Runtime::resetView() {
  if (cMode == MODE_GAME) {
    cView = NORTH;
    cViewPoint.setViewPoint(cView);
  }
}

void Runtime::rotateViewLeft() {
  if (cMode == MODE_GAME) {
    switch (cView) {
      case NORTH: cView = WEST;  break;
      case EAST:  cView = NORTH; break;
      case SOUTH: cView = EAST;  break;
      case WEST:  cView = SOUTH; break;
    }
    cViewPoint.setViewPoint(cView);
  }
}

void Runtime::rotateViewRight() {
  if (cMode == MODE_GAME) {
    switch (cView) {
      case NORTH: cView = EAST;  break;
      case EAST:  cView = SOUTH; break;
      case SOUTH: cView = WEST;  break;
      case WEST:  cView = NORTH; break;
    }
    cViewPoint.setViewPoint(cView);
  }
}

void Runtime::toggleFirstPerson() {
// TODO
/*  cFirstPerson = !cFirstPerson;
  if (cFirstPerson) {
    Coordinate mInternal(0.0, BLOCK_HEIGHT * -1.4, 0.0);
    cViewPoint.setLocation(mInternal);
    cViewPoint.setRotation(0.0, -90.0, 0.0);
    gluPerspective(100.0, 4.0 / 3.0, 0.1, 1000.0);
  } else {
    Coordinate mExternal(0.0, 0.0, -20.0);
    cViewPoint.setLocation(mExternal);
    cViewPoint.setRotation(0.0, -90.0, 0.0);
    gluPerspective(50.0, 4.0 / 3.0, 0.1, 1000.0);
  }*/
}

void Runtime::checkInput() {
  while (SDL_PollEvent(&cEvent)) {
    switch (cEvent.type) {
      case SDL_KEYDOWN         : checkKeyPress();       break;
      case SDL_KEYUP           : checkKeyRelease();     break;
      case SDL_MOUSEMOTION     : checkMouseMotion();    break;
      case SDL_MOUSEBUTTONDOWN : checkMouseButton();    break;
      case SDL_JOYAXISMOTION   : checkAnalogueMotion(); break;
    }
  }
}

void Runtime::toggleMap() {
  cMode = cMode == MODE_MAP ? MODE_GAME : int(MODE_MAP);
  if (cMode == MODE_MAP) {
    Colour mBlack(0.0, 0.0, 0.0);
    cBackground->set(mBlack);
    cViewPoint.setViewPoint(MAP);
    glFogf(GL_FOG_START, 200.0f);
    glFogf(GL_FOG_END, 400.0f);
  } else {
    Colour mColour = cCurrentZone->getBackground();
    cBackground->set(mColour);
    cViewPoint.setViewPoint(cView);
    glFogf(GL_FOG_START, 20.0f);
    glFogf(GL_FOG_END, 40.0f);
  }
}

void Runtime::checkMouseMotion() {
  if (cMode == MODE_MAP) {
    if (cEvent.button.button == SDL_BUTTON_LEFT) {
      cViewPoint.move(cEvent.motion.yrel * 1.0);
    } else {
      cViewPoint.rotate(cEvent.motion.xrel * 1.0, cEvent.motion.yrel * 1.0);
    }
  }
}

void Runtime::checkMouseButton() {
}

void Runtime::updateCurrentZone(Point& point) {
  Zone *mEnteredZone = NULL;
  if (cCurrentZone != NULL) {
    mEnteredZone = cCurrentZone->getAdjacentZoneAt(point);
  }
  if (mEnteredZone == NULL) {
    mEnteredZone = cMap->getZoneAt(point);
  }
  if (mEnteredZone != NULL) {
    cCurrentZone = mEnteredZone;
    cCurrentZone->resetDynamics();
    Colour mColour = mEnteredZone->getBackground();
    cBackground->set(mColour);
    if (cCurrentZone->setExplored()) {
      cRemainingScreens--;
      cAlphaZones.push_back(cCurrentZone);
      checkEndGame();
    }
  }
}

void Runtime::updateAlphaEffects(int ticks) {
  for (int i = 0; i < cAlphaZones.size(); i++) {
    if (cAlphaZones[i]->lighten(ticks * 0.0015)) {
      Zone *mZone = cAlphaZones[i];
      cAlphaZones[i] = cAlphaZones[cAlphaZones.size() - 1];
      cAlphaZones[cAlphaZones.size() - 1] = mZone;
      cAlphaZones.pop_back();
      i--;
    }
  }
}

void Runtime::update(int ticks) {
  switch (cMode) {
    case MODE_GAME: updateGame(ticks); break;
    case MODE_MAP:                     break;
  }
  cBackground->update(ticks);
  cViewPoint.updateCamera(ticks);
}

Coordinate Runtime::getTrampolineMomentumChange() {
  // Ticks are fixed
  double mHighCraftAcceleration = CRAFT_ACCELERATION * 2.0;
  double mAcceleration = (cKeyDown[SDLK_TAB] ? mHighCraftAcceleration : CRAFT_ACCELERATION) * SIMULATED_TRAMPOLINE_TICKS;
  Coordinate mMomentumChange;
  if (cKeyDown[SDLK_LEFT] && cMoveLock != SOUTH) {
    mMomentumChange.y += mAcceleration;
    if (cMoveLock == NORTH) {
      cMoveLock = NO_DIRECTION;
      cSlideWall = NULL;
    }
  }
  if (cKeyDown[SDLK_RIGHT] && cMoveLock != NORTH) {
    mMomentumChange.y -= mAcceleration;
    if (cMoveLock == SOUTH) {
      cMoveLock = NO_DIRECTION;
      cSlideWall = NULL;
    }
  }
  if (cKeyDown[SDLK_UP] && cMoveLock != WEST) {
    mMomentumChange.x += mAcceleration;
    if (cMoveLock == EAST) {
      cMoveLock = NO_DIRECTION;
      cSlideWall = NULL;
    }
  }
  if (cKeyDown[SDLK_DOWN] && cMoveLock != EAST) {
    mMomentumChange.x -= mAcceleration;
    if (cMoveLock == WEST) {
      cMoveLock = NO_DIRECTION;
      cSlideWall = NULL;
    }
  }
  return mMomentumChange;
}

Coordinate Runtime::getMomentumChange(float ticks) {
  double mHighCraftAcceleration = CRAFT_ACCELERATION * 2.0;
  double mAcceleration = (cKeyDown[SDLK_TAB] ? mHighCraftAcceleration : CRAFT_ACCELERATION) * ticks;
  double mPull = GRAVITY_STRENGTH * ticks;

  bool mOnIce = false;
  bool mOnTrampoline = false;
  bool mOnLift = false;
  if (cCurrentSurface != NULL) {
    mOnIce = cCurrentSurface->getSymbol() == ICE;
    mOnTrampoline = cCurrentSurface->getSymbol() == TRAMPOLINE;
    mOnLift = cCurrentSurface->getSymbol() == LIFT_SURFACE;
  }

  Coordinate *mCurPos = &cPlayer.getLocation();

  Coordinate mMomentumChange;
  if (cCurrentSurface != NULL) {
    if (!mOnIce) {
      if (cKeyDown[SDLK_LEFT] && cMoveLock != SOUTH) {
        mMomentumChange.y += mAcceleration;
        if (cMoveLock == NORTH) {
          cMoveLock = NO_DIRECTION;
          cSlideWall = NULL;
        }
      }
      if (cKeyDown[SDLK_RIGHT] && cMoveLock != NORTH) {
        mMomentumChange.y -= mAcceleration;
        if (cMoveLock == SOUTH) {
          cMoveLock = NO_DIRECTION;
          cSlideWall = NULL;
        }
      }
      if (cKeyDown[SDLK_UP] && cMoveLock != WEST) {
        mMomentumChange.x += mAcceleration;
        if (cMoveLock == EAST) {
          cMoveLock = NO_DIRECTION;
          cSlideWall = NULL;
        }
      }
      if (cKeyDown[SDLK_DOWN] && cMoveLock != EAST) {
        mMomentumChange.x -= mAcceleration;
        if (cMoveLock == WEST) {
          cMoveLock = NO_DIRECTION;
          cSlideWall = NULL;
        }
      }
    }

    if (cCurrentSurface != NULL) {
      mMomentumChange.x += -cCurrentSurface->getXSlopeAt(*mCurPos) * CRAFT_ACCELERATION * ticks;
      mMomentumChange.y += -cCurrentSurface->getYSlopeAt(*mCurPos) * CRAFT_ACCELERATION * ticks;
    }
  } else {
    mMomentumChange.z -= mPull;
  }
  return mMomentumChange;
}

vector<ImpactData*> Runtime::getEvents(Coordinate& start, Coordinate& end, float ticks) {
  float mLowestGradient = 2.0f;

  // Get collision events occurring in the zone.
  vector<ImpactData*> mEvents;

  // Test for events caused by rolling along the current surface.
  // (e.g. leaving the surface, or hitting a bump in the surface).
  if (cCurrentSurface != NULL) {
    ImpactPoint *mRollingSurfaceEvent = cCurrentSurface->getRollingEvent(start, end, ticks);
    if (mRollingSurfaceEvent != NULL) {
      mLowestGradient = mRollingSurfaceEvent->getGradient();
      ImpactData* mRollingEvent = new ImpactData(*mRollingSurfaceEvent, cCurrentSurface);
      mEvents.push_back(mRollingEvent);
    }
  }

  // Test for any event that ends sliding along a wall.
  if (cSlideWall != NULL) {
    ImpactPoint *mSlideWallEvent = cSlideWall->getSlideEvent(start, end);
    if (mSlideWallEvent != NULL) {
      float mGradient = mSlideWallEvent->getGradient();
      if (mGradient <= mLowestGradient) {
        ImpactData* mSlideEvent = new ImpactData(*mSlideWallEvent, cSlideWall);
        if (mGradient < mLowestGradient) {
          mLowestGradient = mGradient;
          mEvents.clear();
        }
        mEvents.push_back(mSlideEvent);
      }
    }
  }

  // Test for collision events with things in and aronud this zone.
  vector<ImpactData*> mCollisionEvents = cCurrentZone->getCollisionEvent(start, end, ticks, cCurrentSurface);
  if (mCollisionEvents.size() > 0) {
    float mGradient = mCollisionEvents[0]->getGradient();
    if (mGradient < mLowestGradient) {
      mEvents = mCollisionEvents;
    } else if (mGradient == mLowestGradient) {
      for (int i = 0; i < mCollisionEvents.size(); i++) {
        mEvents.push_back(mCollisionEvents[i]);
      }
    }
  }


  return mEvents;
}

int Runtime::getLockDirection(int eventType) {
  switch (eventType) {
    case EVENT_HIT_NORTH_WALL: return NORTH;
    case EVENT_HIT_EAST_WALL:  return EAST;
    case EVENT_HIT_SOUTH_WALL: return SOUTH;
    case EVENT_HIT_WEST_WALL:  return WEST;
  }
  cout << "No lock direction for event type " << eventType << endl;
  exit(1);
}

void Runtime::updateGame(int ticks) {
  Craft::updateCraftTypes(ticks);

  float mTicksToProcess = ticks;

  updateAlphaEffects(ticks);
  cPlayer.updateVisuals(ticks);

  // Process game physics / collisions
  while (mTicksToProcess > 0) {

    // Get an amount of time to process
    float mTicksPassed = min((float) TIMER_RES, mTicksToProcess);
    mTicksToProcess -= mTicksPassed;
  
    // Predict where the craft will be after the amount of time to process
    int mCurrentSurfaceType = 0;
    if (cCurrentSurface != NULL) {
      mCurrentSurfaceType = cCurrentSurface->getSymbol();
    }
    Coordinate *mCurPos = &cPlayer.getLocation();
    Coordinate mMomentumChange = getMomentumChange(mTicksPassed);
    Coordinate mNewPos = cPlayer.requestLocation(mTicksPassed, cCurrentSurface == NULL, mCurrentSurfaceType == ICE || mCurrentSurfaceType == TRAMPOLINE, mMomentumChange);
    if (cCurrentSurface != NULL) {
      mNewPos.z = cCurrentSurface->getHeightAt(mNewPos);
    }

    // Get the next event(s) of the craft
    vector<ImpactData*> mCraftEvents = getEvents(*mCurPos, mNewPos, mTicksPassed);

    bool mLeftSurface = false; // TODO: Remove this; it's too hackish

    // Process craft event(s)
    for (int i = 0; i < mCraftEvents.size(); i++) {
      int mEventType = mCraftEvents[i]->getType();
      bool mLocked = false;
      mNewPos = mCraftEvents[i]->getLocation().getRelocation();
      switch (mEventType) {
        case EVENT_LEAVE_FLOOR: {
          float mXSlope = cCurrentSurface->getXSlopeAt(mNewPos);
          float mYSlope = cCurrentSurface->getYSlopeAt(mNewPos);
          cMakeLandSound = cPlayer.calculateZMomentum(mXSlope, mYSlope);
          cCurrentSurface = NULL;
          mLeftSurface = true;
          cout << "Floor leave " << endl;
          break;
        }

        case EVENT_LEAVE_WALL: {
          cMoveLock = NO_DIRECTION;
          cSlideWall = NULL;
          cout << "Slide end" << endl;
          break;
        }

        case EVENT_HIT_WEST_WALL: {
          if (mNewPos.z > mCurPos->z && cCurrentSurface != NULL) {
            cPlayer.calculateZMomentum(cCurrentSurface->getXSlopeAt(mNewPos), cCurrentSurface->getYSlopeAt(mNewPos));
            cCurrentSurface = NULL;
          }
          mLocked = cPlayer.bounceWall(WEST);
          cout << "West wall" << endl;
          break;
        }

        case EVENT_HIT_NORTH_WALL: {
          if (mNewPos.z > mCurPos->z && cCurrentSurface != NULL) {
            cPlayer.calculateZMomentum(cCurrentSurface->getXSlopeAt(mNewPos), cCurrentSurface->getYSlopeAt(mNewPos));
            cCurrentSurface = NULL;
          }
          mLocked = cPlayer.bounceWall(NORTH);
          cout << "North wall" << endl;
          break;
        }

        case EVENT_HIT_EAST_WALL: {
          if (mNewPos.z > mCurPos->z && cCurrentSurface != NULL) {
            cPlayer.calculateZMomentum(cCurrentSurface->getXSlopeAt(mNewPos), cCurrentSurface->getYSlopeAt(mNewPos));
            cCurrentSurface = NULL;
          }
          mLocked = cPlayer.bounceWall(EAST);
          cout << "East wall" << endl;
          break;
        }

        case EVENT_HIT_SOUTH_WALL: {
          if (mNewPos.z > mCurPos->z && cCurrentSurface != NULL) {
            cPlayer.calculateZMomentum(cCurrentSurface->getXSlopeAt(mNewPos), cCurrentSurface->getYSlopeAt(mNewPos));
            cCurrentSurface = NULL;
          }
          mLocked = cPlayer.bounceWall(SOUTH);
          cout << "South wall" << endl;
          break;
        }

        case EVENT_HIT_FLOOR: {
          if (mCraftEvents[i]->getSurface()->getSymbol() == TRAMPOLINE && mNewPos.z > mCurPos->z) {
            cSounds.play(TRAMPOLINE_BOUNCE);
            cPlayer.bounceTrampoline();
            Coordinate mActualMomentumChange = getTrampolineMomentumChange();
            cPlayer.changeMomentum(mActualMomentumChange);
            cMakeLandSound = true;
            cout << "Bouncy bouncy!" << endl;
          } else {
            cCurrentSurface = mCraftEvents[i]->getSurface();
            activateSwitch();
            if (cMakeLandSound && !mLeftSurface) {
              // TODO: Impact type depends on how far we've jumped and the landing surface
              cSounds.play(IMPACT_HARD);
            }
            cout << "Floor enter: " << cCurrentSurface << endl;
          }
          break;
        }

        default: {
          cout << "Unknown hit impact type: " << mEventType << endl;
          exit(1);
        }
      }

      float mTicksUsed = mTicksPassed * mCraftEvents[i]->getGradient();
        // TODO: Change everything up until this point!
      mTicksToProcess += mTicksPassed - mTicksUsed;
      mTicksPassed = mTicksUsed;
      if (mLocked) {
        cout << "Locking movement for sliding" << endl;
        cMoveLock = getLockDirection(mEventType);
        cSlideWall = mCraftEvents[i]->getWall();
      }
    }
    cCurrentZone->updateZoneDynamics(mTicksPassed, *mCurPos);

    Coordinate mActualMomentumChange = getMomentumChange(mTicksPassed);
    cPlayer.changeMomentum(mActualMomentumChange);
    
    updateJewelCollection(*mCurPos, mNewPos);
    updateAlienCollisions(*mCurPos, mNewPos, mTicksPassed);

    // Gradually slow down the craft.
    cPlayer.updateLocation(mNewPos);
    if (cCurrentSurface != NULL && mCurrentSurfaceType != ICE && mCurrentSurfaceType != TRAMPOLINE) {
      cPlayer.updateMomentum(mTicksPassed);
    }

    Coordinate *mPos = &cPlayer.getLocation();
/*    Coordinate mLastPos(mPos->x, mPos->y, mPos->z);*/
  
    Point mPoint = cPlayer.getGridLocation();

    if (!cCurrentZone->contains(mPoint)) {
      updateCurrentZone(mPoint);
    }

    if (cPlayer.getGridLocation().z < cPitDepth) {
      cPlayer.respawn();
      if (cMakeLandSound) {
        cSounds.play(RESPAWN_HARD);
      }
      mPoint = cPlayer.getGridLocation();
      updateCurrentZone(mPoint);
      // TODO: Need to respawn correctly when falling off lower half of steep slope.
//      cout << "Respawn at " << mPos->x << "," << mPos->y << "," << mPos->z << endl;
      Coordinate mCoord(mPos->x, mPos->y, mPos->z + 1);
      cCurrentSurface = cCurrentZone->getSurfaceAt(mCoord);
      mTicksToProcess = 0.0f;
    }

    if (cCurrentSurface != NULL && mCurrentSurfaceType != ICE && mCurrentSurfaceType != TRAMPOLINE) {
      cPlayer.setRespawnPoint(mPoint);
    }

    // Update position on slope
    if (cCurrentSurface != NULL) {
      float mNewHeight = cCurrentSurface->getHeightAt(*mPos);
      cPlayer.setHeight(mNewHeight);
    }
  }
}

void Runtime::checkEndGame() {
  if (cRemainingJewels + cRemainingScreens == 0) {
    cout << "WELL DONE!" << endl;
  }
}

void Runtime::updateJewelCollection(Coordinate& start, Coordinate& end) {
  int mJewelsCollected = cCurrentZone->collectJewels(start, end);
  if (mJewelsCollected > 0) {
    cSounds.play(JEWEL_COLLECT);
    cRemainingJewels -= mJewelsCollected;
    checkEndGame();
  }
}

void Runtime::updateAlienCollisions(Coordinate& start, Coordinate& end, float ticks) {
  float mDamage = cCurrentZone->calculateDamage(start, end, ticks);
//  cout << "Damage taken: " << mDamage << endl;
}

void Runtime::activateSwitch() {
  int mSwitch = cCurrentSurface->getSymbol();
  if (mSwitch >= 0 && mSwitch <= 11 && cSwitchStates.activate(mSwitch)) {
    cSounds.play(mSwitch);
  }
}

void Runtime::mainLoop() {
  // Flush event buffer before we start
  while (SDL_PollEvent(&cEvent));

  cMap->cacheZoneSurfaces();

  int mOldTicks = SDL_GetTicks();
  int mStartTicks = mOldTicks;
  int mFrameCount = 0;
  cSounds.play(MUSIC);
  do {
    int mNewTicks = SDL_GetTicks();
    int mTicksPassed = mNewTicks - mOldTicks;
    if (mTicksPassed > 0) {
      if (mFrameCount > 0) {
  //      cout << "Average FPS: " << (mFrameCount / ((mNewTicks - mStartTicks) / 1000.0)) << endl;
      }
      mFrameCount++;
      mOldTicks = mNewTicks;
  //    SDL_Delay(10); // NOTE: Enable to simulate a very slow PC (for testing purposes, etc.)
      checkInput();
      update(mTicksPassed);
      SDL_GL_SwapBuffers();
      glLoadIdentity();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      cViewPoint.place();
      float mAngle = cViewPoint.getAngle();
      float mTilt = cViewPoint.getTilt();
      if (cConfig.cShowSurroundingZones) {
        cPlayer.moveToCamera();
      } else {
        cCurrentZone->moveToCamera();
      }
      switch (cMode) {
        case MODE_GAME:
          if (cMapShowing < 1.0) {
            cMapShowing += 0.0025 * mTicksPassed;
          }
          cCurrentZone->renderAsInGame(cConfig.cShowSurroundingZones, mTicksPassed, cMapShowing, mAngle, mTilt);
          if (cMapShowing < 1.0) {
            glEnable(GL_BLEND);
            glClear(GL_DEPTH_BUFFER_BIT);
            cMap->renderAsInGame(-cMapShowing + 1.0, mTicksPassed);
            glDisable(GL_BLEND);
          }
          break;
  
        case MODE_MAP:
          if (cMapShowing > 0.0) {
            cMapShowing -= 0.0025 * mTicksPassed;
            cCurrentZone->renderAsInGame(cConfig.cShowSurroundingZones, mTicksPassed, cMapShowing, mAngle, mTilt);
            glClear(GL_DEPTH_BUFFER_BIT);
            glEnable(GL_BLEND);
          }
          cMap->renderAsInGame(-cMapShowing + 1.0, mTicksPassed);
          glDisable(GL_BLEND);
          break;
      }
      if (!cFirstPerson) {
        cPlayer.render(1.0, mAngle, mTilt);
      }
    }
  } while (!cQuit);
  cQuit = false;
}
