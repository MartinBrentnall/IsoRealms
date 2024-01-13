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

#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/IZoneObjectTrait.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/IProcessor.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/IPhysics.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectTypeTraitChaser;
  class ZoneObject;

  /**
   * Trait that causes a physical object to chase a target.
   */
  class Chaser final : public IZoneObjectTrait,
                       public IProcessor {
    public:
    Chaser(ZoneObject& object, ZoneObjectTypeTraitChaser& type);

    /*******************************\
     * Implements IZoneObjectTrait *
    \*******************************/
    void registerAssets(ITraitRegistry& registry) override;
    void unregisterAssets(ZoneObject& object) override;
    void load(DOMNode& node) override;
    void save(DOMNodeWriter* node) const override;
    bool hasConfiguration() const override;
    void reset() override;
    IBinding* getTraitBinding(const std::string& id) override;

    /*************************\
     * Implements IProcessor *
    \*************************/
    void update(unsigned int milliseconds) override;

    private:

    // Definition data.
    ZoneObjectTypeTraitChaser& cDefType;         /// Type of this chaser.
    IPhysics* cDefPhysicalObject; /// The object driven by this chaser.
  };
}
