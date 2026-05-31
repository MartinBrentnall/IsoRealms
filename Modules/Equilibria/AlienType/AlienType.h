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
#pragma once

#include <cmath>
#include <GL/glew.h>

#include "IsoRealms.h"

#include "Modules/Equilibria/Assets/Type/IPhysicalObjectType.h"
#include "Modules/Equilibria/Assets/Type/IWorldEditorTool.h"
#include "Modules/Equilibria/EquilibriaAssetRegistry.h"

namespace IsoRealms::Equilibria {
  class Equilibria;

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
    AlienType(Equilibria& equilibria, IResourceData& data);
    AlienType(Equilibria& equilibria, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    bool hasReadOnlyReferences() const;
    void overrideReadOnlyReferences();

    /*********************\
     * Module interfaces *
    \*********************/
    void reset();

    // Interface to be used by module.
    void registerAssets(const std::string& parentID);
    
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
    IBinding* getPhysicalObjectTypeBinding(const std::string& id) const override;
    std::string getPhysicalObjectTypeBindingID(const IBinding* binding) const override;
    void forEachAvailablePhysicalObjectTypeTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;

    /*******************************\
     * Implements IWorldEditorTool *
    \*******************************/
    IWorldEditorToolInstance* createToolInstance(WorldEditor& editor, IResourceData& owner) override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    inline static const std::string JSON_ACCELERATION = "acceleration";
    inline static const std::string JSON_APPEARANCE   = "appearance";
    inline static const std::string JSON_FRICTION     = "friction";
    inline static const std::string JSON_HEIGHT       = "height";
    inline static const std::string JSON_HUG_MOMENTUM = "hugMomentum";
    inline static const std::string JSON_RADIUS       = "radius";
    inline static const std::string JSON_SPIN_SPEED   = "spinSpeed";
    inline static const std::string JSON_TARGET       = "target";

    inline static const float DEFAULT_ACCELERATION = 0.0000215f;
    inline static const float DEFAULT_HEIGHT       = 1.7f;
    inline static const float DEFAULT_HUG_MOMENTUM = 0.0001f;
    inline static const float DEFAULT_FRICTION     = 0.001f;
    inline static const float DEFAULT_RADIUS       = 0.4f;
    inline static const float DEFAULT_SPIN_SPEED   = 0.0f;

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
      bool inputTool(sf::Event& event) override;
      bool inputTool(SignalInputID id, double yaw) override;
      bool isCursorLocked() const override;
      void processCursorMovement(LiteralVertex* start, LiteralVertex* end) override;
      double getSnapInterval() const override;
      
      private:
      AlienType& cParent;
      WorldEditor& cEditor;
    };

    // External interfaces.
    Equilibria& cEquilibria; /// Equilibria module reference.

    // Asset registry.
    EquilibriaAssetRegistry cAssets; /// Equilibria asset registry.

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
