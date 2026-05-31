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

#include "IsoRealms/Assets/Type/IDigitalInput.h"

namespace IsoRealms {
  class Metadata;
  class IResourceData;

  /**
   * A digital input mapping to a button on a controller.
   */
  class GameControllerButton : public IDigitalInput {
    private:
    inline static const std::string JSON_BUTTON = "button";

    unsigned int cButton; /// The button associated with this mapping.
    // TODO: Support inversion.
    // TODO: Support multiple controllers.

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

    /**
     * Construct a digital input mapping associated with the specified button.
     * 
     * @param key The button to associate this digital input mapping with.
     */
    GameControllerButton(const unsigned int button);
    
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
  };
}
