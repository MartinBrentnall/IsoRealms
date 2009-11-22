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
#include "Map.h"

Map::Map() {
  cCurrentZone = NULL;
}

Map::~Map() {
  for (int i = 0; i < cZones.size(); i++) {
    delete cZones[i];
  }
}

int Map::getZoneCount() {
  return cZones.size();
}

int Map::getJewelCount() {
  int mJewelCount = 0;
  for (int i = 0; i < cZones.size(); i++) {
    mJewelCount += cZones[i]->getJewelCount();
  }
  return mJewelCount;
}

void Map::initialiseForEditor() {
  for (int i = 0; i < cZones.size(); i++) {
    cZones[i]->initForEditor();
  }
}

void Map::deleteZone(Point p) {
  Zone *tmp;

  int zoneIndex = getZone(p);
  if (zoneIndex >= 0) {
    for (int i = zoneIndex; i < cZones.size() - 1; i++) {
      tmp = cZones[i];
      cZones[i] = cZones[i + 1];
      cZones[i + 1] = tmp;
    }
    cZones.pop_back();
  }
}

Point Map::getStartPoint() {
  return cCurrentZone->getStartPoint();
}

Point Map::getEndPoint() {
  return cCurrentZone->getEndPoint();
}

void Map::addBlock(Block& block) {
  cCurrentZone->addBlock(block, true);
}

void Map::addJewel(Jewel& jewel) {
  cCurrentZone->addJewel(jewel);
}

void Map::setCurrentZoneColourScheme(TextureSet& textures) {
  cCurrentZone->setColours(textures);
}

TextureSet& Map::getCurrentZoneColourScheme() {
  return cCurrentZone->getColourScheme();
}

void Map::setActiveZone(Point location) {
  cCurrentZone = getZoneAt(location);
  cCurrentZone->setBackground();
}

void Map::addZone(Zone& zone) {
  cZones.push_back(&zone);
}

void Map::addEnemy(Craft* craft) {
  cCurrentZone->addEnemy(craft);
}

void Map::addLift(Lift* lift) {
  cCurrentZone->addLift(lift);
}

void Map::toggleSlope() {
  cCurrentZone->toggleSlope();
}

void Map::init(TextureSet& textures, ColourScheme* colours) {
  Point location(0, 0, 0);
  Point dimension(8 ,8 ,8);
  createZone(location, dimension, textures, colours);
  if (cCurrentZone == NULL) {
    cCurrentZone = cZones[0];
  }
}

int Map::getPitDepth() {
  int mDepth = 0xFFFFFFFF;
  for (int i = 0; i < cZones.size(); i++) {
    if (cZones[i]->getStartPoint().z < mDepth) {
      mDepth = cZones[i]->getStartPoint().z;
    }
  }
  return mDepth - 10;
}

void Map::createZone(Point location, Point dimension, TextureSet& textures, ColourScheme* colours) {
  Zone* mNewZone = new Zone(location, dimension, textures, colours);
  cZones.push_back(mNewZone);
}

// We don't use this function in the runtime; the Runtime renders only the necessary Zones
void Map::renderScene(float angle, float tilt) {
  for (int i = 0; i < cZones.size(); i++) {
    cZones[i]->renderAsEditor(angle, tilt);
  }
  Craft mCraft(cStartLocation);
  glPushMatrix();
  glTranslatef(BLOCK_RADIUS, BLOCK_RADIUS, 0.0);
  mCraft.render(1.0, angle, tilt);
  glPopMatrix();
}

void Map::renderAsInGame(float alpha, int ticks) {
  for (int i = 0; i < cZones.size(); i++) {
    cZones[i]->renderAsMap(alpha, ticks);
  }
}

void Map::write(FILE *w, ColourSchemeSelector& colourSelection) {
  fprintf(w, "<START LOCATION %d %d %d>\n", cStartLocation.x, cStartLocation.y, cStartLocation.z);
  for (int i = 0; i < cZones.size(); i++) {
    cZones[i]->write(w, colourSelection);
  }
}

void Map::setStartLocation(Point& location) {
  // TODO: Don't set outside zone or at zone border
  cStartLocation.x = location.x;
  cStartLocation.y = location.y;
  cStartLocation.z = location.z;
}

Point Map::getStartLocation() {
  return cStartLocation;
}

// Initialises the map.  Calculates adjacent zones, etc.
void Map::initialise() {
  for (int i = 0; i < cZones.size(); i++) {
    for (int j = 0; j < cZones.size(); j++) {
      if (i == j) {
        continue;
      }
      if (cZones[i]->isAdjacentTo(*cZones[j])) {
        cZones[i]->addCollisionZone(cZones[j]);
      }
    }
  }
}

void Map::initialiseForRuntime() {
  for (int i = 0; i < cZones.size(); i++) {
    cZones[i]->resetExplored();
    for (int j = 0; j < cZones.size(); j++) {
      if (i == j) {
        continue;
      }
      if (cZones[i]->isNearby(*cZones[j])) {
        cZones[i]->initForRuntime();
        cZones[i]->addBackgroundZone(cZones[j]);
      }
    }
  }
}

Block* Map::popBlock() {
  return cCurrentZone->popBlock();
}

void Map::cacheZoneSurfaces() {
  for (int i = 0; i < cZones.size(); i++) {
    cZones[i]->cacheSurfaces();
  }
}

void Map::clearZoneCaches() {
  for (int i = 0; i < cZones.size(); i++) {
    cZones[i]->clearCaches();
  }
}

bool Map::isCurrentZoneEmpty() {
  return cCurrentZone->isEmpty();
}

void Map::load(FILE *r, ColourSchemeSelector& colourSchemes) {
  char current;
  char word[256];
  Point location;
  Point dimension;
  int mTexture;

  fscanf(r, "%s", word);
  if (strcmp("LOCATION", word) != 0) {
    printf("Unrecognised keyword %s in map file.\n");
    exit(1);
  }
  fscanf(r, "%d", &cStartLocation.x);
  fscanf(r, "%d", &cStartLocation.y);
  fscanf(r, "%d", &cStartLocation.z);
  fscanf(r, "%c", &current);
  while (!feof(r)) {
    fscanf(r, "%c", &current);
    if (current <= 32) {
      // Ignorable.
    } else if (current == '<') {
      fscanf(r, "%s", word);
      if (strcmp("ZONE", word) == 0) {
        while (!feof(r)) {
          fscanf(r, "%s", word);
          if (strcmp("LOCATION", word) == 0) {
            fscanf(r, "%d", &location.x);
            fscanf(r, "%d", &location.y);
            fscanf(r, "%d", &location.z);
            fscanf(r, "%s", word);
            if (strcmp("DIMENSIONS", word) == 0) {
              fscanf(r, "%d", &dimension.x);
              fscanf(r, "%d", &dimension.y);
              fscanf(r, "%d", &dimension.z);
              fscanf(r, "%s", word);
              fscanf(r, "%d", &mTexture);
              TextureSet *mTextures = &colourSchemes.getTextureSet(mTexture);
              ColourScheme *mColours = colourSchemes.getColourScheme(mTexture);              
              createZone(location, dimension, *mTextures, mColours);
              fscanf(r, "%c", &current);
              cZones[cZones.size() - 1]->load(r);
            } else {
              printf("Unrecognised keyword %s in map file.\n");
              exit(1);
            }
          } else {
            printf("Unrecognised keyword %s in map file.\n");
            exit(1);
          }
        }
      } else {
        printf("Unrecognised keyword %s in map file.\n");
        exit(1);
      }
    } else {
      printf("Error parsing map file in map (%c).\n", current);
      exit(1);
    }
  }

  initialise();

  for (int i = 0; i < cZones.size(); i++) {
//    printf("Setting up zone %d.\n", i);
    cZones[i]->updateExclusions();
  }  

  if (cCurrentZone == NULL) {
    cCurrentZone = cZones[0];
  }
}

