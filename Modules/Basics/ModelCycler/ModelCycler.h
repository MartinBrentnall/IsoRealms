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

#include "IsoRealms/Lua.h"
#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for a model that can be dynamically cycled through
   * multiple selected models.  The cycle function is achieved through the
   * scripting interface.
   */
  class ModelCycler final {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    ModelCycler(IProject* project, Basics* basics);
    ModelCycler(IProject* project, Basics* basics, DOMNode& node, IOptions* options, IResourceData* data);
    void registerAssets(IAssetRegistry* assets);
    void unregisterAssets(IAssetRemover* assets, IAssets* releaser);
    void save(DOMNodeWriter* node, IAssetIdentifier* identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    std::vector<IProperty*> getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener);

    /***********************\
     * Scripting Interface *
    \***********************/
    void next();
    void previous();
    private:

    // DOM strings.
    static const std::string TAG_MODEL;
    static const std::string TAG_NAME;

    /**
     * An output "Offset" model is created for each input model configured
     * within this model cycler.
     */
    class Offset : public I3DModelType {
      public:

      /**
       * Construct an offset model.
       *
       * @param parent Parent model cycler containing the actual models of the cycle.
       * @param offset Index value of this offset.
       */
      Offset(ModelCycler* parent, unsigned int offset);

      /**
       * Get the current cycle index of this offset, accounting for both the
       * current runtime cycle value and the index value of this offset.
       */
      unsigned int getCycleIndex() const;

      /***************************\
       * Implements I3DModelType *
      \***************************/
      I3DModel* createModel() override;
      bool renderAssetIcon() const override;
    
      private:
      ModelCycler* cParent; /// Parent model cycler.

      /**
       * Each instance from this offset keeps an instance of each model
       * through which it may cycle.
       */
      class Instance : public I3DModel {
        public:

        /**
         * Construct an instance from the parent offset.  The instances
         * through which this instance may cycle are created from the models
         * defined within the top-most model cycler.
         *
         * @param parent Parent model offset.
         */
        Instance(Offset* parent);

        /***********************\
        * Implements I3DModel *
        \***********************/
        void update(unsigned int milliseconds) override;
        void render() const override;

        private:
        Offset* cParent;                                     /// Parent model offest.
        std::vector<std::unique_ptr<ModelInstance>> cModels; /// Model instances through which this instance may cycle.
      };

      // Definition data.
      unsigned int cDefOffset; /// Index value of this offset.

      // Runtime data.
      std::vector<std::unique_ptr<Instance>> cRuntimeInstances; /// Instances created from this offset.
    };

    // Definition data.
    std::vector<std::unique_ptr<Model>> cDefModelTypes;
    std::vector<std::unique_ptr<Offset>> cOffsetModels;

    // Runtime data.
    unsigned int cRuntimeCycleIndex;

    // Scripting interface.
    LuaBinding<ModelCycler> cLuaBinding;

    // Editing data.
    unsigned int cEditingIconCycle;
  };
}

