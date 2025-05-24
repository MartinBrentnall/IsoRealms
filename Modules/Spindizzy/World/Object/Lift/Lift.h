/*
 * Copyright 2023 Martin Brentnall
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
#pragma once

#include <algorithm>
#include <cmath>
#include <GL/glew.h>
#include <iomanip>

#include "IsoRealms/Collision/CollisionUtils.h"
#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/IAssetBrowser.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/ISurface.h"
#include "Modules/Spindizzy/IWorldObject.h"
#include "Modules/Spindizzy/World/Common/CollisionData.h"

namespace IsoRealms::Spindizzy {
  class LiftType;
  
  /**
   * World object for a vertically moving platform.  Whenever the type of this
   * lift is active, it will continuously move up and down, pausing at its
   * highest and lowest points.
   */
  class Lift : public IWorldObject {
    public:
    
    // Constructors.
    Lift(Zone& zone, LiftType& type, int x, int y, int z, int bottom, int top);
    Lift(Zone& zone, Lift& lift, int x, int y, int z);
    Lift(Zone& zone, JSONObject object);

    // Interface for use by parent zone.
    void initialise();
    void reset();
    void save(JSONObject object, int x, int y, int z);
    bool isType(const LiftType* const type) const;
    void updateRuntime(unsigned int milliseconds);
    void updateEditing(unsigned int milliseconds);
    void renderRuntime() const;
    void renderEditing() const;

    /***************************\
     * Implements IWorldObject *
    \***************************/
    bool contains(const LiteralVertex& location) const override;
    void renderSelectionHighlight() const override;
    void remove() override;
    std::vector<std::unique_ptr<IProperty>> getProperties() override;
    std::string getTypeName() const override;
    Zone& getObjectZone() override;

    private:
    
    // Types.
    enum class State {
      START_UP_DELAY,
      START_UP_MOVING_UP,
      START_UP_MOVING_DOWN,
      MOVING_UP,
      MOVING_DOWN,
      PAUSED_TOP,
      PAUSED_BOTTOM
    };

    // JSON members.
    static const std::string JSON_BOTTOM;
    static const std::string JSON_BOTTOM_PAUSE;
    static const std::string JSON_DOWN_SPEED;
    static const std::string JSON_TOP;
    static const std::string JSON_TOP_PAUSE;
    static const std::string JSON_TYPE;
    static const std::string JSON_UP_SPEED;
    static const std::string JSON_X;
    static const std::string JSON_Y;
    static const std::string JSON_Z;

    // External interfaces.
    Zone& cZone; /// Zone to which this lift belongs.
    
    // Definition values
    LiftType* cDefType;                       /// Type of this lift.
    std::unique_ptr<ModelInstance> cDefModel; /// Visual representation of this lift.
    int cDefX;                                /// X position of this lift.
    int cDefY;                                /// Y position of this lift.
    int cDefZ;                                /// Starting Z position of this lift.
    int cDefTop;                              /// Highest Z position of this lift.
    int cDefBottom;                           /// Lowest Z position of this lift.
    int cDefTopPause;                         /// Time in milliseconds to pause at the highest Z position.
    int cDefBottomPause;                      /// Time in milliseconds to pause at the lowest Z position.
    int cDefSpeedUp;                          /// Speed at which this lift moves upwards.
    int cDefSpeedDown;                        /// Speed at which this lift moves downwards.
    
    // Runtime values
    struct LiftValues {
      State cState;        /// The current state of this lift.
      unsigned int cDelay; /// The current time in milliseconds to remain paused before moving again.
    } cRuntimeState;
    
    // TODO: Is this necessary?
    class LiftSurfaceEvent {
      public:
      Lift& cParent;
      LiftValues cState;
      double cTime;
      
      public:
      LiftSurfaceEvent(Lift& parent, LiftValues state, double time);
    };
    
    class Surface : public ISurface {
      public:
      Surface(Lift& parent);
      
      /***********************\
       * Implements ISurface *
      \***********************/
      bool contains(LiteralVertex&, float) override;
      std::unique_ptr<CollisionData> getCollision(LiteralVertex&, LiteralVertex&, float, double startTime, double endTime) override;
      std::unique_ptr<CollisionData> getRollingEvent(LiteralVertex&, LiteralVertex&, double startTime, double endTime) override;
      float getHeightAt(float x, float y) const override;
      float getXAcceleration(float, float) override;
      float getYAcceleration(float, float) override;
      void notifyContact() override;
      void notifyImpact() override;
      float getSurfaceFriction() override;
      float getSurfaceGrip() override;
      float getSurfaceBounce() override;
      Respawn isRespawnAllowed() override;
      bool isRespawnPossible() override;
      void getRestingLocation(LiteralVertex&) override;
      void saveCache(std::ostream& cache, bool physical) override;
      Zone& getZone() override;
      bool isSolid() override;
      void adjustPosition(LiteralVertex& location, double milliseconds) override;
      int getXStart() const override;
      int getXEnd() const override;
      int getYStart() const override;
      int getYEnd() const override;
      IWorldObject* getOwner() override;
      void render() const override;
      void renderOutline() const override;
      void renderHighlight() const override;
      int getSurfaceCellHeight(int x, int y) const override; 
      int getSurfaceCellElevation(int x, int y) const override;
      bool alligned(int x, int y) const override;
      std::vector<std::unique_ptr<IVisualElement>> getStaticVisuals() override;    
      
      private:
      Lift& cParent;
    } cSurface;
    
    void renderEditingArrow() const;
    std::unique_ptr<LiteralVertex> getBoundaryCrossingPoint(LiteralVertex&, LiteralVertex&, float*, float);
    double getHeight(LiftValues state) const;
    std::unique_ptr<CollisionData> getCollision(Surface* surface, LiteralVertex& start, LiteralVertex& end, LiteralVertex& actualStart, LiteralVertex& actualEnd, float height, bool down);
    std::unique_ptr<LiftSurfaceEvent> getEvent(double startTime, double endTime);
    LiftValues getZLocationAfter(unsigned int milliseconds);
  };
}
