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

#include <SFML/Window/Joystick.hpp>

#include "IsoRealms/Utils.h"

#include "IsoRealms/Assets/Type/IAnalogueInput.h"
#include "IsoRealms/Editing/Property/IOptionalObject.h"

namespace IsoRealms {
  class Metadata;
  class IComponentData;

  /**
   * A analogue input mapping to an analogue stick or similar.
   */
  class GameControllerAxis : public IAnalogueInput {
    public:
    GameControllerAxis(const Metadata& metadata, IComponentData& owner);
    GameControllerAxis(const Metadata& metadata, IComponentData& owner, JSONObject object);

    /*****************************\
     * Implements IAnalogueInput *
    \*****************************/
    std::string getName() const override;
    float getState(const sf::Event& event) const override;
    bool matches(const sf::Event& event) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;
    std::string getLocalizedName() const override;
    void loadCustomMapping(JSONObject object) override;
    void registerAssets(ComponentAssetRegistry& assets) override;

    /****************************************\
     * Implements IAsset via IAnalogueInput *
    \****************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    class AxisChooser : public IOptionalObject {
      public:
      AxisChooser(const Metadata& metadata);
      void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;

      private:
      const Metadata& cMetadata;
    };

    // External interfaces.
    const Metadata& cMetadata;
    AxisChooser cAxisChooser;

    // JSON members.
    inline static const std::string JSON_AXIS      = "axis";
    inline static const std::string JSON_DEAD_ZONE = "deadZone";

    // Definition data.
    unsigned int cDefAxis = 0;     /// Axis of this mapping.
    float cDefDeadZone    = 0.16f; /// Dead zone of this mapping

    static std::string getChoiceLabel(const Metadata& metadata, unsigned int axis);
  };
}
