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
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>

#include "IsoRealms/Editing.h"
#include "IsoRealms/Types.h"
#include "IsoRealms/ResourceDefinition.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for creating colour outputs that fade between two or
   * more colour inputs.
   */
  class ColourCycler final {
    public:

    /**********************\
     * Resource Interface *
    \**********************/
    ColourCycler(IProject& project, Basics& basics, IResourceData& data);
    ColourCycler(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void unregisterAssets(IAssetRemover& assets, IAssets& releaser, bool relinquish);
    void save(JSONObject object, IAssetIdentifier& identifier) const;
    void hintInUse(bool inUse);
    bool renderIcon();
    std::vector<std::unique_ptr<IProperty>> getProperties(IAssetBrowser& browser, IAssetRegistry& assets);

    private:

    // JSON members.
    static const std::string JSON_COLOUR;
    static const std::string JSON_INPUTS;
    static const std::string JSON_OFFSET;
    static const std::string JSON_OUTPUTS;
    static const std::string JSON_SPEED;
    static const std::string JSON_SPEED_MULTIPLIER;

    // Definition data.
    class ColourCycle : public IColour {
      public:
      ColourCycle(ColourCycler& parent, float startPosition, float speedMultiplier);
      void registerAssets(IAssetRegistry& assets, const std::string& name);
      void unregisterAssets(IAssetRemover& assets, bool relinquish);
      void save(JSONObject object) const;
      void update(unsigned int milliseconds);
      void reset();
      std::vector<std::unique_ptr<IProperty>> getProperties();

      /**********************\
       * Implements IColour *
      \**********************/
      void set() const override;
      float getRed() const override;
      float getGreen() const override;
      float getBlue() const override;
      float getAlpha() const override;
      void saveAsset(JSONObject object) const override;
      std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
      bool isDefaultConfiguration() const override;

      private:
      ColourCycler& cParent;     /// Parent.

      // Definition data.
      float cDefStartPosition;   /// Offset of the parent set cycle position.
      float cDefSpeedMultiplier; /// Speed to multiply by.

      // Runtime data.
      double cRuntimePosition;   /// Current position in the cycle.
      float cRuntimeRed;         /// Cached calculated red value.
      float cRuntimeGreen;       /// Cached calculated green value.
      float cRuntimeBlue;        /// Cached calculated blue value.
      float cRuntimeAlpha;       /// Cached calculated alpha value.

      /// Misc.
      IStateNotifier<IColour>* cStateNotifier;

      /// Internal Functions.
      void calculateColour();
    };

    IProject& cProject;
    std::vector<std::unique_ptr<Colour>> cDefInputColours;       /// List of input colours to cycle through.
    std::vector<std::unique_ptr<ColourCycle>> cDefOutputColours; /// List of output colours with different cycle offsets.
    Float cDefCycleSpeed;                                        /// Cycle speed.

    /**********************\
     * Internal Functions *
    \**********************/
    void update(unsigned int milliseconds);
  };
}
