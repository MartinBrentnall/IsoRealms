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
  const std::string BoundaryHandler::TAG_BOUNDARY_TYPE  = "Type";
  const std::string BoundaryHandler::TAG_ENTERED_ACTION = "EnteredAction";
  const std::string BoundaryHandler::TAG_EXITED_ACTION  = "ExitedAction";
  const std::string BoundaryHandler::TAG_OBJECT_TYPE    = "Object";

  const std::string BoundaryHandler::BIND_TO_BOUNDARY = "Boundary";
  const std::string BoundaryHandler::BIND_TO_OBJECT   = "Object";

  BoundaryHandler::BoundaryHandler(IProject* project, Spindizzy* spindizzy) :
            cDefSpindizzy(*spindizzy),
            cDefBoundaryType(*spindizzy),
            cDefObjectType(*spindizzy),
            cDefEnteredAction(project),
            cDefExitedAction(project) {
  }

  BoundaryHandler::BoundaryHandler(IProject* project, Spindizzy* spindizzy, DOMNode& node, IOptions* options, IResourceData* data) :
            BoundaryHandler(project, spindizzy) {
    cDefBoundaryType.init(node.getNode(TAG_BOUNDARY_TYPE));
    cDefObjectType.init(node.getNode(TAG_OBJECT_TYPE));
    cDefEnteredAction.init(node, TAG_ENTERED_ACTION, this);
    cDefExitedAction.init(node, TAG_EXITED_ACTION, this);
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

  void BoundaryHandler::save(DOMNodeWriter* node, IAssetIdentifier* identifier) const {
    cDefBoundaryType.save(node, TAG_BOUNDARY_TYPE);
    cDefObjectType.save(node, TAG_OBJECT_TYPE);
    cDefSpindizzy.setBindingIdentifier(this);
    cDefEnteredAction.save(node, TAG_ENTERED_ACTION);
    cDefExitedAction.save(node, TAG_EXITED_ACTION);
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
  
  void BoundaryHandler::saveBinding(DOMNodeWriter* node, const IBinding* binding) const {
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
