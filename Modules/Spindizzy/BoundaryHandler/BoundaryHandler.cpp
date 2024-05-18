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

  BoundaryHandler::BoundaryHandler(IProject* project, Spindizzy* spindizzy) :
            cDefSpindizzy(*spindizzy),
            cDefBoundaryType(*spindizzy),
            cDefObjectType(*spindizzy),
            cDefEnteredAction(project),
            cDefExitedAction(project) {
  }

  BoundaryHandler::BoundaryHandler(IProject* project, Spindizzy* spindizzy, JSONObject object, IOptions* options, IResourceData* data) :
            BoundaryHandler(project, spindizzy) {
    cDefBoundaryType.init(object.getObject(JSON_BOUNDARY));
    cDefObjectType.init(object.getObject(JSON_OBJECT));
    cDefEnteredAction.init(object, JSON_ON_ENTRY, this);
    cDefExitedAction.init(object, JSON_ON_EXIT, this);
    spindizzy->getProject()->init([this, spindizzy](IAssets* assets) {
      spindizzy->added(this);
    });
  }

  void BoundaryHandler::registerAssets(IAssetRegistry* assets) {
    // Nothing to do.
  }

  void BoundaryHandler::unregisterAssets(IAssetRemover* assets, IAssets* releaser) {
    // Nothing to do.
  }

  void BoundaryHandler::save(JSONObject object, IAssetIdentifier* identifier) const {
    cDefBoundaryType.save(object, JSON_BOUNDARY);
    cDefObjectType.save(object, JSON_OBJECT);
    cDefSpindizzy.setBindingIdentifier(this);
    cDefEnteredAction.save(object, JSON_ON_ENTRY);
    cDefExitedAction.save(object, JSON_ON_EXIT);
    cDefSpindizzy.setBindingIdentifier(nullptr);
  }

  void BoundaryHandler::hintInUse(bool inUse) {
    // Nothing to do.
  }

  bool BoundaryHandler::renderIcon() {
    return false;
  }

  std::vector<IProperty*> BoundaryHandler::getProperties(IAssetBrowser* browser, IAssetRegistry* assets, IPropertyListener* listener) {
    return std::vector<IProperty*>({
    });
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
