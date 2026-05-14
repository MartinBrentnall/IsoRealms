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

namespace IsoRealms::Equilibria {
  class Equilibria;

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
    ModelCycler(Equilibria& equilibria, IResourceData& data);
    ModelCycler(Equilibria& equilibria, IResourceData& data, JSONObject object);
    void registerAssets(ResourceAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    void getProperties(IPropertyMaker& owner, const Metadata& metadata);
    void removed();

    /*********************\
     * Module interfaces *
    \*********************/
    void updateEditing(unsigned int milliseconds);
    void reset();

    /***********************\
     * Scripting Interface *
    \***********************/
    void next();
    void previous();
    private:

    // JSON members.
    static const std::string JSON_MODEL;
    static const std::string JSON_MODELS;

    /**
     * An output "Offset" model is created for each input model configured
     * within this model cycler.
     */
    class Offset : public IModel {
      public:

      /**
       * Construct an offset model.
       *
       * @param parent Parent model cycler containing the actual models of the cycle.
       * @param offset Index value of this offset.
       */
      Offset(ModelCycler& parent, unsigned int offset);

      /**
       * Get the current cycle index of this offset, accounting for both the
       * current runtime cycle value and the index value of this offset.
       */
      unsigned int getCycleIndex() const;

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
      ModelCycler& cParent; /// Parent model cycler.

      /**
       * Each instance from this offset keeps an instance of each model
       * through which it may cycle.
       */
      class Instance : public IModelInstance {
        public:

        /**
         * Construct an instance from the parent offset.  The instances
         * through which this instance may cycle are created from the models
         * defined within the top-most model cycler.
         *
         * @param parent Parent model offset.
         */
        Instance(Offset& parent);

        /***********************\
        * Implements IModelInstance *
        \***********************/
        void update(unsigned int milliseconds) override;
        void render() const override;

        private:
        Offset& cParent;                                     /// Parent model offest.
        std::vector<std::unique_ptr<ModelInstance>> cModels; /// Model instances through which this instance may cycle.
      };

      // Definition data.
      unsigned int cDefOffset; /// Index value of this offset.

      // Runtime data.
      std::vector<std::unique_ptr<Instance>> cRuntimeInstances; /// Instances created from this offset.
    };

    // Definition data.
    std::vector<std::unique_ptr<Model>> cDefModels;
    std::vector<std::unique_ptr<Offset>> cOffsetModels;

    // Runtime data.
    unsigned int cRuntimeCycleIndex;

    // Scripting interface.
    LuaBinding<ModelCycler> cLuaBinding;

    // Editing data.
    unsigned int cEditingIconCycle;
  };
}

