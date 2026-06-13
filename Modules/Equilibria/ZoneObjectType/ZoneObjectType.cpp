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
#include "ZoneObjectType.h"

#include "Modules/Equilibria/Equilibria.h"
#include "Modules/Equilibria/World/World.h"

namespace IsoRealms::Equilibria {
  ZoneObjectType::ZoneObjectType(Equilibria& equilibria, IResourceData& data) :
            cEquilibria(equilibria),  
            cResourceData(data),
            cAssets(equilibria) {
  }
  
  void ZoneObjectType::registerAssets(ResourceAssetRegistry& assets) {
    // TODO
  }
    
  void ZoneObjectType::hintInUse(bool inUse) {
    // Nothing to do.
  }
  
  bool ZoneObjectType::renderIcon() const {
    return false; // TODO
  }

  void ZoneObjectType::getProperties(IPropertyMaker& owner, const Metadata& metadata) {
    // Nothing to do.
  }

  void ZoneObjectType::removed() {
    // Nothing to do.
  }

  bool ZoneObjectType::hasReadOnlyReferences() const {
    return cEquilibria.isUsedInReadOnlyWorld(*this);
  }

  void ZoneObjectType::overrideReadOnlyReferences() {
    cEquilibria.overrideReadOnlyWorlds(*this);
  } 

  ZoneObjectType::~ZoneObjectType() {
    cEquilibria.removeAll(this);
  }
  
  void ZoneObjectType::registerAssets(const std::string& parentID) {
    cAssets.add<IWorldEditorTool>(this, parentID, "Zone Object Types");
    for (const std::pair<const std::string, IZoneObjectTypeTrait*>& mPair : cDefTypeTraits) {
      mPair.second->registerAssets(cAssets, parentID + "/" + mPair.first);
    }
  }  
  
  Equilibria& ZoneObjectType::getEquilibria() {
    return cEquilibria;
  }

  IResourceData& ZoneObjectType::getResourceData() {
    return cResourceData;
  }
  
  void ZoneObjectType::registerEditor(IZoneObjectTraitEditor* editor) {
    cRuntimeEditors.emplace_back(editor);
  }
  
  IBinding* ZoneObjectType::getBinding(const std::string& id) const {
//     std::size_t mSplit = id.find('/');
//     std::string mBindTo = mSplit == std::string::npos ? id : id.substr(0, mSplit);
//     std::string mSubID = mSplit == std::string::npos ? "" : id.substr(mSplit + 1);
//     return mBindTo == BIND_TO_ZONE  ? cDefZone.getZoneBinding(mSubID)
//          : mBindTo == BIND_TO_TRAIT ? getTraitBinding(mSubID)
//          :                            nullptr;
    return nullptr;
  }
  
  std::string ZoneObjectType::getBindingID(const IBinding* binding) const {
    return "";
  }
  
  std::map<std::string, std::unique_ptr<IZoneObjectTrait>> ZoneObjectType::createTraits(ZoneObject& object) const {
    std::map<std::string, std::unique_ptr<IZoneObjectTrait>> mObjectTraits;
    for (const std::pair<const std::string, IZoneObjectTypeTrait*>& mPair : cDefTypeTraits) {
      mObjectTraits.emplace(std::string(mPair.first), mPair.second->createTrait(object));
    }
    return mObjectTraits;
  }
  
  std::string ZoneObjectType::getID(const IZoneObjectTypeTrait* trait) const {
    for (const std::pair<const std::string, IZoneObjectTypeTrait*>& mPair : cDefTypeTraits) {
      if (mPair.second == trait) {
        return mPair.first;
      }
    }
    throw ArgumentException("ERROR: ZoneObjectType::getID: Specified trait not registered with this object type");
  }
  
  std::string ZoneObjectType::getRuntimeRendererID() const {
    return cDefRuntimeRendererID;
  }

  std::string ZoneObjectType::getEditingRendererID() const {
    return cDefEditingRendererID;
  }

  std::string ZoneObjectType::getRuntimeProcessorID() const {
    return cDefRuntimeProcessorID;
  }

  std::string ZoneObjectType::getEditingProcessorID() const {
    return cDefEditingProcessorID;
  }

  IWorldEditorToolInstance* ZoneObjectType::createToolInstance(WorldEditor& editor, IResourceData& owner) {
    return cEditingPens.emplace_back(std::make_unique<Pen>(*this, editor)).get();
  }

  bool ZoneObjectType::renderAssetIcon() const {
    return false;
  }

  void ZoneObjectType::saveAsset(JSONObject object) const {
    // Nothing to do.
  }

  void ZoneObjectType::getAssetProperties(IPropertyMaker& owner) {
    // Nothing to do.
  }

  bool ZoneObjectType::isDefaultConfiguration() const {
    return true;
  }

  ZoneObjectType::Pen::Pen(ZoneObjectType& parent, WorldEditor& editor) :
            cParent(parent),
            cEditor(editor) {
  }

  bool ZoneObjectType::Pen::isTool(IWorldEditorTool* tool) const {
    return tool == &cParent;
  }

  bool ZoneObjectType::Pen::renderIcon(float yaw) const {
    return cParent.renderIcon();
  }

  void ZoneObjectType::Pen::renderEditingPreview() const {
    // TODO
  }

  void ZoneObjectType::Pen::renderUI(float aspectRatio) const {
    // Nothing to do.
  }

  void ZoneObjectType::Pen::updateUI(unsigned int milliseconds) {
    // Nothing to do.
  }

  bool ZoneObjectType::Pen::inputTool(sf::Event& event) {
    return false;
  }

  bool ZoneObjectType::Pen::inputTool(SignalInputID id, double yaw) {
    // TODO: Implement this.
//     switch (cRuntimeEditors[cEditingTrait]->inputEdit(event)) {
//       case IZoneObjectTraitEditor::InputEditResult::COMPLETED: {
//         if (cEditingTrait == 0) {
//           cPinnedZone = cEditor.getWorld().getOrDrawZone(cEditor.getCursorCell(), cEditor);
//         }
//
//         cEditingTrait++;
//         if (cEditingTrait == cRuntimeEditors.size()) {
//           cPinnedZone->draw(cParent);
//         }
//         return true;
//       }
//
//       case IZoneObjectTraitEditor::InputEditResult::MODIFIED: {
//         return true;
//       }
//
//       case IZoneObjectTraitEditor::InputEditResult::CANCELLED: {
//         if (cEditingTrait > 0) {
//           cEditingTrait--;
//           if (cEditingTrait == 0) {
//             if (cPinnedZone.empty()) {
//               cEditor.getWorld().remove(cPinnedZone);
//             }
//             cPinnedZone = nullptr;
//           }
//         }
//         return true;
//       }
//
//       default: {
//         return false;
//       }
//     }
    return false;
  }

  bool ZoneObjectType::Pen::isCursorLocked() const {
    return false;
  }

  void ZoneObjectType::Pen::processCursorMovement(LiteralVertex* start, LiteralVertex* end) {
//     if (cPinnedZone != nullptr) {
//       cPinnedZone->processCursorMovement(*end);
//       cRuntimeEditors[cEditingTrait]->processCursorMovement(*end);
//     }
  }

  double ZoneObjectType::Pen::getSnapInterval() const {
    return 1.0f; // cRuntimeEditors[cEditingTrait]->getSnapInterval();
  }
  
  IBinding* ZoneObjectType::getTraitBinding(const std::string& id) const {
//     std::size_t mSplit = id.find('/');
//     std::string mBindTo = mSplit == std::string::npos ? id : id.substr(0, mSplit);
//     std::string mSubID = mSplit == std::string::npos ? "" : id.substr(mSplit + 1);
//     std::map<std::string, std::unique_ptr<IZoneObjectTrait>>::iterator mIterator = cDefTraits.find(mBindTo);
//     return mIterator != cDefTraits.end() ? mIterator->second->getTraitBinding(mSubID) : nullptr;
    return nullptr;
  }
}
