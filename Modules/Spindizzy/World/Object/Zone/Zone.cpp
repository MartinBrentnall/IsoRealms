/*
 * Copyright 2025 Martin Brentnall
 *
 * This file is part of IsoRealms.
 *
 * IsoRealms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * IsoRealms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IsoRealms.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Zone.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/TerrainType/TerrainType.h"
#include "Modules/Spindizzy/World/Object/Terrain/Terrain.h"
#include "Modules/Spindizzy/World/World.h"
#include "Modules/Spindizzy/ZoneType/ZoneType.h"

namespace IsoRealms::Spindizzy {
  const std::string Zone::JSON_ALIENS    = "aliens";
  const std::string Zone::JSON_HEIGHT    = "height";
  const std::string Zone::JSON_LIFTS     = "lifts";
  const std::string Zone::JSON_LENGTH    = "length";
  const std::string Zone::JSON_OBJECTS   = "objects";
  const std::string Zone::JSON_PICK_UPS  = "pickUps";
  const std::string Zone::JSON_TERRAIN   = "terrain";
  const std::string Zone::JSON_THEME     = "theme";
  const std::string Zone::JSON_THEME_SET = "themeSet";
  const std::string Zone::JSON_TYPE      = "type";
  const std::string Zone::JSON_VISITED   = "visited";
  const std::string Zone::JSON_WIDTH     = "width";
  const std::string Zone::JSON_X         = "x";
  const std::string Zone::JSON_Y         = "y";
  const std::string Zone::JSON_Z         = "z";

  Zone::Zone(World& world, ZoneType& type, int xStart, int yStart, int zStart, int xEnd, int yEnd, int zEnd, Zone* clone) :
            cDefWorld(world),
            cDefType(&type),
            cDefStartX(std::min(xStart, xEnd)),
            cDefEndX(  std::max(xStart, xEnd)),
            cDefStartY(std::min(yStart, yEnd)),
            cDefEndY(  std::max(yStart, yEnd)),
            cDefStartZ(std::min(zStart, zEnd)),
            cDefEndZ(  std::max(zStart, zEnd)),
            cDefThemeSet(clone != nullptr ? clone->cDefThemeSet : cDefWorld.getDefaultThemeSet()),
            cDefTheme(clone != nullptr ? clone->cDefTheme : cDefThemeSet->getDefaultTheme()),
            cDefVisited(clone != nullptr ? clone->cDefVisited : false) {
    if (clone != nullptr) {
      int mXDifference = cDefStartX - clone->cDefStartX;
      int mYDifference = cDefStartY - clone->cDefStartY;
      int mZDifference = cDefStartZ - clone->cDefStartZ;

      for (std::unique_ptr<Alien>& mAlien : clone->cDefAliens) {
        cDefAliens.emplace_back(std::make_unique<Alien>(*this, *mAlien.get(), mXDifference, mYDifference, mZDifference));
      }
      for (std::unique_ptr<Lift>& mLift : clone->cDefLifts) {
        cDefLifts.emplace_back(std::make_unique<Lift>(*this, *mLift.get(), mXDifference, mYDifference, mZDifference));
      }
      for (std::unique_ptr<PickUp>& mPickUp : clone->cDefPickUps) {
        cDefPickUps.emplace_back(std::make_unique<PickUp>(*this, *mPickUp.get(), mXDifference, mYDifference, mZDifference));
      }
      for (std::unique_ptr<Terrain>& mTerrain : clone->cDefTerrain) {
        cDefTerrain.emplace_back(std::make_unique<Terrain>(*this, *mTerrain.get(), mXDifference, mYDifference, mZDifference));
      }
      // TODO: Zone Objects
    }

    reset();
  }

  Zone::Zone(World& world, JSONObject object) :
            cDefWorld(world),
            cDefType(nullptr),
            cDefStartX(object.getInteger(JSON_X)),
            cDefEndX(object.getInteger(JSON_WIDTH)  + cDefStartX - 1),
            cDefStartY(object.getInteger(JSON_Y)),
            cDefEndY(object.getInteger(JSON_LENGTH) + cDefStartY - 1),
            cDefStartZ(object.getInteger(JSON_Z)),
            cDefEndZ(object.getInteger(JSON_HEIGHT) + cDefStartZ - 1),
            cDefThemeSet(nullptr),
            cDefTheme(nullptr),
            cDefVisited(object.getBoolean(JSON_VISITED)) {
    if (object.hasMember(JSON_ALIENS)) {
      for (JSONObject mAlienObject : object.getArray(JSON_ALIENS)) {
        cDefAliens.emplace_back(std::make_unique<Alien>(*this, mAlienObject));
      }
    }
    if (object.hasMember(JSON_LIFTS)) {
      for (JSONObject mLiftObject : object.getArray(JSON_LIFTS)) {
        cDefLifts.emplace_back(std::make_unique<Lift>(*this, mLiftObject));
      }
    }
    if (object.hasMember(JSON_OBJECTS)) {
      for (JSONObject mObjectObject : object.getArray(JSON_OBJECTS)) {
        cDefObjects.emplace_back(std::make_unique<ZoneObject>(*this, mObjectObject));
      }
    }
    if (object.hasMember(JSON_PICK_UPS)) {
      for (JSONObject mPickUpObject : object.getArray(JSON_PICK_UPS)) {
        cDefPickUps.emplace_back(std::make_unique<PickUp>(*this, mPickUpObject));
      }
    }
    if (object.hasMember(JSON_TERRAIN)) {
      for (JSONObject mTerrainObject : object.getArray(JSON_TERRAIN)) {
        addTerrain(std::make_unique<Terrain>(*this, mTerrainObject));
      }
    }
    cDefWorld.getSpindizzy().getProject().init([this, object](IAssets& assets) {
      cDefType = cDefWorld.getSpindizzy().getZoneType(object.getString(JSON_TYPE));
      std::string mThemeSet = object.getString(JSON_THEME_SET);
      std::string mThemeName = object.getString(JSON_THEME);
      cDefThemeSet = mThemeSet != "" ? cDefWorld.getSpindizzy().getThemeSet(mThemeSet) : nullptr;
      cDefTheme = mThemeName != "" ? cDefThemeSet->getTheme(mThemeName) : nullptr;
      reset();

      // Refresh terrain states.
      cDefTerrainStates.clear();
      for (std::unique_ptr<Terrain>& mTerrain : cDefTerrain) {
        addTerrainState(mTerrain.get());
      }
    });
  }

  void Zone::registerAssets() {
    for (std::unique_ptr<ZoneObject>& mObject : cDefObjects) {
      mObject->registerAssets();
    }
  }
  
  Alien* Zone::draw(AlienType& type, const WorldEditorCursorCell& cell) {
    if (cell.cDefX >= cDefStartX && cell.cDefX <= cDefEndX && cell.cDefY >= cDefStartY && cell.cDefY <= cDefEndY && cell.cDefZ >= cDefStartZ && cell.cDefZ <= cDefEndZ) {
      return cDefAliens.emplace_back(std::make_unique<Alien>(*this, type, cell.cDefX, cell.cDefY, cell.cDefZ)).get();
    }
    return nullptr;
  }
  
  Lift* Zone::draw(LiftType& type, const WorldEditorCursorCell& cell, int bottomRange, int topRange) {
    if (cell.cDefX >= cDefStartX && cell.cDefX <= cDefEndX && cell.cDefY >= cDefStartY && cell.cDefY <= cDefEndY && cell.cDefZ >= cDefStartZ && cell.cDefZ <= cDefEndZ && bottomRange >= cDefStartZ && bottomRange <= cDefEndZ && topRange >= cDefStartZ && topRange <= cDefEndZ) {
      return cDefLifts.emplace_back(std::make_unique<Lift>(*this, type, cell.cDefX, cell.cDefY, cell.cDefZ, bottomRange, topRange)).get();
    }
    return nullptr;
  }
  
  PickUp* Zone::draw(PickUpType& type, const WorldEditorCursorCell& cell) {
    if (cell.cDefX >= cDefStartX && cell.cDefX <= cDefEndX && cell.cDefY >= cDefStartY && cell.cDefY <= cDefEndY && cell.cDefZ >= cDefStartZ && cell.cDefZ <= cDefEndZ) {
      return cDefPickUps.emplace_back(std::make_unique<PickUp>(*this, type, cell.cDefX, cell.cDefY, cell.cDefZ)).get();
    }
    return nullptr;
  }
  
  Terrain* Zone::draw(TerrainType& type, const WorldEditorCursorCell& start, const WorldEditorCursorCell& end, int southWestHeight, int southEastHeight, int northWestHeight, int northEastHeight, bool alternativeSplit, bool steppedBase, bool negation) {
    if (isValidTerrainPlacement(start, end, southWestHeight, southEastHeight, northWestHeight, northEastHeight)) {
      return addTerrain(std::make_unique<Terrain>(*this, type, start.cDefX, start.cDefY, start.cDefZ, end.cDefX, end.cDefY, end.cDefZ, southWestHeight, southEastHeight, northWestHeight, northEastHeight, alternativeSplit, steppedBase, !negation));
    }
    return nullptr;
  }
  
  ZoneObject* Zone::draw(ZoneObjectType& type) {
// TODO     if (type->isValidPlacement()) {
//       return cDefObjects.emplace_back(std::make_unique<ZoneObject>(*this, type));
//     }
    return nullptr;
  }
  


























  void Zone::registerView(IScreen& screen) {
    cRuntimeDisplayLists[&screen] = 0;
  }
  
  unsigned int Zone::getHintCount() {
    unsigned int mCount = 0;
    for (unsigned int i = 0; i < cDefTerrainStates.size(); i++) {
      if (!cDefTerrainStates[i]->getValue()) {
        mCount++;
      }
    }
    return mCount;
  }

  void Zone::giveHint(unsigned int index) {
    unsigned int mHintCount = 0;
    for (unsigned int i = 0; i < cDefTerrainStates.size(); i++) {
      if (!cDefTerrainStates[i]->getValue()) {
        if (mHintCount == index) {
          cDefTerrainStates[i]->giveHint();
          return;
        }
        mHintCount++;
      }
    }
  }

  unsigned int Zone::getRemainingPickUpCount() {
    unsigned int mPickUpCount = 0;
    for (std::unique_ptr<PickUp>& mPickUp : cDefPickUps) {
      if (!mPickUp->isCollected()) {
        mPickUpCount++;
      }
    }
    return mPickUpCount;
  }

  bool Zone::isType(const ZoneType* const type) const {
    return cDefType == type;
  }
  
  Terrain* Zone::addTerrain(std::unique_ptr<Terrain> terrain) {
    Terrain* mTerrain = cDefTerrain.emplace_back(std::move(terrain)).get();
    flagForInitialisation(mTerrain);
    addTerrainState(mTerrain);
    return mTerrain;
  }

  void Zone::addTerrainState(Terrain* terrain) {
    std::optional<Condition>& mCondition = terrain->getCondition();
    if (mCondition.has_value()) {
      std::set<IBoolean*> mInputs = mCondition->getInputs();
      for (std::set<IBoolean*>::iterator i = mInputs.begin(); i != mInputs.end(); i++) {
        TerrainState* mTerrainState = cDefWorld.getSpindizzy().getTerrainState(*i);
        bool mExists = false;
        for (unsigned int j = 0; j < cDefTerrainStates.size(); j++) {
          if (cDefTerrainStates[j] == mTerrainState) {
            mExists = true;
            break;
          }
        }
        if (!mExists) {
          cDefTerrainStates.push_back(mTerrainState);
        }
      }
    }
  }
  
  void Zone::remove(Terrain* terrain) {
    Utils::removeElementUnique(cDefTerrain, terrain);

    // Refresh terrain states.
    cDefTerrainStates.clear();
    for (std::unique_ptr<Terrain>& mTerrain : cDefTerrain) {
      addTerrainState(mTerrain.get());
    }

    if (empty()) {
      cDefWorld.remove(this);
    }
  }

  void Zone::remove(PickUp* pickUp) {
    Utils::removeElementUnique(cDefPickUps, pickUp);
    if (empty()) {
      cDefWorld.remove(this);
    }
  }

  void Zone::remove(Alien* alien) {
    Utils::removeElementUnique(cDefAliens, alien);
    if (empty()) {
      cDefWorld.remove(this);
    }
  }

  void Zone::remove(Lift* lift) {
    Utils::removeElementUnique(cDefLifts, lift);
    if (empty()) {
      cDefWorld.remove(this);
    }
  }

  void Zone::remove(ZoneObject* object) {
    Utils::removeElementUnique(cDefObjects, object);
    if (empty()) {
      cDefWorld.remove(this);
    }
  }

  void Zone::removeAll(AlienType* type) {
    for (int i = cDefAliens.size() - 1; i >= 0; i--) {
      if (cDefAliens[i]->isType(type)) {
        cDefAliens.erase(cDefAliens.begin() + i);
      }
    }
    if (empty()) {
      cDefWorld.remove(this);
    }
  }
  
  void Zone::removeAll(LiftType* type) {
    for (int i = cDefLifts.size() - 1; i >= 0; i--) {
      if (cDefLifts[i]->isType(type)) {
        cDefLifts.erase(cDefLifts.begin() + i);
      }
    }
    if (empty()) {
      cDefWorld.remove(this);
    }
  }
  
  void Zone::removeAll(PickUpType* type) {
    for (int i = cDefPickUps.size() - 1; i >= 0; i--) {
      if (cDefPickUps[i]->isType(type)) {
        cDefPickUps.erase(cDefPickUps.begin() + i);
      }
    }
    if (empty()) {
      cDefWorld.remove(this);
    }
  }
  
  void Zone::removeAll(TerrainType* type) {
    for (int i = cDefTerrain.size() - 1; i >= 0; i--) {
      if (cDefTerrain[i]->isType(type)) {
        cDefTerrain.erase(cDefTerrain.begin() + i);
      }
    }
    updateDisplayList();
    if (empty()) {
      cDefWorld.remove(this);
    }
  }
  
  void Zone::removeAll(ZoneObjectType* type) {
    for (int i = cDefObjects.size() - 1; i >= 0; i--) {
      if (cDefObjects[i]->isType(type)) {
        cDefObjects.erase(cDefObjects.begin() + 1);
      }
    }
    if (empty()) {
      cDefWorld.remove(this);
    }
  }
  
  World& Zone::getWorld() {
    return cDefWorld;
  }
    
  void Zone::updateBounds() {
    cRuntimeEndZ = cDefWorld.getMaxZoneHeight(cDefStartX, cDefEndX, cDefStartY, cDefEndY, cDefEndZ + 1, std::numeric_limits<int>::max());
  }
    
  unsigned int Zone::getOrderIndex(Terrain* terrain) {
    for (unsigned int i = 0; i < cDefTerrain.size(); i++) {
      if (cDefTerrain[i].get() == terrain) {
        return i;
      }
    }
    throw IllegalStateException("WARNING: Zone::getOrderIndex(): argument terrain is not a member of this zone");
  }

  void Zone::initialiseObjects() {
    cDefWorld.registerBoundary(cDefType, this, cDefStartX, cDefEndX, cDefStartY, cDefEndY);
    for (std::unique_ptr<PickUp>& mPickUp : cDefPickUps) {
      mPickUp->initialise();
    }
    for (std::unique_ptr<Lift>& mLift : cDefLifts) {
      mLift->initialise();
    }
  }

  void Zone::initialiseTerrain(std::ifstream& cache) {
    for (std::unique_ptr<Terrain>& mTerrain : cDefTerrain) {
      mTerrain->loadCachedSurfaces(cache);
    }
    cRuntimeTerrainToInitialise.clear();
  }

  void Zone::initialiseTerrain() {
    for (Terrain* mTerrain : cRuntimeTerrainToInitialise) {
      mTerrain->generateSurfaces();
    }
    cRuntimeTerrainToInitialise.clear();
    updateDisplayList();
  }
  
  void Zone::updateDisplayList() {
    std::vector<const IScreen*> mScreens;
    for (std::pair<const IScreen*, GLuint> mDisplayList : cRuntimeDisplayLists) {
      glDeleteLists(mDisplayList.second, 1);
      mScreens.push_back(mDisplayList.first);
    }
    for (const IScreen* mScreen : mScreens) {
      cRuntimeDisplayLists[mScreen] = 0;
    }
  }

  bool Zone::intersects(int xStart, int yStart, int zStart, int xEnd, int yEnd, int zEnd) const {
    return std::max(xStart, xEnd) >= cDefStartX && std::min(xStart, xEnd) <= cDefEndX
        && std::max(yStart, yEnd) >= cDefStartY && std::min(yStart, yEnd) <= cDefEndY
        && std::max(zStart, zEnd) >= cDefStartZ && std::min(zStart, zEnd) <= cDefEndZ;
  }

  void Zone::setActive(bool active) {
    cRuntimeSetActive += active ? 1 : -1;
  }

  unsigned int Zone::getPickUpCount() {
    return cDefPickUps.size();
  }

  void Zone::renderEditing(const IScreen* screen) const {
    if (cDefTheme != nullptr) {
      cDefTheme->set();
    }
    for (const std::unique_ptr<Terrain>&    mTerrain : cDefTerrain) {mTerrain->renderEditing();}
    for (const std::unique_ptr<Lift>&       mLift    : cDefLifts)   {mLift->renderEditing();}
    for (const std::unique_ptr<Alien>&      mAlien   : cDefAliens)  {mAlien->render();}
    for (const std::unique_ptr<PickUp>&     mPickUp  : cDefPickUps) {mPickUp->renderEditing();}
    for (const std::unique_ptr<ZoneObject>& mObject  : cDefObjects) {mObject->renderEditing();}
    renderStatic(screen);

    float x  = cDefStartX - 0.5f;
    float y  = cDefStartY - 0.5f;
    float z  = cDefStartZ * 0.5f - 0.5f;
    float xs = cDefEndX   + 0.5f;
    float ys = cDefEndY   + 0.5f;
    float zs = cDefEndZ   * 0.5f;
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    Utils::renderVolumeMarkers(x, xs, y, ys, z, zs, 0.5f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
    if (cDefThemeSet != nullptr) {
      cDefThemeSet->applyDefaultTheme();
    }
  }

  void Zone::renderSelectionHighlight() const {
    Utils::renderVolumeCuboid(cDefStartX - 0.501f, cDefEndX + 0.501f, cDefStartY - 0.501f, cDefEndY + 0.501f, (cDefStartZ - 1.0f) * 0.5f, (cDefEndZ + 2.0f) * 0.5f);
  }

  std::vector<std::unique_ptr<IVisualElement>> Zone::getStaticVisuals() const {
    std::vector<std::unique_ptr<IVisualElement>> mZoneVisuals;
//    mZoneVisuals.push_back(this);
    for (const std::unique_ptr<Terrain>& mTerrain : cDefTerrain) {
      std::vector<std::unique_ptr<IVisualElement>> mTerrainVisuals = mTerrain->getStaticVisuals();
      mZoneVisuals.insert(mZoneVisuals.end(), make_move_iterator(mTerrainVisuals.begin()), make_move_iterator(mTerrainVisuals.end()));
    }
    return mZoneVisuals;
  }

  bool Zone::isVisited() const {
    return cRuntimeVisited;
  }

  void Zone::setVisited() {
    cRuntimeVisited = true;
  }

  IColour* Zone::getColour(ThemeColour* themeColour) {
    return cDefTheme->getColour(themeColour);
  }

  void Zone::setTheme(Theme* theme) {
    cDefTheme = theme;
  }

  void Zone::setTheme(ThemeSet* themeSet) {
    cDefThemeSet = themeSet;
    cDefTheme = cDefThemeSet->getDefaultTheme();
    if (cDefTheme == nullptr) {
      std::vector<Theme*> mThemes = cDefThemeSet->getThemes();
      if (!mThemes.empty()) {
        cDefTheme = mThemes[0];
        updateDisplayList();
      }
    }
  }

  void Zone::setNextTheme() {
    std::vector<Theme*> mThemes = cDefThemeSet->getThemes();
    for (unsigned int i = 0; i < mThemes.size(); i++) {
      if (mThemes[i] == cDefTheme) {
        cDefTheme = i == mThemes.size() - 1 ? mThemes[0] : mThemes[i + 1];
        updateDisplayList();
        return;
      }
    }
  }

  void Zone::setPreviousTheme() {
    std::vector<Theme*> mThemes = cDefThemeSet->getThemes();
    for (unsigned int i = 0; i < mThemes.size(); i++) {
      if (mThemes[i] == cDefTheme) {
        cDefTheme = i == 0 ? mThemes[mThemes.size() - 1] : mThemes[i - 1];
        updateDisplayList();
        return;
      }
    }
  }

  void Zone::spindizzyZoneThemeEdited(Theme* theme) {
    if (cDefTheme == theme) {
      updateDisplayList();
    }
  }

  Theme* Zone::getTheme() {
    return cDefTheme;
  }

  void Zone::renderStatic(const IScreen* screen) const {
    std::map<const IScreen*, GLuint>::const_iterator mDisplayList = cRuntimeDisplayLists.find(screen);
    if (mDisplayList != cRuntimeDisplayLists.end()) {
      if (mDisplayList->second == 0) {
        std::vector<std::unique_ptr<IVisualElement>> mAllVisuals = getStaticVisuals();
        GLuint mNewDisplayList = glGenLists(1);
        cRuntimeDisplayLists[screen] = mNewDisplayList;
        glNewList(mNewDisplayList, GL_COMPILE_AND_EXECUTE);
        Utils::renderStaticVisuals(mAllVisuals);
        glEndList();
      } else {
        glCallList(mDisplayList->second);
      }
    } else {
      throw ArgumentException("ERROR: Zone::renderStatic: Specified screen must be registered with this zone first.");
    }
  }
  
  void Zone::renderRuntime(const IScreen* screen) const {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    if (cDefTheme != nullptr) {
      cDefTheme->set();
    }

    // Dynamic content
    for (const std::unique_ptr<Terrain>& mTerrain : cDefTerrain) { mTerrain->renderRuntime(); }
    if (cRuntimeActive > 0) {
      for (const std::unique_ptr<Lift>& mLift         : cDefLifts)   {mLift->renderRuntime();}
      for (const std::unique_ptr<Alien>& mAlien       : cDefAliens)  {mAlien->render();}
      for (const std::unique_ptr<ZoneObject>& mObject : cDefObjects) {mObject->renderRuntime();}
    }
    for (const std::unique_ptr<PickUp>& mPickUp : cDefPickUps) {mPickUp->renderRuntime();}
    renderStatic(screen);
    
    if (cDefTheme != nullptr) {
      cDefThemeSet->applyDefaultTheme();
    }
  }

  void Zone::updateEditing(unsigned int milliseconds) {
    for (std::unique_ptr<Lift>& mLift         : cDefLifts)   {mLift->updateEditing(milliseconds);}
    for (std::unique_ptr<Alien>& mAlien       : cDefAliens)  {mAlien->updateEditing(milliseconds);}
    for (std::unique_ptr<PickUp>& mPickUp     : cDefPickUps) {mPickUp->updateEditing(milliseconds);}
    for (std::unique_ptr<ZoneObject>& mObject : cDefObjects) {mObject->updateEditing(milliseconds);}
  }

  void Zone::updateRuntime(unsigned int milliseconds) {
    int mStillActive = cRuntimeActive + cRuntimeSetActive;
    if (cRuntimeActive > 0) {
      if (mStillActive > 0) {
        for (std::unique_ptr<Alien>& mAlien : cDefAliens) {mAlien->updateRuntime(milliseconds);}
      } else {
        for (std::unique_ptr<Alien>& mAlien : cDefAliens) {mAlien->updateVanish();}
      }
    } else {
      if (mStillActive > 0) {
        for (std::unique_ptr<Alien>& mAlien : cDefAliens) {mAlien->updateAppear();}
      }
    }
    cRuntimeActive = mStillActive;
    cRuntimeSetActive = 0;
    if (cRuntimeActive) {
      for (std::unique_ptr<Lift>& mLift   : cDefLifts)   {mLift->updateRuntime(milliseconds);}
    }
    for (std::unique_ptr<PickUp>& mPickUp : cDefPickUps) {mPickUp->updateRuntime(milliseconds);}
    for (std::unique_ptr<ZoneObject>& mObject : cDefObjects) {mObject->updateRuntime(milliseconds);}
  }

  void Zone::save(JSONObject object) {
    Spindizzy& mSpindizzy = cDefWorld.getSpindizzy();

    object.addString(JSON_TYPE, mSpindizzy.getID(cDefType));
    if (cDefThemeSet != nullptr) {
      object.addString(JSON_THEME_SET, mSpindizzy.getID(cDefThemeSet));
      object.addString(JSON_THEME,     cDefThemeSet->getName(cDefTheme));
    }
    object.addBoolean(JSON_VISITED, cDefVisited);
    object.addInteger(JSON_X,      cDefStartX);
    object.addInteger(JSON_Y,      cDefStartY);
    object.addInteger(JSON_Z,      cDefStartZ);
    object.addInteger(JSON_WIDTH,  (cDefEndX - cDefStartX) + 1);
    object.addInteger(JSON_LENGTH, (cDefEndY - cDefStartY) + 1);
    object.addInteger(JSON_HEIGHT, (cDefEndZ - cDefStartZ) + 1);

    JSONArray mTerrainArray = object.addArray(JSON_TERRAIN);
    for (std::unique_ptr<Terrain>& mTerrain : cDefTerrain) {
      JSONObject mTerrainObject = mTerrainArray.addObject();
      mTerrain->save(mTerrainObject, cDefStartX, cDefStartY, cDefStartZ);
    }

    if (!cDefLifts.empty()) {
      JSONArray mLiftsArray = object.addArray(JSON_LIFTS);
      for (std::unique_ptr<Lift>& mLift : cDefLifts) {
        JSONObject mLiftObject = mLiftsArray.addObject();
        mLift->save(mLiftObject, cDefStartX, cDefStartY, cDefStartZ);
      }
    }

    if (!cDefAliens.empty()) {
      JSONArray mAliensArray = object.addArray(JSON_ALIENS);
      for (std::unique_ptr<Alien>& mAlien : cDefAliens) {
        JSONObject mAlienObject = mAliensArray.addObject();
        mAlien->save(mAlienObject, cDefStartX, cDefStartY, cDefStartZ);
      }
    }

    if (!cDefPickUps.empty()) {
      JSONArray mPickupsArray = object.addArray(JSON_PICK_UPS);
      for (std::unique_ptr<PickUp>& mPickUp  : cDefPickUps) {
        JSONObject mPickUpObject = mPickupsArray.addObject();
        mPickUp->save(mPickUpObject, cDefStartX, cDefStartY, cDefStartZ);
      }
    }

    if (!cDefObjects.empty()) {
      JSONArray mObjectsArray = object.addArray(JSON_OBJECTS);
      for (std::unique_ptr<ZoneObject>& mObject : cDefObjects) {
        JSONObject mObjectObject = mObjectsArray.addObject();
        mObject->save(mObjectObject);
      }
    }
  }

  void Zone::saveCache(std::ostream& cache) {
    for (std::unique_ptr<Terrain>& mTerrain : cDefTerrain) {
      mTerrain->saveCachedSurfaces(cache);
    }
  }

  void Zone::flagForInitialisation(Terrain* terrain) {
    cRuntimeTerrainToInitialise.insert(terrain);
    cDefWorld.flagForInitialisation(this);
  }
  
  void Zone::setDefaultTheme() {
    if (cDefThemeSet != nullptr) {
      cDefThemeSet->setDefaultTheme(cDefTheme);
    }
  }

  void Zone::resetDynamics() {
    for (std::unique_ptr<Lift>& mLift         : cDefLifts)   {mLift->reset();}
    for (std::unique_ptr<Alien>& mAlien       : cDefAliens)  {mAlien->reset();}
    for (std::unique_ptr<ZoneObject>& mObject : cDefObjects) {mObject->reset();}
  }

  void Zone::resetLifts() {
    for (std::unique_ptr<Lift>& mLift         : cDefLifts)   {mLift->reset();}
  }

  void Zone::reset() {
    for (std::unique_ptr<PickUp>& mPickUp : cDefPickUps) {mPickUp->reset();}
    resetDynamics();
    cRuntimeVisited = cDefVisited;
    cRuntimeSetActive = -cRuntimeActive;
    updateRuntime(0);
  }

  int Zone::getStartX() const {
    return cDefStartX;
  }

  int Zone::getStartY() const {
    return cDefStartY;
  }

  int Zone::getStartZ() const {
    return cDefStartZ;
  }

  int Zone::getEndX() const {
    return cDefEndX;
  }

  int Zone::getEndY() const {
    return cDefEndY;
  }

  int Zone::getEndZ() const {
    return cDefEndZ;
  }

  bool Zone::isValidTerrainPlacement(const WorldEditorCursorCell& start, const WorldEditorCursorCell& end, int southWestHeight, int southEastHeight, int northWestHeight, int northEastHeight) const {
    int mXSlope = Spindizzy::getTerrainSlope(southWestHeight, northWestHeight, southEastHeight, northEastHeight);
    int mYSlope = Spindizzy::getTerrainSlope(southWestHeight, southEastHeight, northWestHeight, northEastHeight);
    return std::max(start.cDefZ, end.cDefZ) + std::abs(mXSlope * (end.cDefX - start.cDefX)) + std::abs(mYSlope * (end.cDefY - start.cDefY)) <= cDefEndZ;
  }

  void Zone::processCursorMovement(LiteralVertex& cursor) {
    cursor.x = std::clamp(cursor.x, static_cast<double>(cDefStartX), static_cast<double>(cDefEndX));
    cursor.y = std::clamp(cursor.y, static_cast<double>(cDefStartY), static_cast<double>(cDefEndY));
    cursor.z = std::clamp(cursor.z, static_cast<double>(cDefStartZ), static_cast<double>(cDefEndZ));
  }

  bool Zone::contains(LiteralVertex& location) const {
    float mWest   = cDefStartX   - 0.5f;
    float mSouth  = cDefStartY   - 0.5f;
    float mBottom = cDefStartZ   - 1.0f;
    float mEast   = cDefEndX     + 0.5f;
    float mNorth  = cDefEndY     + 0.5f;
    float mTop    = cRuntimeEndZ + 0.5f;
    if (CollisionUtils::contains(location, mWest, mEast, mSouth, mNorth, mBottom, mTop)) {
      return true;
    }
    return false;
  }

  bool Zone::isEntered(LiteralVertex& start, LiteralVertex& end) const {
    float mWest   = cDefStartX   - 0.5f;
    float mSouth  = cDefStartY   - 0.5f;
    float mBottom = cDefStartZ   - 1.0f;
    float mEast   = cDefEndX     + 0.5f;
    float mNorth  = cDefEndY     + 0.5f;
    float mTop    = cRuntimeEndZ + 0.5f;
    bool mContainsStart = CollisionUtils::contains(start, mWest, mEast, mSouth, mNorth, mBottom, mTop);
    bool mContainsEnd   = CollisionUtils::contains(end,   mWest, mEast, mSouth, mNorth, mBottom, mTop);
    if (!mContainsStart && mContainsEnd) { // One event
      return true;
    } else if (!mContainsStart && !mContainsEnd) { // Two events or no events
      std::optional<IsoRealms::CollisionData> mEntryVertex = CollisionUtils::getCrossingPoint(start, end, mWest, mEast, mSouth, mNorth, mBottom, mTop, true);
      if (mEntryVertex) {
        return true;
      }
    }
    return false;
  }

  bool Zone::isExited(LiteralVertex& start, LiteralVertex& end) const {
    float mWest   = cDefStartX   - 0.5f;
    float mSouth  = cDefStartY   - 0.5f;
    float mBottom = cDefStartZ   - 1.0f;
    float mEast   = cDefEndX     + 0.5f;
    float mNorth  = cDefEndY     + 0.5f;
    float mTop    = cRuntimeEndZ + 0.5f;
    bool mContainsStart = CollisionUtils::contains(start, mWest, mEast, mSouth, mNorth, mBottom, mTop);
    bool mContainsEnd   = CollisionUtils::contains(end,   mWest, mEast, mSouth, mNorth, mBottom, mTop);
    if (mContainsStart && !mContainsEnd) { // One event
      return true;
    } else if (!mContainsStart && !mContainsEnd) { // Two events or no events
      std::optional<IsoRealms::CollisionData> mEntryVertex = CollisionUtils::getCrossingPoint(start, end, mWest, mEast, mSouth, mNorth, mBottom, mTop, true);
      if (mEntryVertex) {
        return true;
      }
    }
    return false;
  }

  bool Zone::aligned(int x, int y) const {
    return x >= cDefStartX && x <= cDefEndX && y >= cDefStartY && y <= cDefEndY;
  }

  bool Zone::contains(int x, int y, int z) const {
    return x >= cDefStartX && x <= cDefEndX && y >= cDefStartY && y <= cDefEndY && z >= cDefStartZ && z <= cDefEndZ;
  }

  void Zone::addProperty(IZoneProperty* property) {
    cRuntimeZoneProperties.insert(property);
  }

  void Zone::bindValues() {
    cDefWorld.getSpindizzy().bind(this);
    for (IZoneProperty* mProperty : cRuntimeZoneProperties) {
      mProperty->bindProperty();
    }
  }

  void Zone::bindValues2(Wall* wall) {
    cDefWorld.getSpindizzy().bind(this);
    cDefWorld.getSpindizzy().bind(wall);
    for (IZoneProperty* mProperty : cRuntimeZoneProperties) {
      mProperty->bindProperty2();
    }
  }

  void Zone::unbindValues() {
    cDefWorld.getSpindizzy().bind(static_cast<Zone*>(nullptr));
    cDefWorld.getSpindizzy().bind(static_cast<Wall*>(nullptr));
    for (IZoneProperty* mProperty : cRuntimeZoneProperties) {
      mProperty->unbindProperty();
    }
  }

  void Zone::unbindValues2() {
    cDefWorld.getSpindizzy().bind(static_cast<Zone*>(nullptr));
    for (IZoneProperty* mProperty : cRuntimeZoneProperties) {
      mProperty->unbindProperty2();
    }
  }

  void Zone::render() {
    // Nothing to do
  }

  ITexture* Zone::getTexture() {
    return nullptr;
  }

  void Zone::prepareVisual() {
    if (cDefTheme != nullptr) {
      cDefTheme->set();
    }
  }

  void Zone::selectObjects(LiteralVertex* start, LiteralVertex& end, std::function<bool(IWorldObject*)> condition, std::function<void(IWorldObject*)> select) {
    for (std::unique_ptr<Terrain>& mTerrain : cDefTerrain) {
      if ((start == nullptr || !mTerrain->contains(*start)) && mTerrain->contains(end) && condition(mTerrain.get())) {
        select(mTerrain.get());
      }
    }
    for (std::unique_ptr<Lift>& mLift : cDefLifts) {
      if ((start == nullptr || !mLift->contains(*start)) && mLift->contains(end)) {
        select(mLift.get());
      }
    }
    for (std::unique_ptr<Alien>& mAlien : cDefAliens) {
      if ((start == nullptr || !mAlien->contains(*start)) && mAlien->contains(end)) {
        select(mAlien.get());
      }
    }
    for (std::unique_ptr<PickUp>& mPickUp : cDefPickUps) {
      if ((start == nullptr || !mPickUp->contains(static_cast<const LiteralVertex&>(*start))) && mPickUp->contains(static_cast<const LiteralVertex&>(end))) {
        select(mPickUp.get());
      }
    }
    for (std::unique_ptr<ZoneObject>& mObject : cDefObjects) {
      if ((start == nullptr || !mObject->contains(*start)) && mObject->contains(end)) {
        select(mObject.get());
      }
    }
  }

  ZoneType* Zone::getType() {
    return cDefType;
  }

  bool Zone::empty() const {
    return cDefTerrain.empty() && cDefAliens.empty() && cDefPickUps.empty() && cDefLifts.empty() && cDefObjects.empty();
  }

  Zone::~Zone() {
    cDefWorld.unregisterBoundary(cDefType, this);
    while (!cDefTerrain.empty()) {
      cDefTerrain[0]->remove();
    }
  }
}
