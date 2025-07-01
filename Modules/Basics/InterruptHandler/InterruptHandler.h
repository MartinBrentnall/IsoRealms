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

#include <vector>

#include "IsoRealms/ResourceDefinition.h"
#include "IsoRealms/Types.h"

namespace IsoRealms::Basics {
  class Basics;

  /**
   * Resource definition for an input handler that performs an action upon any
   * active input, such as a mouse button, key press, or game controller
   * button.  The action is not performed on inputs that are considered
   * passive such as mouse movements and analogue stick movements.
   *
   * This resource is useful for implementing the ability to interrupt non-
   * interactive things such as an attract mode or recorded gameplay.
   */
  class InterruptHandler final : public IInputHandler {
    public:
    InterruptHandler(IProject& project, Basics& basics, IResourceData& data);
    InterruptHandler(IProject& project, Basics& basics, IResourceData& data, JSONObject object, IOptions& options);
    void registerAssets(IAssetRegistry& assets);
    void save(JSONObject object) const;
    void hintInUse(bool inUse);
    bool renderIcon() const;
    std::vector<std::unique_ptr<IProperty>> getProperties(IResourceData& owner);

    /****************************\
     * Implements IInputHandler *
    \****************************/
    bool input(sf::Event& event) override;
    void resetInput() override;
    bool renderAssetIcon() const override;
    void saveAsset(JSONObject object) const override;
    std::vector<std::unique_ptr<IProperty>> getAssetProperties() override;
    bool isDefaultConfiguration() const override;

    private:

    // JSON members.
    static const std::string JSON_CONSUME;
    static const std::string JSON_ON_INPUT;

    // Definition data.
    Action cDefAction;
    bool cDefConsume;
  };
}
