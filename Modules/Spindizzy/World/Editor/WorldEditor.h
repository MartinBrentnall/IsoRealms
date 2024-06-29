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

#include "SignalInputID.h"
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
    void resetInput() override;
    void renderScreen(float scale, float aspectRatio) const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void notifyVisible() override;
    void notifyHidden() override;
    void notifyLostFocus() override;
    void notifyGainedFocus() override;
    std::vector<std::string> getDigitalInputs() const override;
    std::vector<std::string> getAnalogueInputs() const override;
    void setDigitalInput(const std::string& name, IBoolean* input) override;
    void setAnalogueInput(const std::string& name, IFloat* input) override;
    void setExitAction(IAction* action) override;
    bool signal(SignalInputID id);

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

    class AnalogueInput {
      public:
      AnalogueInput() :
                cInput(nullptr) {
      }

      void set(IFloat* input) {
        cInput = input;
      }

      float get() {
        return cInput != nullptr ? cInput->getValue() : 0.0f;
      }

      private:
      IFloat* cInput;
    };

    class DigitalInput {
      public:
      DigitalInput(WorldEditor& parent, SignalInputID signal) :
                cParent(parent),
                cInput(nullptr),
                cPreviousValue(false),
                cSignal(signal) {
      }

      void set(IBoolean* input) {
        cInput = input;
      }

      bool get() {
        return cInput != nullptr ? cInput->getValue() : false;
      }

      bool triggerOnChange() {
        bool mCurrentValue = get();
        bool mSignalled = false;
        if (mCurrentValue && !cPreviousValue) {
          if (cParent.signal(cSignal)) {
            mSignalled = true;
          }
        }
        cPreviousValue = mCurrentValue;
        return mSignalled;
      }

      private:
      WorldEditor& cParent;
      IBoolean* cInput;
      bool cPreviousValue;
      SignalInputID cSignal;
    };

    const std::map<std::string, AnalogueInput*> cAnalogueInputsByName; /// Mapping of digital inputs by name.
    const std::map<std::string, DigitalInput*> cDigitalInputsByName; /// Mapping of digital inputs by name.

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

    AnalogueInput cPitchSpeed;
    AnalogueInput cYawSpeed;
    AnalogueInput cXSpeed;
    AnalogueInput cYSpeed;
    AnalogueInput cZSpeed;
    AnalogueInput cDistanceInSpeed;
    AnalogueInput cDistanceOutSpeed;

    DigitalInput cActiveLeft;
    DigitalInput cActiveRight;
    DigitalInput cActiveUp;
    DigitalInput cActiveDown;
    DigitalInput cActiveHigher;
    DigitalInput cActiveLower;
    DigitalInput cActiveSlow;
    DigitalInput cActiveFast;
    DigitalInput cRotatingView;
    DigitalInput cZoomingView;
    DigitalInput cCancel;
    DigitalInput cConfigureTool;
    DigitalInput cNextTheme;
    DigitalInput cNextTool;
    DigitalInput cPreviousTheme;
    DigitalInput cPreviousTool;
    DigitalInput cToolMode;
    DigitalInput cUseTool;
    DigitalInput cExit;

    bool cSignalConsumed;
    bool cHasFocus;

    IAction* cExitAction;

    double cDistance;
    double cRotation;
    double cTilt;
    double cRoll;
    ScreenFloat cScreenYaw;
    ScreenFloat cScreenPitch;
    IStateNotifier<IFloat>* cScreenYawNotifier;
    IStateNotifier<IFloat>* cScreenPitchNotifier;

    HatHandler& cHatHandler;
    LiteralVertex cLocation;
    LiteralVertex cMomentum;
    IWorldEditorToolInstance* cSelectedTool;
    AnimatedFloat cPaletteSelectionX;
    TerrainBrush cTerrainBrush;

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
    bool isCursorLocked() const;
  };
}
