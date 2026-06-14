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

#include <map>

#include "IsoRealms/Resources/Type/IDigitalInput.h"
#include "IsoRealms/Editing/Property/IOptionalObject.h"
#include "IsoRealms/Input/HatHandler.h"

namespace IsoRealms {
  class Metadata;
  class IComponentData;

  /**
   * A digital input mapping to an analogue stick or similar.
   */
  class GameControllerHat : public IDigitalInput {
    public:
    GameControllerHat(const Metadata& metadata, IComponentData& owner);
    GameControllerHat(const Metadata& metadata, IComponentData& owner, JSONObject object);

    /**
     * Retrieve the direction of the specified name.  The key is always such that it
     * may be passed into to the getName() function in order to retrieve the
     * same name as was passed into this function.
     *
     * @param name Name of the direction to retrieve.
     * @return The direction of the specified name.
     * @throws ArgumentException If the specified name is not known.
     */
    static HatHandler::Direction getDirection(const std::string& name);

    /**
     * Retrieve the name of the specified direction.  The name is always such that it
     * may be passed into to the getKey() function in order to retrieve the
     * same direction as was passed into this function.
     *
     * @param key The direction to retrieve the name of.
     * @return Name of the specified direction.
     * @throws ArgumentException If the specified value is not known.
     */
    static std::string getName(const HatHandler::Direction value);

    /****************************\
     * Implements IDigitalInput *
    \****************************/
    void saveResource(JSONObject object) const override;
    void getResourceProperties(IComponentDefiner& definer) override;
    bool getState(const sf::Event& event) const override;
    bool matches(const sf::Event& event) const override;
    std::string getShortName() const override;
    std::string getLongName() const override;
    std::string getLocalizedName() const override;

    /******************************************\
     * Implements IResource via IDigitalInput *
    \******************************************/
    private:
    class DirectionChooser : public IOptionalObject {
      public:
      DirectionChooser(const Metadata& metadata);
      void forEachAvailableTreeItem(std::function<void(const TreeItemInfo&)> getTreeItemInfoFunction) const override;

      private:
      const Metadata& cMetadata;
    };

    inline static const std::string JSON_DIRECTION = "direction";

    inline static const std::map<std::string, HatHandler::Direction> cDirectionsByName = {
      {"LeftUp",    HatHandler::Direction::HAT_LEFTUP},
      {"Up",        HatHandler::Direction::HAT_UP},
      {"RightUp",   HatHandler::Direction::HAT_RIGHTUP},
      {"Left",      HatHandler::Direction::HAT_LEFT},
      {"Centered",  HatHandler::Direction::HAT_CENTERED},
      {"Right",     HatHandler::Direction::HAT_RIGHT},
      {"LeftDown",  HatHandler::Direction::HAT_LEFTDOWN},
      {"Down",      HatHandler::Direction::HAT_DOWN},
      {"RightDown", HatHandler::Direction::HAT_RIGHTDOWN}
    };

    // External interfaces.
    const Metadata& cMetadata;
    HatHandler& cHatHandler;
    DirectionChooser cDirectionChooser;

    HatHandler::Direction cDirection; /// Determines the direction of true and false state.
  };
}
