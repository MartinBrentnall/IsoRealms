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

#include "IsoRealms/Assets/Type/IDigitalInput.h"
#include "IsoRealms/Editing/Property/IOptionalObject.h"

namespace IsoRealms {
  class Metadata;
  class IResourceData;

  /**
   * A digital input mapping to a button on a controller.
   */
  class GameControllerButton : public IDigitalInput {
    public:
#if _WIN32
    inline static const unsigned int CROSS    = 1;
    inline static const unsigned int CIRCLE   = 2;
    inline static const unsigned int SQUARE   = 0;
    inline static const unsigned int TRIANGLE = 3;
    inline static const unsigned int L2       = 6;
#else
    inline static const unsigned int CROSS    = 0;
    inline static const unsigned int CIRCLE   = 1;
    inline static const unsigned int SQUARE   = 3;
    inline static const unsigned int TRIANGLE = 2;
    inline static const unsigned int L2       = 6;
#endif

    GameControllerButton(const Metadata& metadata, IResourceData& owner);
    GameControllerButton(const Metadata& metadata, IResourceData& owner, JSONObject object);

    /****************************\
     * Implements IDigitalInput *
    \****************************/
    bool getState(const sf::Event& event) const override;
    bool matches(const sf::Event& event) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;

    /***************************************\
     * Implements IAsset via IDigitalInput *
    \***************************************/
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    void getAssetProperties(IPropertyMaker& owner) override;
    bool isDefaultConfiguration() const override;

    private:
    class ButtonChooser : public IOptionalObject {
      public:
      void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;
    };

    inline static const std::string JSON_BUTTON = "button";

    // External interfaces.
    const Metadata& cMetadata;
    ButtonChooser cButtonChooser;

    unsigned int cButton; /// The button associated with this mapping.
    // TODO: Support multiple controllers.
  };
}
