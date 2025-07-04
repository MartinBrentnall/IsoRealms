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
#include "ZoneObject.h"

#include "IsoRealms/Project.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/World/Object/Zone/Zone.h"
#include "Modules/Spindizzy/World/World.h"
#include "Modules/Spindizzy/ZoneObjectType/ZoneObjectType.h"

namespace IsoRealms::Spindizzy {
  const std::string ZoneObject::JSON_ID     = "id";
  const std::string ZoneObject::JSON_TRAITS = "traits";
  const std::string ZoneObject::JSON_TYPE   = "type";

  ZoneObject::ZoneObject(Zone& zone, ZoneObjectType* type) :
            cZone(zone),
            cDefType(type),
            cDefTraits(cDefType->createTraits(*this)) {
    reset();
  }

  ZoneObject::ZoneObject(Zone& zone, JSONObject object) :
            cZone(zone),
            cDefType(nullptr) {
    std::cout << "TODO: ZoneObject::ZoneObject" << std::endl;
//     cZone.getWorld().getSpindizzy()->getProject()->init([this, &node](IAssets& assets) {
//       cDefType = cZone.getWorld().getSpindizzy()->getZoneObjectType(node.getAttribute(JSON_TYPE));
//       cDefTraits = cDefType->createTraits(*this);
//       registerAssets();
//
//       for (OMNode& mNode : node) {
//         std::string mTag = mNode.getName();
//         if (mTag == AG_PROPERTY) {
//           std::string mTraitID = mNode.getAttribute(JSON_ID);
//           std::map<std::string, std::unique_ptr<IZoneObjectTrait>>::iterator mIterator = cDefTraits.find(mTraitID);
//           if (mIterator == cDefTraits.end()) {
//             throw ResourceInitException("ERROR: ZoneObject::ZoneObject: No trait found for trait ID \"" + mTraitID + "\"");
//           }
//           mIterator->second->load(mNode);
//         } else {
//           throw ResourceInitException("ERROR: ZoneObject::ZoneObject: Unknown tag \"" + mTag + "\"");
//         }
//       }
//       cDefRuntimeRenderer = getRenderer(cDefType->getRuntimeRendererID());
//       cDefEditingRenderer = getRenderer(cDefType->getEditingRendererID());
//       cDefRuntimeProcessor = getProcessor(cDefType->getRuntimeProcessorID());
//       cDefEditingProcessor = getProcessor(cDefType->getEditingProcessorID());
//       reset();
//     });
  }

  void ZoneObject::registerAssets() {
    for (const std::pair<const std::string, std::unique_ptr<IZoneObjectTrait>>& mPair : cDefTraits) {
      TraitRegistry mLocalRegistry(*this, mPair.first);
      mPair.second->registerAssets(mLocalRegistry);
    }
  }
  
  void ZoneObject::reset() {
    for (const std::pair<const std::string, std::unique_ptr<IZoneObjectTrait>>& mPair : cDefTraits) {
      mPair.second->reset();
    }
  }

  void ZoneObject::save(JSONObject object) const {
    object.addString(JSON_TYPE, cZone.getWorld().getSpindizzy().getID(cDefType));
    JSONArray mTraitsArray = object.addArray(JSON_TRAITS);
    for (const std::pair<const std::string, std::unique_ptr<IZoneObjectTrait>>& mPair : cDefTraits) {
      if (mPair.second->hasConfiguration()) {
        JSONObject mTraitObject = mTraitsArray.addObject();
        mTraitObject.addString(JSON_ID, mPair.first);
        mPair.second->save(mTraitObject);
      }
    }
  }

  bool ZoneObject::isType(const ZoneObjectType* const type) const {
    return cDefType == type;
  }

//   void ZoneObject::setActive() {
//     // TODO: Implement this.
//     reset();
//   }
//
//   void ZoneObject::setInactive() {
//     // TODO: Implement this.
//   }

  void ZoneObject::updateRuntime(unsigned int milliseconds) {
    if (cDefRuntimeProcessor != nullptr) {
      cDefRuntimeProcessor->update(milliseconds); 
    }
  }

  void ZoneObject::updateEditing(unsigned int milliseconds) {
    if (cDefEditingProcessor != nullptr) {
      cDefEditingProcessor->update(milliseconds);
    }
  }

  void ZoneObject::renderRuntime() const {
    cDefRuntimeRenderer->render();
  }
  
  void ZoneObject::renderEditing() const {
    cDefEditingRenderer->render();
  }

  void ZoneObject::registerLocation(const std::string& id, ILocation* location) {
    cDefLocations.emplace(id, location);
  }

  void ZoneObject::registerMovable(const std::string& id, IMovable* movable) {
    cDefMovables.emplace(id, movable);
  }
  
  void ZoneObject::registerPhysics(const std::string& id, IPhysics* physics) {
    cDefPhysics.emplace(id, physics);
  }

  void ZoneObject::registerRenderer(const std::string& id, IRenderer* renderer) {
    cDefRenderers.emplace(id, renderer);
  }
  
  void ZoneObject::registerProcessor(const std::string& id, IProcessor* processor) {
    cDefProcessors.emplace(id, processor);
  }
  
  void ZoneObject::unregisterLocation(ILocation* location) {
    if (Utils::removeByValue<std::string, ILocation*>(cDefLocations, location) == 0) {
      std::cout << "WARNING: ZoneObject::unregisterLocation: Location to unregister was not found in this object" << std::endl;
    }
  }
  
  void ZoneObject::unregisterMovable(IMovable* movable) {
    if (Utils::removeByValue(cDefMovables, movable) == 0) {
      std::cout << "WARNING: ZoneObject::unregisterMovable: Movable to unregister was not found in this object" << std::endl;
    }
  }
  
  void ZoneObject::unregisterPhysics(IPhysics* physics) {
    if (Utils::removeByValue(cDefPhysics, physics) == 0) {
      std::cout << "WARNING: ZoneObject::unregisterPhysics: Physics to unregister was not found in this object" << std::endl;
    }
  }
  
  void ZoneObject::unregisterRenderer(IRenderer* renderer) {
    if (Utils::removeByValue(cDefRenderers, renderer) == 0) {
      std::cout << "WARNING: ZoneObject::unregisterRenderer: Renderer to unregister was not found in this object" << std::endl;
    }
  }
  
  void ZoneObject::unregisterProcessor(IProcessor* processor) {
    if (Utils::removeByValue(cDefProcessors, processor) == 0) {
      std::cout << "WARNING: ZoneObject::unregisterProcessor: Processor to unregister was not found in this object" << std::endl;
    }
  }
    
  ILocation* ZoneObject::getLocation(const std::string& id) const {
    std::map<std::string, ILocation*>::const_iterator mIterator = cDefLocations.find(id);
    if (mIterator == cDefLocations.end()) {
      throw ArgumentException("ERROR: ZoneObject::getLocation: Unknown location id \"" + id + "\"");
    }
    return mIterator->second;
  }

  IMovable* ZoneObject::getMovable(const std::string& id) const {
    std::map<std::string, IMovable*>::const_iterator mIterator = cDefMovables.find(id);
    if (mIterator == cDefMovables.end()) {
      throw ArgumentException("ERROR: ZoneObject::getMovable: Unknown movable id \"" + id + "\"");
    }
    return mIterator->second;
  }

  IPhysics* ZoneObject::getPhysics(const std::string& id) const {
    std::map<std::string, IPhysics*>::const_iterator mIterator = cDefPhysics.find(id);
    if (mIterator == cDefPhysics.end()) {
      throw ArgumentException("ERROR: ZoneObject::getPhysics: Unknown physics id \"" + id + "\"");
    }
    return mIterator->second;
  }

  IRenderer* ZoneObject::getRenderer(const std::string& id) const {
    std::map<std::string, IRenderer*>::const_iterator mIterator = cDefRenderers.find(id);
    if (mIterator == cDefRenderers.end()) {
      throw ArgumentException("ERROR: ZoneObject::getRenderer: Unknown renderer id \"" + id + "\"");
    }
    return mIterator->second;
  }

  IProcessor* ZoneObject::getProcessor(const std::string& id, bool required) const {
    std::map<std::string, IProcessor*>::const_iterator mIterator = cDefProcessors.find(id);
    if (mIterator == cDefProcessors.end()) {
      if (required) {
        throw ArgumentException("ERROR: ZoneObject::getProcessor: Unknown processor id \"" + id + "\"");
      } else {
        return nullptr;
      }
    }
    return mIterator->second;
  }

  Zone& ZoneObject::getZone() {
    return cZone;
  }
  
  bool ZoneObject::contains(const LiteralVertex& location) const {
    for (std::pair<std::string, ILocation*> mPair : cDefLocations) {
      if (location.x > mPair.second->getX() - 0.5f && location.x < mPair.second->getX() + 0.5f
       && location.y > mPair.second->getY() - 0.5f && location.y < mPair.second->getY() + 0.5f
       && location.z > mPair.second->getZ() - 0.5f && location.z < mPair.second->getZ() + 0.5f) {
        return true;
      }
    }
    return false;
  }

  void ZoneObject::renderSelectionHighlight() const {
    // TODO: Implement this.
  }

  void ZoneObject::remove() {
    cZone.remove(this);
  }

  std::vector<std::unique_ptr<IProperty>> ZoneObject::getProperties() {
    return std::vector<std::unique_ptr<IProperty>>();
  }

  std::string ZoneObject::getTypeName() const {
    return "Object";
  }

  Zone& ZoneObject::getObjectZone() {
    return cZone;
  }

  ZoneObject::TraitRegistry::TraitRegistry(ZoneObject& parent, const std::string& id) :
            cParent(parent),
            cID(id) {
  }
  
  void ZoneObject::TraitRegistry::registerLocation(ILocation* location) {
    cParent.registerLocation(cID, location);
  }
  
  void ZoneObject::TraitRegistry::registerMovable(IMovable* movable) {
    cParent.registerMovable(cID, movable);
  }
  
  void ZoneObject::TraitRegistry::registerProcessor(IProcessor* processor) {
    cParent.registerProcessor(cID, processor);
  }
  
  void ZoneObject::TraitRegistry::registerPhysics(IPhysics* physics) {
    cParent.registerPhysics(cID, physics);
  }
  
  void ZoneObject::TraitRegistry::registerRenderer(IRenderer* renderer) {
    cParent.registerRenderer(cID, renderer);
  }  
}
