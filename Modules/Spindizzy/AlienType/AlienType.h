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

#include <cmath>
#include <GL/glew.h>

#include "IsoRealms/Common/IVisualElement.h"
#include "IsoRealms/Editing.h"
#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/Assets/Type/IPhysicalObjectType.h"
#include "Modules/Spindizzy/Assets/Type/IWorldEditorTool.h"
#include "Modules/Spindizzy/ISpindizzyRegistry.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
   * Resource definition for a type of alien.  The appearance, target, and
   * physical attributes of alien instances of this type are configurable.
   */
  class AlienType : public IPhysicalObjectType,
                    public IWorldEditorTool {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    AlienType(IProject& project, Spindizzy& spindizzy, IResourceData& data);
    AlienType(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner);

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    virtual ~AlienType();

    // Interface to be used by module.
    void registerAssets(ISpindizzyRegistry* registry);
    
    // Interface to be used by alien instances.
    std::unique_ptr<ModelInstance> createModel();
    IVertex* getTarget();
    float getAcceleration();
    float getFriction();
    float getSpinSpeed();
    float getStepReach() const;
    float getHeight() const;
    float getRadius() const;
    float getHugMomentum() const;

    /*********************\
     * Scripting Support *
    \*********************/
    void setSpinSpeed(float spinSpeed);

    /**********************************\
     * Implements IPhysicalObjectType *
    \**********************************/
    std::string getPhysicalObjectTypeID() const override;
    IBinding* getBinding(const std::string& id) const override;
    std::string getBindingID(const IBinding* binding) const override;

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor& editor) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_ACCELERATION;
    static const std::string JSON_APPEARANCE;
    static const std::string JSON_FRICTION;
    static const std::string JSON_HEIGHT;
    static const std::string JSON_HUG_MOMENTUM;
    static const std::string JSON_RADIUS;
    static const std::string JSON_SPIN_SPEED;
    static const std::string JSON_TARGET;

    static const float DEFAULT_ACCELERATION;
    static const float DEFAULT_HEIGHT;
    static const float DEFAULT_HUG_MOMENTUM;
    static const float DEFAULT_FRICTION;
    static const float DEFAULT_RADIUS;
    static const float DEFAULT_SPIN_SPEED;

    // Internal classes.
    class Pen : public IWorldEditorToolInstance {
      public:
      Pen(AlienType& parent, WorldEditor& editor);
      
      /***************************************\
       * Implements IWorldEditorToolInstance *
      \***************************************/
      bool isTool(IWorldEditorTool* tool) const override;
      bool renderIcon(float yaw) const override;
      void renderEditingPreview() const override;
      void renderUI(float aspectRatio) const override;
      void updateUI(unsigned int milliseconds) override;
      bool inputTool(SignalInputID id, double yaw) override;
      bool isCursorLocked() const override;
      void processCursorMovement(LiteralVertex* start, LiteralVertex* end) override;
      double getSnapInterval() const override;
      
      private:
      AlienType& cParent;
      WorldEditor& cEditor;
    };

    // External interfaces.
    Spindizzy& cSpindizzy; /// Spindizzy module reference.

    // Definition data
    Model cDefModel;          /// Visual representation of this alien type.
    Vertex cDefTarget;        /// Target that aliens of this type move towards.
    float cDefAcceleration;   /// Acceleration factor of aliens of this type.
    float cDefFriction;       /// Friction factor of aliens of this type.
    float cDefSpinSpeed;      /// Initial spin speed of aliens of this type.
    float cDefHeight;         /// Height of aliens of this type.
    float cDefRadius;         /// Radius of aliens of this type.
    float cDefHugMomentum;    /// Hug momentum of aliens of this type.

    // Runtime data
    float cRuntimeSpinSpeed; /// Current spin speed of aliens of this type.

    // Editing data.
    std::vector<std::unique_ptr<Pen>> cEditingPens; /// Pens for drawing aliens of this type.

    LuaBinding<AlienType> cLuaBinding;
  };
}
