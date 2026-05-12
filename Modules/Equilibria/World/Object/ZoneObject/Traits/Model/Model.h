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

#include "Modules/Equilibria/World/Object/ZoneObject/Traits/IZoneObjectTrait.h"
#include "Modules/Equilibria/World/Object/ZoneObject/ILocation.h"
#include "Modules/Equilibria/World/Object/ZoneObject/IProcessor.h"
#include "Modules/Equilibria/World/Object/ZoneObject/IRenderer.h"

namespace IsoRealms::Equilibria {
  class ZoneObjectTypeTraitModel;
  class ZoneObject;

  /**
   * Trait that renders a model.
   */
  class Model final : public IZoneObjectTrait,
                      public IRenderer,
                      public IProcessor {
    public:
    Model(ZoneObject& object, ZoneObjectTypeTraitModel& type);

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
    ZoneObjectTypeTraitModel& cDefType;       /// Type of model.
    ILocation* cDefLocation;                  /// Model location.
    std::unique_ptr<ModelInstance> cDefModel; /// Model to render at location.
    
    // Runtime data.
    bool cRuntimeEnabled; /// Determines whether this model is rendered.
    
    // Scripting support.
    LuaBinding<Model> cLuaBinding; /// Allows models to be bound to lua variables.
  };
}
