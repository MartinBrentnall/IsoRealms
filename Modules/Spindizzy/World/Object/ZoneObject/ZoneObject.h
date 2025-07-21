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
#include <memory>
#include <string>

#include "ILocation.h"
#include "IMovable.h"
#include "IPhysics.h"
#include "IProcessor.h"
#include "IRenderer.h"
#include "ITraitRegistry.h"
#include "Traits/IZoneObjectTrait.h"

#include "Modules/Spindizzy/IWorldObject.h"

namespace IsoRealms::Spindizzy {
  class ZoneObjectType;
  
  class ZoneObject : public IWorldObject {
    public:
    
    // Constructors.
    ZoneObject(Zone& zone, ZoneObjectType* type);
    ZoneObject(Zone& zone, JSONObject object);

    // Interface for use by parent zone.
    void registerAssets();
    void reset();
    void save(JSONObject object) const;
    bool isType(const ZoneObjectType* const type) const;
    void updateRuntime(unsigned int milliseconds);
    void updateVanish();
    void updateAppear();
    void updateEditing(unsigned int milliseconds);
    void renderRuntime() const;
    void renderEditing() const;

    // Interface to be used by traits.
    void registerLocation(const std::string& id, ILocation* location);
    void registerMovable(const std::string& id, IMovable* movable);
    void registerPhysics(const std::string& id, IPhysics* physics);
    void registerRenderer(const std::string& id, IRenderer* renderer);
    void registerProcessor(const std::string& id, IProcessor* processor);
    void unregisterLocation(ILocation* location);
    void unregisterMovable(IMovable* movable);
    void unregisterPhysics(IPhysics* physics);
    void unregisterRenderer(IRenderer* renderer);
    void unregisterProcessor(IProcessor* processor);
    ILocation* getLocation(const std::string& id) const;
    IMovable* getMovable(const std::string& id) const;
    IPhysics* getPhysics(const std::string& id) const;
    IRenderer* getRenderer(const std::string& id) const;
    IProcessor* getProcessor(const std::string& id, bool required = true) const;
    
    Zone& getZone();

    /***************************\
     * Implements IWorldObject *
    \***************************/
    bool contains(const LiteralVertex& location) const override;
    void renderSelectionHighlight() const override;
    void remove() override;
    std::vector<std::unique_ptr<IProperty>> getProperties(IPropertyOwner& owner) override;
    std::string getTypeName() const override;
    Zone& getObjectZone() override;

    private:
    
    // JSON members.
    static const std::string JSON_ID;
    static const std::string JSON_TRAITS;
    static const std::string JSON_TYPE;

    // Internal classes.
    class TraitRegistry : public ITraitRegistry {
      public:
      TraitRegistry(ZoneObject& parent, const std::string& id);
      
      /*****************************\
       * Implements ITraitRegistry *
      \*****************************/
      void registerLocation(ILocation* location) override;
      void registerMovable(IMovable* movable) override;
      void registerProcessor(IProcessor* processor) override;
      void registerPhysics(IPhysics* physics) override;
      void registerRenderer(IRenderer* renderer) override;
      
      private:
      ZoneObject& cParent;
      std::string cID;
    };

    // External interfaces.
    Zone& cZone; /// Zone to which this object belongs.
    
    // Definition data
    ZoneObjectType* cDefType;                                            /// Type of this object.
    std::map<std::string, std::unique_ptr<IZoneObjectTrait>> cDefTraits; /// Traits of this object.
    IRenderer* cDefRuntimeRenderer;                                      /// Visual representation of this object while playing.
    IRenderer* cDefEditingRenderer;                                      /// Visual representation of this object while editing.
    IProcessor* cDefRuntimeProcessor;                                    /// Processing of this object while playing.
    IProcessor* cDefEditingProcessor;                                    /// Processing of this object while editing.
    
    // Runtime data.
    std::map<std::string, ILocation*> cDefLocations;                     /// Registered locations of this object.
    std::map<std::string, IMovable*> cDefMovables;                       /// Registered movable elements of this object.
    std::map<std::string, IPhysics*> cDefPhysics;                        /// Registered physical elements of this object.
    std::map<std::string, IRenderer*> cDefRenderers;                     /// Registered renderers of this object.
    std::map<std::string, IProcessor*> cDefProcessors;                   /// Registered processors of this object.
  };
}
