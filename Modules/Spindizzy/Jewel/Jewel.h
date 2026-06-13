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

#include "IsoRealms.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
  * Model type of a Spindizzy jewel.  The model consists of a statically coloured
  * frame, with colour cycling panels.  The frame and the panel cycle colours are
  * user configurable.
  */
  class Jewel final : public IModel {
    public:
      
    /***********************\
     * Component interface *
    \***********************/
    Jewel(Spindizzy& spindizzy, IComponentData& data);
    void registerAssets(ComponentAssetRegistry& assets);
    bool renderIcon() const;
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);

    /*********************\
     * Module interfaces *
    \*********************/
    void updateEditing(unsigned int milliseconds);

    /*********************\
     * Implements IModel *
    \*********************/
    IModelInstance* createModel() override;
    bool renderPreview() const override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    class CycleColour {
      public:
      CycleColour(Jewel& parent, IComponentData& owner);
      CycleColour(Jewel& parent, IComponentData& owner, JSONObject object);

      void save(JSONObject object) const;
      const Colour* getColour() const;
      bool operator==(const CycleColour& cycleColour) const;
      void getProperties(IPropertyMaker& owner, const Metadata& metadata, std::function<void()> removeFunction);
      
      private:
      
      // External interfaces.
      Jewel& cParent;
      
      // Definition data.
      Colour cDefColour;
    };
    
    /**
     * Model instance of a Spindizzy jewel.  The model consists of a statically
     * coloured frame, with colour cycling panels.  The frame and the panel cycle
     * colours are user configurable.
     */
    class Instance : public IModelInstance {
      public:
      Instance(Jewel& parent, Project& engine);
      void randomize();
      ~Instance();
      
      /*****************************\
       * Implements IModelInstance *
      \*****************************/
      void update(unsigned int) override;
      void render() const override;
      
      private:
      
      // Global data.
      static unsigned int cReferenceCount;      /// Presence of display list depends on reference count.
      static GLuint cPanelDisplayList;          /// Display list for rendering the jewel panels.
      static GLuint cFrameDisplayList;          /// Display list for rendering the jewel frame.
      
      // External interfaces.
      Jewel& cDefParent;
      
      // Runtime data.
      float cProgress; /// Value from 0.0 to < cColoursCycle.size() to determine the actual current panel colour of this model instance.
    };

    void randomizeInstances();

    // JSON members.
    inline static const std::string JSON_COLOUR        = "colour";

    // Default values.
    inline static const float DEFAULT_CYCLE_SPEED = 0.001f;

    // External interfaces.
    Project& cProject; // Required for pre-rendering.

    // Definition data.
    Colour cDefColourFrame;                                     /// Colour of the frame.
    std::vector<std::unique_ptr<CycleColour>> cDefColoursCycle; /// Ordered cycle colours of panel.
    float cDefCycleSpeed;                                       /// Cycle speed for colours.
    
    // Runtime data.
    std::vector<std::unique_ptr<Instance>> cInstances;       /// Model instances.  Each instance has a different position.

    // Editing data.
    std::unique_ptr<Instance> cSampleModel;
  };
}
