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
#include "IsoRealms/Types.h"

#include "Modules/Spindizzy/World/Object/ZoneObject/Traits/IZoneObjectTrait.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/ILocation.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/IProcessor.h"
#include "Modules/Spindizzy/World/Object/ZoneObject/IRenderer.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectTypeTraitSpinner;
  class ZoneObject;

  /**
   * Trait that renders a model that spins as it moves.
   */
  class Spinner final : public IZoneObjectTrait,
                        public IRenderer,
                        public IProcessor {
    public:
    Spinner(ZoneObject& object, ZoneObjectTypeTraitSpinner& type);

    // Scripting support.
    void setEnabled(bool enabled);
    bool isEnabled() const;

    /*******************************\
     * Implements IZoneObjectTrait * 
    \*******************************/
    void registerAssets(ITraitRegistry& registry) override;
    void save(JSONObject object) const override;
    bool hasConfiguration() const override;
    void reset() override;
    IBinding* getTraitBinding(const std::string& id) override;
    
    /************************\
     * Implements IRenderer *
    \************************/
    void render() const override;
    
    /*************************\
     * Implements IProcessor *
    \*************************/
    void update(unsigned int milliseconds) override;
    
    private:
    
    // Definition data.
    ZoneObjectTypeTraitSpinner& cDefType;     /// Type of spinner.
    ILocation* cDefLocation;                  /// Object location.
    std::unique_ptr<ModelInstance> cDefModel; /// Model to render at object location.
    
    // Runtime data.
    bool cRuntimeEnabled; /// Determines whether this spinner is rendered.
    
    // Scripting support.
    LuaBinding<Spinner> cLuaBinding; /// Allows spinners to be bound to lua variables.
  };
}
