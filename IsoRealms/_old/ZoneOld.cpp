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
#include "Zone.h"

Zone::Zone(Point loc, Point dim, TextureSet& textures, ColourScheme* colours) {
  cStartLocation.set(loc);
  cEndLocation.set(dim);
  if (&textures == NULL) {
    cout << "CANNOT ADD NULL TEXTURE POINTER TO ZONE!" << endl;
    exit(1);
  }
  cTextures = &textures;
  cColours = colours;
  cAlpha = 0.0;
  cDirty = true;
  cExplored = false;
  cContainsDisplayList = false;
}

Zone::~Zone() {
  clearCaches();
  for (int i = 0; i < cBlocks.size(); i++) {
    delete cBlocks[i];
  }
  for (int i = 0; i < cJewels.size(); i++) {
    delete cJewels[i];
  }
  if (cContainsDisplayList) {
    glDeleteLists(cDisplayList, 1);
  }
}

bool Zone::isAdjacentTo(Zone& z) {
  int xmax = max(cStartLocation.x, z.cStartLocation.x);
  int ymax = max(cStartLocation.y, z.cStartLocation.y);
  int zmax = max(cStartLocation.z, z.cStartLocation.z);
  int xmin = min(cEndLocation.x + cStartLocation.x - 1, z.cEndLocation.x + z.cStartLocation.x - 1);
  int ymin = min(cEndLocation.y + cStartLocation.y - 1, z.cEndLocation.y + z.cStartLocation.y - 1);
  int zmin = min(cEndLocation.z + cStartLocation.z - 1, z.cEndLocation.z + z.cStartLocation.z - 1);
  if (xmax <= xmin + 1 && ymax <= ymin + 1 && zmax <= zmin + 1) {
    return true;
  }
  return false;
}

void Zone::moveToCamera() {
  glTranslatef(float(-cStartLocation.x) - cEndLocation.x / 2.0, float(-cStartLocation.y) - cEndLocation.x / 2.0, float(-cStartLocation.z));
//  glTranslatef(-cStartLocation.x, -cStartLocation.y, -cStartLocation.z);
}

void Zone::setBackground() {
  Colour mBackgroundColour = (*cColours)[ColourScheme::BACKGROUND];
  glClearColor(mBackgroundColour.getRed(), mBackgroundColour.getGreen(), mBackgroundColour.getBlue(), 0.0);
}

bool Zone::isNearby(Zone& zone) {
  float mDistance = getDistanceFrom(zone);
//  cout << "Distance is " << mDistance << endl;
  return mDistance < 32.0;
}

void Zone::initForEditor() {
  cBackgroundZones.clear();
  cExplored = true;
  cSpriteVisibility = 1.0;
  if (cExplored) {
    cDirty = true; // Restore wireframe grid
  }
}

void Zone::initForRuntime() {
  cSpriteVisibility = 0.0;
}

bool Zone::setExplored() {
  bool mNewZone = !cExplored;
  cExplored = true;
  return mNewZone;
}

void Zone::resetExplored() {
  cExplored = false;
}

float Zone::getDistanceFrom(Zone& zone) {
  Point mThisEnd(cEndLocation.x + cStartLocation.x, cEndLocation.y + cStartLocation.y, cEndLocation.z + cStartLocation.z);
  Point mThatEnd(zone.cEndLocation.x + zone.cStartLocation.x, zone.cEndLocation.y + zone.cStartLocation.y, zone.cEndLocation.z + zone.cStartLocation.z);


  bool mXAlligned = (cStartLocation.x >= zone.cStartLocation.x && cStartLocation.x <= mThatEnd.x) || (mThisEnd.x >= zone.cStartLocation.x && mThisEnd.x <= mThatEnd.x);
  bool mYAlligned = (cStartLocation.y >= zone.cStartLocation.y && cStartLocation.y <= mThatEnd.y) || (mThisEnd.y >= zone.cStartLocation.y && mThisEnd.y <= mThatEnd.y);
  bool mZAlligned = (cStartLocation.z >= zone.cStartLocation.z && cStartLocation.z <= mThatEnd.z) || (mThisEnd.z >= zone.cStartLocation.z && mThisEnd.y <= mThatEnd.z);

  // Zones are cCollisionZones
  if (mXAlligned && mYAlligned && mZAlligned) {
    return 0.0;
  }

  // Check if zones are in line; then we just return the block distance between them
  if (mXAlligned && mYAlligned) {
    return cStartLocation.z > zone.cStartLocation.z ? cStartLocation.z - mThatEnd.z : zone.cStartLocation.z - mThisEnd.z;
  }
  if (mXAlligned && mZAlligned) {
    return cStartLocation.y > zone.cStartLocation.y ? cStartLocation.y - mThatEnd.y : zone.cStartLocation.y - mThisEnd.y;
  }
  if (mYAlligned && mZAlligned) {
    return cStartLocation.x > zone.cStartLocation.x ? cStartLocation.x - mThatEnd.x : zone.cStartLocation.x - mThisEnd.x;
  }

  // TODO: Implement this
  return 10000.0;
}

void Zone::addCollisionZone(Zone *z) {
  cCollisionZones.push_back(z);
}

void Zone::addBackgroundZone(Zone *z) {
  cBackgroundZones.push_back(z);
}


Zone* Zone::getAdjacentZoneAt(Point point) {
  for (int i = 0; i < cCollisionZones.size(); i++) {
    if (cCollisionZones[i]->contains(point)) {
      return cCollisionZones[i];
    }
  }
  return NULL;
}

Point Zone::getEndPoint() {
  return cEndLocation;
}

Point Zone::getStartPoint() {
  return cStartLocation;
}

void Zone::toggleSlope() {
  if (cBlocks.size() > 0) {
    cBlocks[cBlocks.size() - 1]->toggleSlope();
  }
  updateExclusions();
}

TextureSet& Zone::getColourScheme() {
  return *cTextures;
}

bool Zone::contains(Point p) {
  return p.x >= cStartLocation.x 
      && p.y >= cStartLocation.y
      && p.z >= cStartLocation.z
      && p.x <= cEndLocation.x + cStartLocation.x - 1
      && p.y <= cEndLocation.y + cStartLocation.y - 1
      && p.z <= cEndLocation.z + cStartLocation.z - 1;
}

void Zone::addJewel(Jewel& jewel) {
  cJewels.push_back(&jewel);
  cDirty = true;
}

void Zone::addEnemy(Craft* craft) {
  cEnemies.push_back(craft);
  cDirty = true;
}

void Zone::addLift(Lift* lift) {
  cLifts.push_back(lift);
  cDirty = true;
}

void Zone::addBlock(Block& block, bool updateExclude) {

  // Ensure sloped block is within zone boundary.
  int xstep = abs(block.getXSlope());
  int ystep = abs(block.getYSlope());
  Point start = block.getStartLocation();
  Point end = block.getEndLocation();
  if (((end.x - start.x) * xstep) + ((end.y - start.y) * ystep) + end.z > cEndLocation.z - 1) {
    return;
  }

  cBlocks.push_back(&block);

  if (updateExclude) {
    updateExclusions();
  }
  cDirty = true;
}

Block* Zone::popBlock() {
  Block* block = cBlocks[cBlocks.size() - 1];
  cBlocks.pop_back();
  updateExclusions();
  cDirty = true;
  return block;
}

void Zone::setColours(TextureSet& textures) {
  cTextures = &textures;
  cDirty = true;
}

bool Zone::isEmpty() {
  return cBlocks.size() == 0;
}

void Zone::write(FILE *w, ColourSchemeSelector& colourSelection) {
  fprintf(w, "<ZONE LOCATION %d %d %d DIMENSIONS %d %d %d COLOURSCHEME %d>\n",
          cStartLocation.x, cStartLocation.y, cStartLocation.z,
          cEndLocation.x, cEndLocation.y, cEndLocation.z, colourSelection.indexOf(*cTextures));
  for (int i = 0; i < cBlocks.size(); i++) {
    cBlocks[i]->write(w);
  }
  for (int i = 0; i < cJewels.size(); i++) {
    cJewels[i]->write(w);
  }
  for (int i = 0; i < cEnemies.size(); i++) {
    cEnemies[i]->write(w);
  }
  for (int i = 0; i < cLifts.size(); i++) {
    cLifts[i]->write(w);
  }
}

void Zone::load(FILE *r) {
  char current;
  char word[256];
  Point cStartLocation;
  Point cEndLocation;
  int north;
  int south;
  int east;
  int west;
  int split;
  int symbol;
  int hollow;

  while (!feof(r)) {
    fscanf(r, "%c", &current);
    if (current <= 32) {
      // Ignorable.
    } else if (current == '<') {
      fscanf(r, "%s", word);
      if (strcmp("BLOCK", word) == 0) {
        fscanf(r, "%s", word);
        if (strcmp("LOCATION", word) == 0) {
          fscanf(r, "%d", &cStartLocation.x);
          fscanf(r, "%d", &cStartLocation.y);
          fscanf(r, "%d", &cStartLocation.z);
          fscanf(r, "%s", word);
          if (strcmp("DIMENSION", word) == 0) {
            fscanf(r, "%d", &cEndLocation.x);
            fscanf(r, "%d", &cEndLocation.y);
            fscanf(r, "%d", &cEndLocation.z);
            fscanf(r, "%s", word);
            Block* mNewBlock;
            if (strcmp("TYPE", word) == 0) {
              fscanf(r, "%d", &symbol);
              mNewBlock = new Block(cStartLocation, cEndLocation, symbol);
              addBlock(*mNewBlock, false);
              fscanf(r, "%c", &current);
            } else if (strcmp("NORTH", word) == 0) {
              fscanf(r, "%d", &north);
              fscanf(r, "%s", word);
              fscanf(r, "%d", &south);
              fscanf(r, "%s", word);
              fscanf(r, "%d", &east);
              fscanf(r, "%s", word);
              fscanf(r, "%d", &west);
              fscanf(r, "%s", word);
              fscanf(r, "%d", &split);

              fscanf(r, "%s", word);
              fscanf(r, "%d", &hollow);

              mNewBlock = new Block(cStartLocation, cEndLocation, north, east, south, west, split, hollow);
              addBlock(*mNewBlock, false);
              fscanf(r, "%c", &current);
            } else {
              printf("Unrecognised keyword %s in map file.\n");
              exit(1);
            }
            if (current == ' ') {
              int mNot;
              int mSymbolA;
              int mSymbolB;
              fscanf(r, "%s", word);
              fscanf(r, "%s", word);
              fscanf(r, "%d", &mNot);
              fscanf(r, "%s", word);
              fscanf(r, "%d", &mSymbolA);
              fscanf(r, "%s", word);
              fscanf(r, "%d", &mSymbolB);
              fscanf(r, "%c", &current);
/*              Condition* mCondition = new Condition(mSymbolA, mSymbolB, mNot == 1 ? true : false);
              mNewBlock->setCondition(mCondition);*/
            }
          } else {
            printf("Unrecognised keyword %s in map file.\n");
            exit(1);
          }
        } else {
          printf("Unrecognised keyword %s in map file.\n");
          exit(1);
        }
      } else if (strcmp("JEWEL", word) == 0) {
        fscanf(r, "%s", word);
        if (strcmp("LOCATION", word) == 0) {
          fscanf(r, "%d", &cStartLocation.x);
          fscanf(r, "%d", &cStartLocation.y);
          fscanf(r, "%d", &cStartLocation.z);
          Jewel *mJewel = new Jewel(cStartLocation);
          addJewel(*mJewel);
          fscanf(r, "%c", &current);
        } else {
          printf("Unrecognised keyword %s in map file.\n");
          exit(1);
        }
      } else if (strcmp("CRAFT", word) == 0) {
        fscanf(r, "%s", word);
        int mCraftType;
        if (strcmp("TYPE", word) == 0) {
          fscanf(r, "%d", &mCraftType);
        } else {
          printf("Unrecognised keyword %s in map file.\n");
          exit(1);
        }
        fscanf(r, "%s", word);
        if (strcmp("LOCATION", word) == 0) {
          fscanf(r, "%d", &cStartLocation.x);
          fscanf(r, "%d", &cStartLocation.y);
          fscanf(r, "%d", &cStartLocation.z);
          Craft *mEnemy = new Craft(cStartLocation, mCraftType);
          addEnemy(mEnemy);
          fscanf(r, "%c", &current);
        } else {
          printf("Unrecognised keyword %s in map file.\n");
          exit(1);
        }
      } else if (strcmp("LIFT", word) == 0) {
        int mLiftType;
        fscanf(r, "%s", word);
        if (strcmp("TYPE", word) == 0) {
          fscanf(r, "%d", &mLiftType);
        } else {
          printf("Unrecognised keyword %s in map file:  Expected TYPE\n");
          exit(1);
        }
        fscanf(r, "%s", word);
        if (strcmp("LOCATION", word) != 0) {
          printf("Unrecognised keyword %s in map file:  Expected LOCATION\n");
          exit(1);
        }
        fscanf(r, "%d", &cStartLocation.x);
        fscanf(r, "%d", &cStartLocation.y);
        fscanf(r, "%d", &cStartLocation.z);
        fscanf(r, "%s", word);
        if (strcmp("TOP", word) != 0) {
          printf("Unrecognised keyword %s in map file:  Expected TOP\n");
          exit(1);
        }
        Lift* mLift = new Lift(mLiftType);
        mLift->setLocation(cStartLocation);
        int mTemp;
        fscanf(r, "%d", &mTemp);
        mLift->setHighPoint(mTemp);
        fscanf(r, "%d", &mTemp);
        mLift->setUpDelay(mTemp);
        fscanf(r, "%s", word);
        if (strcmp("BOTTOM", word) != 0) {
          printf("Unrecognised keyword %s in map file:  Expected BOTTOM\n");
          exit(1);
        }
        fscanf(r, "%d", &mTemp);
        mLift->setLowPoint(mTemp);
        fscanf(r, "%d", &mTemp);
        mLift->setDownDelay(mTemp);

        addLift(mLift);
        fscanf(r, "%c", &current);
      } else if (strcmp("ZONE", word) == 0) {
        return;
      } else {
        printf("Unrecognised keyword %s in map file.\n");
        exit(1);
      }
    } else {
      printf("Error parsing map file in zone (%c).\n", current);
      exit(1);
    }
  }
}

bool Zone::lighten(float value) {
  cAlpha += value;
  bool mDone = false;
  if (cAlpha > 1.0) {
    cAlpha = 1.0;
    mDone = true;
  }
  return mDone;
}

bool Zone::areJewelsRemaining() {
  for (int i = 0; i < cJewels.size(); i++) {
    if (!cJewels[i]->isCollected()) {
      return true;
    }
  }
  return false;
}

void Zone::renderAsMap(float alpha, int ticks) {
  float x = (cEndLocation.x + cStartLocation.x - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float y = (cEndLocation.y + cStartLocation.y - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float z = (cEndLocation.z + cStartLocation.z - 1) * BLOCK_HEIGHT;
  float xs = (cStartLocation.x - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float ys = (cStartLocation.y - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float zs = (cStartLocation.z - 1) * BLOCK_HEIGHT;

  glBindTexture(GL_TEXTURE_2D, 0);
  if (cExplored) {
    glColor4f(0.8, 1.0, 1.0, alpha);
  } else {
    glColor4f(0.0, 0.2, 0.3, alpha);
  }
  glLineWidth(2.0);
  renderForMap();
  if (cExplored && areJewelsRemaining()) {
    glPushMatrix();
    glTranslatef(BLOCK_RADIUS * -1.0, BLOCK_RADIUS * -1.0, 0.0);
    glTranslatef(cStartLocation.x + cEndLocation.x / 2.0, cStartLocation.y + cEndLocation.y / 2.0, (cStartLocation.z) * BLOCK_HEIGHT);
    glScalef(4.0, 4.0, 4.0);
    cMapJewel.render(ticks, alpha);
    glScalef(1.0, 1.0, 1.0);
    glPopMatrix();
  }
  glEnd();
}

void Zone::renderAsInGame(bool primary, int ticks, float alpha, float angle, float tilt) {
  renderAsInGame(primary, primary, true, ticks, alpha, angle, tilt);
}

void Zone::renderAsInGame(bool primary, bool fadein, bool showSprites, int ticks, float alpha, float angle, float tilt) {
  if (!cExplored) {
    return;
  }

  glColor4f(1.0f * alpha, 1.0f * alpha, 1.0f * alpha, cAlpha);
  if (fadein && cAlpha < 1.0) {
    glEnable(GL_BLEND);
  } else {
    glDisable(GL_BLEND);
  }

  if (cDirty) {
    // TODO: SOMETHING IN HERE IS ALLOCATING MEMORY AND THEN NOT FREEING IT!
    cDisplayList = glGenLists(1);
    glNewList(cDisplayList, GL_COMPILE_AND_EXECUTE);
    for (int i = 0; i < cBlocks.size(); i++) {
      cBlocks[i]->renderStatic(*cTextures);
    }
    glEndList();
    cContainsDisplayList = true;
    cDirty = false;
  } else {
    glCallList(cDisplayList);
  }
  for (int i = 0; i < cDynamicPlaneSurfaces.size(); i++) {
    cDynamicPlaneSurfaces[i]->render(*cTextures);
  }

  for (int i = 0; i < cDynamicWallSurfaces.size(); i++) {
    cDynamicWallSurfaces[i]->render(*cTextures);
  }

  if (fadein && cAlpha * alpha < 1.0) {
    glEnable(GL_BLEND);
  } else {
    glDisable(GL_BLEND);
  }
  for (int i = 0; i < cJewels.size(); i++) {
    cJewels[i]->render(ticks, alpha * cAlpha);
  }

  if (primary) {
    for (int i = 0; i < cBackgroundZones.size(); i++) {
      cBackgroundZones[i]->renderAsInGame(false, fadein, false, ticks, alpha, angle, tilt);
    }
  }
  
  if (showSprites) {
    if (cSpriteVisibility < 1.0) {
      cSpriteVisibility += 0.0025 * ticks;
      if (cSpriteVisibility > 1.0) {
        cSpriteVisibility = 1.0;
      }
    }
  } else {
    if (cSpriteVisibility > 0.0) {
      cSpriteVisibility -= 0.0025 * ticks;
      if (cSpriteVisibility < 0.0) {
        cSpriteVisibility = 0.0;
      }
    }
  }

  if (cSpriteVisibility > 0.0) {
    if (cSpriteVisibility < 1.0) {
      glEnable(GL_BLEND);
    }
    for (int i = 0; i < cEnemies.size(); i++) {
      cEnemies[i]->render(cSpriteVisibility, angle, tilt);
    }
    for (int i = 0; i < cLifts.size(); i++) {
      cLifts[i]->renderAsDynamic(cSpriteVisibility);
    }
    glDisable(GL_BLEND);
  }

  if (cAlpha < 1.0) {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  }
}

int Zone::getJewelCount() {
  return cJewels.size();
}

void Zone::renderAsEditor(float angle, float tilt) {
  glDisable(GL_BLEND);
  if (cDirty) {
    // TODO: SOMETHING IN HERE IS ALLOCATING MEMORY AND THEN NOT FREEING IT!
    glDeleteLists(cDisplayList, 1);
    cDisplayList = glGenLists(1);
    glNewList(cDisplayList, GL_COMPILE_AND_EXECUTE);
    renderEditBoundaries();
    for (int i = 0; i < cBlocks.size(); i++) {
//      cout << "Doing block " << i << endl;
      cBlocks[i]->render(*cTextures);
    }
    glEndList();
    cContainsDisplayList = true;
    cDirty = false;
  } else {
    glCallList(cDisplayList);
  }

  for (int i = 0; i < cJewels.size(); i++) {
    cJewels[i]->render(20, 0.0);
  }

  for (int i = 0; i < cEnemies.size(); i++) {
    cEnemies[i]->render(1.0, angle, tilt); 
  }

  for (int i = 0; i < cLifts.size(); i++) {
    cLifts[i]->render(cSpriteVisibility);
  }
}

int Zone::collectJewels(Coordinate& start, Coordinate& end) {
  int mCollected = 0;
  for (int i = 0; i < cJewels.size(); i++) {
    if (cJewels[i]->isCollected(start, end)) {
      mCollected++;
    }
  }
  return mCollected;
}

float Zone::calculateDamage(Coordinate& start, Coordinate& end, float ticks) {
  float mDamage = 0.0f;
  for (int i = 0; i < cEnemies.size(); i++) {
    mDamage += cEnemies[i]->getCollisionTime(start, end, ticks);
  }
  return mDamage;
}

PlaneSurface* Zone::getLiftAt(Coordinate& point) {
  float mClosest = -1000; // TODO: Better precision (or something)

  Point mPoint((int) floor(point.x + 0.5), (int) floor(point.y + 0.5), (int) floor(point.z + 0.5));
  PlaneSurface* mResult = NULL;
  for (int i = 0; i < cLifts.size(); i++) {
    if (cLifts[i]->within(mPoint.x, mPoint.y)) {
      float mGroundHeight = cLifts[i]->getHeightAt(point);
      if (mGroundHeight - STEP_HEIGHT <= point.z) {
        mClosest = mGroundHeight;
        mResult = cLifts[i];
      }
    }
  }
  return mResult;
}

PlaneSurface* Zone::getSurfaceAt(Coordinate& point) {
  float mClosest = -1000; // TODO: Better precision (or something)

  PlaneSurface* mResult = getLiftAt(point);
  if (mResult != NULL) {
    return mResult;
  }

  Point mPoint((int) floor(point.x + 0.5), (int) floor(point.y + 0.5), (int) floor(point.z + 0.5));
  for (int i = 0; i < cStaticPlaneSurfaces.size(); i++) {
    if (cStaticPlaneSurfaces[i]->getSymbol() != WATER && cStaticPlaneSurfaces[i]->isBelow(mPoint)) {
      float mGroundHeight = cStaticPlaneSurfaces[i]->getHeightAt(point);
      if (mGroundHeight - STEP_HEIGHT <= point.z && mGroundHeight - STEP_HEIGHT >= mClosest) {
        mClosest = mGroundHeight;
        mResult = cStaticPlaneSurfaces[i];
      }
    }
  }
  for (int i = 0; i < cDynamicPlaneSurfaces.size(); i++) {
    if (cDynamicPlaneSurfaces[i]->getSymbol() != WATER && cDynamicPlaneSurfaces[i]->isBelow(mPoint) && cDynamicPlaneSurfaces[i]->isVisible()) {
      float mGroundHeight = cDynamicPlaneSurfaces[i]->getHeightAt(point);
      if (mGroundHeight - STEP_HEIGHT <= point.z && mGroundHeight - STEP_HEIGHT >= mClosest) {
        mClosest = mGroundHeight;
        mResult = cDynamicPlaneSurfaces[i];
      }
    }
  }

  return mResult;
}

void Zone::renderEditBoundaries() {
  float x = (cEndLocation.x + cStartLocation.x - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float y = (cEndLocation.y + cStartLocation.y - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float z = (cEndLocation.z + cStartLocation.z - 1) * BLOCK_HEIGHT;
  float xs = (cStartLocation.x - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float ys = (cStartLocation.y - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float zs = (cStartLocation.z - 1) * BLOCK_HEIGHT;

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINES);
  glVertex3f(xs, ys, z); glVertex3f(x,  ys, z);
  glVertex3f(x,  ys, z); glVertex3f(x,  y,  z);
  glVertex3f(x,  y,  z); glVertex3f(xs, y,  z);
  glVertex3f(xs, y,  z); glVertex3f(xs, ys, z);

  glVertex3f(xs, ys, zs);   glVertex3f(x,  ys, zs);
  glVertex3f(x,  ys, zs);   glVertex3f(x,  y,  zs);
  glVertex3f(x,  y,  zs);   glVertex3f(xs, y,  zs);
  glVertex3f(xs, y,  zs);   glVertex3f(xs, ys, zs);

  glVertex3f(x,  ys, z); glVertex3f(x,  ys, zs);
  glVertex3f(x,  y,  z); glVertex3f(x,  y,  zs);
  glVertex3f(xs, y,  z); glVertex3f(xs, y,  zs);
  glVertex3f(xs, ys, z); glVertex3f(xs, ys, zs);
  glEnd();
}

void Zone::renderForMap() {
  float x = (cEndLocation.x + cStartLocation.x - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float y = (cEndLocation.y + cStartLocation.y - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float z = (cEndLocation.z + cStartLocation.z - 1) * BLOCK_HEIGHT;
  float xs = (cStartLocation.x - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float ys = (cStartLocation.y - 1) * BLOCK_SIZE + BLOCK_RADIUS;
  float zs = (cStartLocation.z - 1) * BLOCK_HEIGHT;
  float mOffset = BLOCK_RADIUS * 0.6;

  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_LINES);
  glVertex3f(xs + mOffset, ys + mOffset, z - BLOCK_HEIGHT * 0.5); glVertex3f(x - mOffset,  ys + mOffset, z - BLOCK_HEIGHT * 0.5);
  glVertex3f(x - mOffset,  ys + mOffset, z - BLOCK_HEIGHT * 0.5); glVertex3f(x - mOffset,  y - mOffset,  z - BLOCK_HEIGHT * 0.5);
  glVertex3f(x - mOffset,  y - mOffset,  z - BLOCK_HEIGHT * 0.5); glVertex3f(xs + mOffset, y - mOffset,  z - BLOCK_HEIGHT * 0.5);
  glVertex3f(xs + mOffset, y - mOffset,  z - BLOCK_HEIGHT * 0.5); glVertex3f(xs + mOffset, ys + mOffset, z - BLOCK_HEIGHT * 0.5);

  glVertex3f(xs + mOffset, ys + mOffset, zs + BLOCK_HEIGHT * 0.5);   glVertex3f(x - mOffset,  ys + mOffset, zs + BLOCK_HEIGHT * 0.5);
  glVertex3f(x - mOffset,  ys + mOffset, zs + BLOCK_HEIGHT * 0.5);   glVertex3f(x - mOffset,  y - mOffset,  zs + BLOCK_HEIGHT * 0.5);
  glVertex3f(x - mOffset,  y - mOffset,  zs + BLOCK_HEIGHT * 0.5);   glVertex3f(xs + mOffset, y - mOffset,  zs + BLOCK_HEIGHT * 0.5);
  glVertex3f(xs + mOffset, y - mOffset,  zs + BLOCK_HEIGHT * 0.5);   glVertex3f(xs + mOffset, ys + mOffset, zs + BLOCK_HEIGHT * 0.5);

  glVertex3f(x - mOffset,  ys + mOffset, z - BLOCK_HEIGHT * 0.5); glVertex3f(x - mOffset,  ys + mOffset, zs + BLOCK_HEIGHT * 0.5);
  glVertex3f(x - mOffset,  y - mOffset,  z - BLOCK_HEIGHT * 0.5); glVertex3f(x - mOffset,  y - mOffset,  zs + BLOCK_HEIGHT * 0.5);
  glVertex3f(xs + mOffset, y - mOffset,  z - BLOCK_HEIGHT * 0.5); glVertex3f(xs + mOffset, y - mOffset,  zs + BLOCK_HEIGHT * 0.5);
  glVertex3f(xs + mOffset, ys + mOffset, z - BLOCK_HEIGHT * 0.5); glVertex3f(xs + mOffset, ys + mOffset, zs + BLOCK_HEIGHT * 0.5);
  glEnd();
}

void Zone::clearCaches() {
  for (int i = 0; i < cStaticPlaneSurfaces.size(); i++) {
    delete cStaticPlaneSurfaces[i];
  }
  cStaticPlaneSurfaces.clear();
  cDirty = true;
}

void Zone::cacheSurfaces() {
  if (!cStaticPlaneSurfaces.empty()) {
    clearCaches();
  }

  for (int i = 0; i < cBlocks.size(); i++) {
    vector<PlaneSurface*> mBlockFloorSurfaces = cBlocks[i]->getStaticPlaneSurfaces(FLOOR);
    vector<PlaneSurface*> mBlockCeilingSurfaces = cBlocks[i]->getStaticPlaneSurfaces(CEILING);
    vector<PlaneSurface*> mDynamicFloorSurfaces = cBlocks[i]->getDynamicPlaneSurfaces(FLOOR);
    vector<PlaneSurface*> mDynamicCeilingSurfaces = cBlocks[i]->getDynamicPlaneSurfaces(CEILING);

    vector<WallSurface*> mNorthStaticWallSurfaces = cBlocks[i]->getStaticWallSurfaces(NORTH);
    vector<WallSurface*> mSouthStaticWallSurfaces = cBlocks[i]->getStaticWallSurfaces(SOUTH);
    vector<WallSurface*> mEastStaticWallSurfaces = cBlocks[i]->getStaticWallSurfaces(EAST);
    vector<WallSurface*> mWestStaticWallSurfaces = cBlocks[i]->getStaticWallSurfaces(WEST);

    vector<WallSurface*> mNorthDynamicWallSurfaces = cBlocks[i]->getDynamicWallSurfaces(NORTH);
    vector<WallSurface*> mSouthDynamicWallSurfaces = cBlocks[i]->getDynamicWallSurfaces(SOUTH);
    vector<WallSurface*> mEastDynamicWallSurfaces = cBlocks[i]->getDynamicWallSurfaces(EAST);
    vector<WallSurface*> mWestDynamicWallSurfaces = cBlocks[i]->getDynamicWallSurfaces(WEST);

    for (int j = 0; j < mBlockFloorSurfaces.size(); j++) {
      if (!mBlockFloorSurfaces[j]->isCeiling()) {
        cStaticPlaneSurfaces.push_back(mBlockFloorSurfaces[j]);
      }
    }
    for (int j = 0; j < mBlockCeilingSurfaces.size(); j++) {
      if (!mBlockCeilingSurfaces[j]->isCeiling()) {
        cStaticPlaneSurfaces.push_back(mBlockCeilingSurfaces[j]);
      }
    }
    for (int j = 0; j < mDynamicFloorSurfaces.size(); j++) {
      if (!mDynamicFloorSurfaces[j]->isCeiling()) {
        cDynamicPlaneSurfaces.push_back(mDynamicFloorSurfaces[j]);
      }
    }
    for (int j = 0; j < mDynamicCeilingSurfaces.size(); j++) {
      if (!mDynamicCeilingSurfaces[j]->isCeiling()) {
        cDynamicPlaneSurfaces.push_back(mDynamicCeilingSurfaces[j]);
      }
    }

    for (int j = 0; j < mNorthStaticWallSurfaces.size(); j++) {
      WallSurface* mWallSurface = mNorthStaticWallSurfaces[j];
      cStaticWallSurfaces.push_back(mWallSurface);
    }
    for (int j = 0; j < mSouthStaticWallSurfaces.size(); j++) {
      WallSurface* mWallSurface = mSouthStaticWallSurfaces[j];
      cStaticWallSurfaces.push_back(mWallSurface);
    }
    for (int j = 0; j < mEastStaticWallSurfaces.size(); j++) {
      WallSurface* mWallSurface = mEastStaticWallSurfaces[j];
      cStaticWallSurfaces.push_back(mWallSurface);
    }
    for (int j = 0; j < mWestStaticWallSurfaces.size(); j++) {
      WallSurface* mWallSurface = mWestStaticWallSurfaces[j];
      cStaticWallSurfaces.push_back(mWallSurface);
    }

    for (int j = 0; j < mNorthDynamicWallSurfaces.size(); j++) {
      WallSurface* mWallSurface = mNorthDynamicWallSurfaces[j];
      cDynamicWallSurfaces.push_back(mWallSurface);
    }
    for (int j = 0; j < mSouthDynamicWallSurfaces.size(); j++) {
      WallSurface* mWallSurface = mSouthDynamicWallSurfaces[j];
      cDynamicWallSurfaces.push_back(mWallSurface);
    }
    for (int j = 0; j < mEastDynamicWallSurfaces.size(); j++) {
      WallSurface* mWallSurface = mEastDynamicWallSurfaces[j];
      cDynamicWallSurfaces.push_back(mWallSurface);
    }
    for (int j = 0; j < mWestDynamicWallSurfaces.size(); j++) {
      WallSurface* mWallSurface = mWestDynamicWallSurfaces[j];
      cDynamicWallSurfaces.push_back(mWallSurface);
    }
  }
  cDirty = true;
}

Colour& Zone::getBackground() {
  return (*cColours)[ColourScheme::BACKGROUND];
}

void Zone::resetDynamics() {
  for (int i = 0; i < cEnemies.size(); i++) {
    cEnemies[i]->resetToOrigin();
  }
  for (int i = 0; i < cLifts.size(); i++) {
    cLifts[i]->resetToOrigin();
  }
}

vector<ImpactData*> Zone::getCollisionEvent(Coordinate& start, Coordinate& end, float ticks, PlaneSurface* rollingSurface) {
  vector<ImpactData*> mImpacts;
  float mLowestGradient = 2.0f;


  // TODO: This is very hackish; surface at zone edges should be cached?
  vector<PlaneSurface*> mStaticPlaneSurfaces = cStaticPlaneSurfaces;
  for (int i = 0; i < cCollisionZones.size(); i++) {
    for (int j = 0; j < cCollisionZones[i]->cStaticPlaneSurfaces.size(); j++) {
      mStaticPlaneSurfaces.push_back(cCollisionZones[i]->cStaticPlaneSurfaces[j]);
    }
  }
  vector<WallSurface*> mStaticWallSurfaces = cStaticWallSurfaces;
  for (int i = 0; i < cCollisionZones.size(); i++) {
    for (int j = 0; j < cCollisionZones[i]->cStaticWallSurfaces.size(); j++) {
      mStaticWallSurfaces.push_back(cCollisionZones[i]->cStaticWallSurfaces[j]);
    }
  }

  // Check lifts for collisions
  for (int i = 0; i < cLifts.size(); i++) {
    ImpactPoint *mImpactPoint = cLifts[i]->getCollisionEvent(start, end, ticks);
    if (mImpactPoint != NULL) {
      if (mImpactPoint->getGradient() <= mLowestGradient) {
        if (mImpactPoint->getGradient() < mLowestGradient) {
          mLowestGradient = mImpactPoint->getGradient();
          mImpacts.clear();
          // TODO: Unallocate space used by ImpactDatas
        }
        cout << "Stepped onto a lift!" << endl;
      }
    }
  }

  // TODO: If lift and floor are hit simultaneously, we must use the higher of the two - same with multiple lifts

  // Check floors for collisions
  for (int i = 0; i < mStaticPlaneSurfaces.size(); i++) {
    if (mStaticPlaneSurfaces[i]->getSymbol() != WATER && mStaticPlaneSurfaces[i] != rollingSurface) {
      ImpactPoint *mImpactPoint = mStaticPlaneSurfaces[i]->getCollisionEvent(start, end, ticks);
      if (mImpactPoint != NULL) {
        if (mImpactPoint->getGradient() <= mLowestGradient) {
          if (mImpactPoint->getGradient() < mLowestGradient) {
            mLowestGradient = mImpactPoint->getGradient();
            mImpacts.clear();
            // TODO: Unallocate space used by ImpactDatas
            mImpacts.push_back(new ImpactData(*mImpactPoint, mStaticPlaneSurfaces[i]));
          } else { // TODO: Need to be sure that it's really another block... what about lifts?
            PlaneSurface* mOtherSurface = mImpacts[mImpacts.size() - 1]->getSurface();
            PlaneSurface* mLineBetweenSurfaces = new LineSurface(mOtherSurface, mStaticPlaneSurfaces[i]);
            mImpacts.pop_back(); // TODO: De-allocate?
            mImpacts.push_back(new ImpactData(*mImpactPoint, mLineBetweenSurfaces));
          }
        }
      }
    }
  }

  // Check walls for collisions
  for (int i = 0; i < mStaticWallSurfaces.size(); i++) {
    ImpactPoint *mImpactPoint = mStaticWallSurfaces[i]->getCollisionEvent(start, end);
    if (mImpactPoint != NULL) {
      if (mImpactPoint->getGradient() <= mLowestGradient) {
        if (mImpactPoint->getGradient() < mLowestGradient) {
          mLowestGradient = mImpactPoint->getGradient();
          mImpacts.clear();
          // TODO: Unallocate space used by ImpactDatas
        }
        mImpacts.push_back(new ImpactData(*mImpactPoint, mStaticWallSurfaces[i]));
      }
    }
  }

  return mImpacts;
}

void Zone::updateZoneDynamics(float ticks, Coordinate& player) {
  for (int i = 0; i < cEnemies.size(); i++) {
    cEnemies[i]->moveTowards(player, ticks);
    Point mLocation = cEnemies[i]->getGridLocation();
    cEnemies[i]->setGhost(!contains(mLocation));
  }
  for (int i = 0; i < cLifts.size(); i++) {
    cLifts[i]->updateLocation(ticks);
  }
}

void Zone::updateExclusions() {
  Point cStartLocation;
  Point cEndLocation;

  // Remove all exclusions first
  for (int i = 0; i < cBlocks.size(); i++) {
    cBlocks[i]->removeAllExclusions();
  }

  // Here we do the floors.
  for (int i = 0; i < cBlocks.size(); i++) {
    for (int j = 0; j < cBlocks.size(); j++) {
/*      if (i == j || (cBlocks[j]->isSpace() && (i > j || cBlocks[i]->isSpace()))) {
        continue;
      }
      if (j > i && cBlocks[i]->isSpace() && !cBlocks[j]->isSpace()) {
        continue;
      }
      if (i > j && cBlocks[j]->isSpace() && !cBlocks[i]->isSpace()) {
        continue;
      }
      if (cBlocks[i]->overlapsWithFloor(cBlocks[j], i < j)) {*/
//        cBlocks[j]->setInclusions();
      if (i == j) { // Floors do not exclude themselves.
        continue;
      }
      cBlocks[i]->addFloorExclusion(cBlocks[j], i < j);
    }
  }

  // Here we do the ceilings.
  for (int i = 0; i < cBlocks.size(); i++) {
    for (int j = 0; j < cBlocks.size(); j++) {
/*      if (i == j || (cBlocks[j]->isSpace() && (i > j || cBlocks[i]->isSpace()))) {
        continue;
      }
      if (j > i && cBlocks[i]->isSpace() && !cBlocks[j]->isSpace()) {
        continue;
      }
      if (i > j && cBlocks[j]->isSpace() && !cBlocks[i]->isSpace()) {
        continue;
      }
      if (cBlocks[i]->overlapsWithCeiling(cBlocks[j], i < j)) {*/
      if (i == j/* || cBlocks[j]->isWater() || cBlocks[i]->isWater()*/) { // Ceilings do not exclude themselves
        continue;
      }
      cBlocks[i]->addCeilingExclusion(cBlocks[j], i < j);
    }
  }

  // Here we do the walls (TODO right now we always get true.)
  for (int i = 0; i < cBlocks.size(); i++) {
    for (int j = 0; j < cBlocks.size(); j++) {

//      if (i > j && cBlocks[j]->isSpace()) { // Spaces have no effect on blocks that appear after them
//        continue;
//      }
      if (cBlocks[i]->obscuresWallSurface(*cBlocks[j], i < j) && !cBlocks[i]->isWater() && !cBlocks[j]->isWater()) {
        cBlocks[i]->addWallExclusion(cBlocks[j], i < j);
      }
    }
    // Get blocks from cCollisionZones zones too
    for (int j = 0; j < cCollisionZones.size(); j++) {
      for (int k = 0; k < cCollisionZones[j]->cBlocks.size(); k++) {
        if (cBlocks[i]->obscuresWallSurface(*(cCollisionZones[j]->cBlocks[k]), i < k) && !cBlocks[i]->isWater() && !cCollisionZones[j]->cBlocks[k]->isWater()) {
          cBlocks[i]->addWallExclusion(cCollisionZones[j]->cBlocks[k], i < k);
        }        
      }
    }
  }
}


// TODO Don't add exclusion zones that exist entirely within spaces.
