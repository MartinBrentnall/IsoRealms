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

#include "IsoRealms/IProject.h"
#include "IsoRealms/Literals.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Spindizzy {
  class Spindizzy;

  /**
  * Model type of a Spindizzy jewel.  The model consists of a statically coloured
  * frame, with colour cycling panels.  The frame and the panel cycle colours are
  * user configurable.
  */
  class Jewel final : public I3DModelType {
    public:
      
    /**********************\
     * Resource interface *
    \**********************/
    Jewel(IProject* project, Spindizzy* spindizzy);
    Jewel(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);  
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /***************************\
     * Implements I3DModelType *
    \***************************/
    I3DModel* createModel() override;
    bool renderPreview() const override;
    bool renderAssetIcon() const override;
    
    private:
    class CycleColour {
      public:
      CycleColour(Jewel* parent, IProject* project, DOMNode& node);
      
      void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
      const IColour* getColour() const;
      bool operator==(const CycleColour& cycleColour) const;
      
      private:
      
      // External interfaces.
      Jewel* cParent;
      
      // Definition data.
      Colour cDefColour;
    };
    
    /**
     * Model instance of a Spindizzy jewel.  The model consists of a statically
     * coloured frame, with colour cycling panels.  The frame and the panel cycle
     * colours are user configurable.
     */
    class Instance : public I3DModel {
      public:
      Instance(Jewel* parent, IProject* engine);
      void randomize();
      ~Instance();
      
      /***********************\
       * Implements I3DModel *
      \***********************/
      void update(unsigned int) override;
      void render() const override;
      
      private:
      
      // Global data.
      static unsigned int cReferenceCount;      /// Presence of display list depends on reference count.
      static GLuint cPanelDisplayList;          /// Display list for rendering the jewel panels.
      static GLuint cFrameDisplayList;          /// Display list for rendering the jewel frame.
      
      // External interfaces.
      Jewel* cDefParent;
      
      // Runtime data.
      float cProgress; /// Value from 0.0 to < cColoursCycle.size() to determine the actual current panel colour of this model instance.
    };

    // DOM strings.
    static const std::string TAG_COLOUR;
    static const std::string TAG_COLOUR_CYCLE;
    static const std::string TAG_FRAME_COLOUR;

    static const std::string ATTRIBUTE_CYCLE_SPEED;
    
    // External interfaces.
    IProject* cEngine; // Required for pre-rendering.
      
    // Definition data.
    std::vector<std::unique_ptr<Instance>> cInstances;       /// Model instances.  Each instance has a different position.
    Colour cColourFrame;                                     /// Colour of the frame.
    std::vector<std::unique_ptr<CycleColour>> cColoursCycle; /// Ordered cycle colours of panel.
    float cCycleSpeed;                                       /// Cycle speed for colours.
    
    // Editing data.
    std::unique_ptr<Instance> cSampleModel;
  };
}
