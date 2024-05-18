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

#include <functional>
#include <memory>
#include <set>
#include <stack>

#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/Collision/CollisionUtils.h"
#include "IsoRealms/Input/HatHandler.h"
#include "IsoRealms/Property/PropertyAppearance.h"
#include "IsoRealms/Struct/SpatialContainer2D.h"
#include "IsoRealms/System.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/Utils.h"

#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"
#include "Modules/Spindizzy/World/World.h"
#include "Modules/Spindizzy/WorldEditorCursorCell.h"

#include "TerrainBrush.h"

namespace IsoRealms::Spindizzy {
  class WorldEditor : public IEditableScreen {
    public:
    WorldEditor(IAssetRegistry* assets, World* world);
    void updateScreen(unsigned int milliseconds);
    World* getWorld() const;
    TerrainBrush& getTerrainBrush();
    const TerrainBrush& getTerrainBrush() const;
    WorldEditorCursorCell getCursorCell();
    LiteralVertex& getCursorLocation();
    float getCursorX() const;
    float getCursorY() const;
    float getCursorZ() const;
    IPropertyAppearance* getPropertyAppearance();

    /******************************\
     * Implements IEditableScreen *
    \******************************/
    bool input(sf::Event& event) override;
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void notifyVisible() override;
    void notifyHidden() override;
    void setAppearance(IFont* font, float scale) override;
    void unregisterAssets(IAssetRemover* assets) override;
    const IFloat* getYaw() const override;
    const IFloat* getPitch() const override;
    IScreen* screen() override;

    private:
    
    class ScreenFloat : public IFloat {
      private:
      double* cValue;

      public:
      ScreenFloat(double* value);

      /*********************\
       * Implements IFloat *
      \*********************/
      float getValue() const override;
      bool renderAssetIcon() const override;
      void saveAsset(JSONObject object) const override;
    };

    static const float SPEED_FAST;
    static const float SPEED_NORMAL;
    static const float SPEED_SLOW;
    static const float STOP_THRESHOLD;
    static const float SELECTION_BOUNDARY_RENDERING_OFFSET;

    static const float BORDER_SPACING;
    static const float ICON_WIDTH;
    static const float ICON_HEIGHT;
    static const float BOTTOM_BORDER;
    static const float ICON_SPACING;

    int cDefAnalogueSensitivity;

    int cXDirection;
    int cYDirection;
    int cZDirection;

    double cPitchSpeed;
    double cYawSpeed;
    double cXSpeed;
    double cYSpeed;
    double cZSpeed;
    double cDistanceInSpeed;
    double cDistanceOutSpeed;

    double cDistance;
    double cRotation;                       
    double cTilt;
    double cRoll;
    ScreenFloat cScreenYaw;
    ScreenFloat cScreenPitch;
    IStateNotifier<IFloat>* cScreenYawNotifier;
    IStateNotifier<IFloat>* cScreenPitchNotifier;
    
    bool cActiveLeft;
    bool cActiveRight;
    bool cActiveUp;
    bool cActiveDown;
    bool cActiveHigher;
    bool cActiveLower;
    bool cActiveSlow;
    bool cActiveFast;
    HatHandler& cHatHandler;
    LiteralVertex cLocation;
    LiteralVertex cMomentum;
    IWorldEditorToolInstance* cSelectedTool;
    AnimatedFloat cPaletteSelectionX;
    TerrainBrush cTerrainBrush;

    bool cRotatingView;
    bool cZoomingView;
    int cPreviousX;
    int cPreviousY;

    std::vector<IWorldEditorToolInstance*> cTools;
    World* cWorld;
    std::set<IVisualElement*> cEditingVisuals;

    PropertyAppearance cPropertyAppearance;

    IScreen* cProxyScreen;

    bool isMovingWest();
    bool isMovingEast();
    bool isMovingSouth();
    bool isMovingNorth();
    float getMovementSpeed();
    bool inputCamera(sf::Event&);
    float getAngle();
    void resetEditingView();
    void move(float amount);
    void rotate(float yaw, float pitch);
    void move(float x, float y, float z);
    
    void selectToolRelative(int amount);
    void setPreviousTheme();
    void setNextTheme();
  };
}
