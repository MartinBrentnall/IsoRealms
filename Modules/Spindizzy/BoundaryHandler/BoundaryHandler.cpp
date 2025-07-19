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
#include "BoundaryHandler.h"

#include "Modules/Spindizzy/Spindizzy.h"
#include "Modules/Spindizzy/BoundaryHandler/BoundaryHandlerInstance.h"

namespace IsoRealms::Spindizzy {
  const std::string BoundaryHandler::JSON_BOUNDARY = "boundary";
  const std::string BoundaryHandler::JSON_OBJECT   = "object";
  const std::string BoundaryHandler::JSON_ON_ENTRY = "onEntry";
  const std::string BoundaryHandler::JSON_ON_EXIT  = "onExit";

  const std::string BoundaryHandler::BIND_TO_BOUNDARY = "Boundary";
  const std::string BoundaryHandler::BIND_TO_OBJECT   = "Object";

  BoundaryHandler::BoundaryHandler(IProject& project, Spindizzy& spindizzy, IResourceData& data) :
            cSpindizzy(spindizzy),
            cActionClient(data, *this),
            cDefBoundaryType(spindizzy),
            cDefObjectType(spindizzy),
            cDefEnteredAction(cActionClient),
            cDefExitedAction(cActionClient) {
  }

  BoundaryHandler::BoundaryHandler(IProject& project, Spindizzy& spindizzy, IResourceData& data, JSONObject object, IOptions& options) :
            BoundaryHandler(project, spindizzy, data) {
    cDefBoundaryType.init(object, JSON_BOUNDARY);
    cDefObjectType.init(object, JSON_OBJECT);
    cDefEnteredAction.init(object, JSON_ON_ENTRY);
    cDefExitedAction.init(object, JSON_ON_EXIT);
    spindizzy.getProject().init([this, &spindizzy](IAssets& assets) {
      spindizzy.added(this);
    });
  }

  void BoundaryHandler::registerAssets(ResourceAssetRegistry& assets) {
    // Nothing to do.
  }

  void BoundaryHandler::save(JSONObject object) const {
    cDefBoundaryType.save(object, JSON_BOUNDARY);
    cDefObjectType.save(object, JSON_OBJECT);
    cSpindizzy.setBindingIdentifier(this);
    cDefEnteredAction.save(object, JSON_ON_ENTRY);
    cDefExitedAction.save(object, JSON_ON_EXIT);
    cSpindizzy.setBindingIdentifier(nullptr);
  }

  void BoundaryHandler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool BoundaryHandler::renderIcon() {
    return false;
  }

  std::vector<std::unique_ptr<IProperty>> BoundaryHandler::getProperties(IResourceData& owner) {
    std::vector<std::unique_ptr<IProperty>> mProperties;
    mProperties.emplace_back(std::make_unique<PropertyAsset<PhysicalObjectType>>(owner.getPropertyData("EntityType"),   cDefObjectType));
    mProperties.emplace_back(std::make_unique<PropertyAsset<BoundaryType>>(      owner.getPropertyData("BoundaryType"), cDefBoundaryType));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>(            owner.getPropertyData("OoEntry"),      cDefEnteredAction));
    mProperties.emplace_back(std::make_unique<PropertyAsset<Action>>(            owner.getPropertyData("OnExit"),       cDefExitedAction));
    return mProperties;
  }

  const BoundaryType* BoundaryHandler::getBoundaryType() const {
    return &cDefBoundaryType;
  }

  const PhysicalObjectType* BoundaryHandler::getObjectType() const {
    return &cDefObjectType;
  }

  std::unique_ptr<BoundaryHandlerInstance> BoundaryHandler::createInstance(World* world) {
    return std::make_unique<BoundaryHandlerInstance>(this, world);
  }

  void BoundaryHandler::executeAction(bool exited) {
    (exited ? cDefExitedAction : cDefEnteredAction).execute();
  }
  
  IBinding* BoundaryHandler::getBinding(const std::string& id) {
    std::size_t mSplit = id.find('/');
    std::string mBindTo = id.substr(0, mSplit);
    std::string mBindPath = id.substr(mSplit + 1);
    return mBindTo == BIND_TO_BOUNDARY ? cDefBoundaryType.getBoundaryBinding(mBindPath)
         : mBindTo == BIND_TO_OBJECT   ? cDefObjectType.getBinding(mBindPath)
         :                               nullptr;
  }
  
  void BoundaryHandler::saveBinding(JSONObject object, const IBinding* binding) const {
    // TODO: Implement this.
  }

  std::string BoundaryHandler::getBindingID(const IBinding* binding) const {
    std::string mBindingID = cDefBoundaryType.getBoundaryBindingID(binding);
    if (mBindingID != "") {
      return BIND_TO_BOUNDARY + "/" + mBindingID;
    }
    return BIND_TO_OBJECT + "/" + cDefObjectType.getBindingID(binding);
  }
  
  void BoundaryHandler::releaseBinding(const IBinding* asset) {
    // Nothing to do.
  }
}
